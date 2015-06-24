#include "registrymodel.h"
#include <QCoreApplication>
#include <QDebug>
#include <QSettings>

Item::Item()
{

}

Item::~Item()
{
    delete data;
    delete type;
    delete fullpath;
    delete childs;
    delete directory;
    delete keys;
}

RegistryModel::RegistryModel(const QString &data, QObject * parent)
     : QAbstractItemModel(parent)
{

    //QCoreApplication::setApplicationName(data);
    // Регистрируем корень
    Item * it = new Item();
    it->data = new QVariant(data);
    it->columnCount = 1;
    it->expanded = false;
    it->fullpath = new QString(data);
    rootItem = it;
    rootItem->row = 0;
    rootItem->column = 0;

    treeMap = new QMap<QString,Item*>();
  //  treeMap->insert(data,it);
    storeItems(rootItem);
}
/* Регистрируем наследников 1 уровня (спуск на 1 вглубину) и его ключи */
void RegistryModel::storeItems(Item * item)
{
    QSettings settings(*(item->fullpath),QSettings::NativeFormat);
    QStringList keys = settings.childGroups();
    if (keys.count() != 0) {
        item->childs = new QList<Item*>();
        item->rowCount = 0;
    }
    else
        return;
    int iter = 0;
    foreach (QString key, keys) // Записываем наследников 1 уровня
    {
        Item * child = new Item();
        child->data = new QVariant(key);
        child->childs = new QList<Item*>();
        child->columnCount = 1;
        child->rowCount = 0;
        child->fullpath = new QString(*item->fullpath + "\\" + key );
        child->expanded = false;
        child->row=iter++;
        child->parent=item;
        child->column = 0;
        child->directory = new QString(*item->fullpath);        
        item->rowCount++;
        item->childs->append(child);
        QSettings keysSettings(*child->fullpath,QSettings::NativeFormat);
        QStringList realKeys = keysSettings.childKeys();
        child->keys = new QList<TableItem*>();
        // Делаем ложный ключ (по-умолчанию) для копирования пустых веток
        TableItem * def = new TableItem();
        def->fullpath = QString(*child->fullpath);
        def->key = QString("Default");
        def->value = "";
        def->type = "";
        child->keys->append(def);
        foreach (QString keych, realKeys) // Записываем ключи данного пути
        {
            QVariant value = keysSettings.value(keych);
            TableItem * titem = new TableItem();
            titem->key = keych;
            titem->type = value.typeName();
            titem->value = value.toString();
            titem->fullpath = QString(*child->fullpath);
            child->keys->append(titem);
        }
        treeMap->insert(*child->fullpath,child);
    //    storeItems(child);
    }
}

RegistryModel::~RegistryModel()
{
    delete rootItem;
    delete treeMap;
}

/* Переопределение для получения индекса ячейки */
QModelIndex RegistryModel::index(int row, int column, const QModelIndex &parent) const
 {
    if (!hasIndex(row, column, parent))
        return QModelIndex();
    Item * parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<Item*>(parent.internalPointer());
    if (row < parentItem->childs->count())
        return createIndex(row,column,parentItem->childs->at(row));
    else
        return QModelIndex();
 }

/* Переопределения для получения родителя элемента ячейки */
QModelIndex RegistryModel::parent(const QModelIndex &index) const
 {
     if (!index.isValid())
         return QModelIndex();

     Item *childItem = static_cast<Item*>(index.internalPointer());
     Item *parentItem = childItem->parent;

     if (parentItem == rootItem)
         return QModelIndex();

     return createIndex(parentItem->row, 0, parentItem);
 }

/* Одноуровневое дерево, колонок вседа 1 */
int RegistryModel::columnCount(const QModelIndex &) const
 {
    return 1;
 }

/* Строк столько, сколько наследников у элемента */
int RegistryModel::rowCount(const QModelIndex &parent) const
 {
     Item *parentItem;
     if (parent.column() > 0)
         return 0;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<Item*>(parent.internalPointer());

     return parentItem->childs->count();
 }

/* Рекурсивная функция копирования ячейки в directoryTo */
void RegistryModel::CopyGroup(Item * from, const QString& directoryTo)
{

    QSettings settings(directoryTo,QSettings::NativeFormat); // В этой группе создаем детей старой
    settings.beginGroup(from->data->toString());
    foreach (TableItem * titem, *from->keys)
    {
        settings.setValue(titem->key,titem->value);
    }
    settings.endGroup();
    foreach (Item * child, *from->childs)
        CopyGroup(child,directoryTo+"\\"+from->data->toString());
}

/* Переопределенные функции для работы drag'n'drop */
Qt::DropActions RegistryModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

/* Создаем MIME объект для drag'n'drop */
QMimeData* RegistryModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData * mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes) {
        if (index.isValid())
        {
            Item *item = static_cast<Item*>(index.internalPointer());
            QString text = *item->fullpath;
            stream << text; // записываем полный путь к объекту для однозначного определения
        }
    }

    mimeData->setData("application/registryitem", encodedData);
    return mimeData;
}

/* Выгружаем Mime объект в место drop'а */
bool RegistryModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int, int , const QModelIndex &parent)
{
    qDebug() << "dropMimeData";
    if (action == Qt::IgnoreAction)
        return true;
    if (!data->hasFormat("application/registryitem"))
        return false;
    QByteArray encodedData = data->data("application/registryitem");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QStringList newItems;
    int rows = 0;
    QList<Item*> * items = new QList<Item*>(); // нельзя удалить, т.к. содержит ссылки из treeMap
    // Получаем данные об источнике
    while (!stream.atEnd())
    {
        QString text; // fullpath
        stream >> text;
        qDebug() << text;
        newItems << text;
        ++rows;
        items->append((*this->treeMap)[text]);
    }
    Item * par = static_cast<Item*>(parent.internalPointer());
    QString groupTo = *par->fullpath;
    qDebug() << groupTo;
    // Загружаем их в требуемую ветку
    foreach (Item* item, *items)
    {
        qDebug() << *item->fullpath;

        CopyGroup(item,groupTo);
    }
    storeItems(par); // Перечитываем значения
    emit layoutChanged();  // Сообщаем об изменение значений

    return true;
}

QStringList RegistryModel::mimeTypes() const
{
    QStringList types;
    types << "application/registryitem"; // Произвольный формат
    return types;
}

/* Флаги с учетом возможности drag'n'drop */
Qt::ItemFlags RegistryModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags nReturn = QAbstractItemModel::flags(index);

    nReturn |= Qt::ItemIsDropEnabled;

    if (index.isValid())
    {
        nReturn |= Qt::ItemIsSelectable;
        nReturn |= Qt::ItemIsDragEnabled;
        nReturn |= Qt::ItemIsDropEnabled;
        nReturn |= Qt::ItemIsEnabled;
    }
    return nReturn;

}

/* Данные ячейки */
QVariant RegistryModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (role != Qt::DisplayRole)
         return QVariant();
     Item *item = static_cast<Item*>(index.internalPointer());
     return *item->data;
 }

QVariant RegistryModel::headerData(int, Qt::Orientation,
                                int) const
 {
        return QVariant(*rootItem->fullpath);
 }

/* Обработка дозагрузки данных с реестра */
void RegistryModel::myClickProcess(const QModelIndex &index)
{
    Item *item = static_cast<Item*>(index.internalPointer());
    storeItems(item);
    emit layoutChanged();
}

Qt::DropActions RegistryModel::supportedDragActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}
