#include "registrymodel.h"
#include "tablemodel.h"
#include <QSettings>
#include <QDebug>

/* Модель для хранения информации о ключах ветки */
TableModel::TableModel(QObject * parent) : QAbstractTableModel(parent)
{
    header_data << tr("Key") << tr("Type") << tr("Value");
    list = new QList<TableItem*>();
}

TableModel::~TableModel()
{
    delete this->list;
}

int TableModel::rowCount(const QModelIndex &) const
{
    return list->size();
}
int TableModel::columnCount(const QModelIndex &) const
{
    return 3; // value, default value, type
}
QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= list->size())
        return QVariant();
 //   if (role == Qt::EditRole)
 //       return QVariant();
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();
    if (index.column() == 0)
        return list->at(index.row())->key;
    if (index.column() == 1)
        return list->at(index.row())->type;
    if (index.column() == 2)
        return list->at(index.row())->value;
    return QVariant();
}

/* функция фиксирования изменений в реестре */
bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        if (index.column() == 2)
        {
            list->at(index.row())->value = value.toString();
            QSettings settings(list->at(index.row())->fullpath,QSettings::NativeFormat);
            settings.setValue(list->at(index.row())->key,value);
            settings.sync();
        }
        return true;
    }
    return false;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return header_data.at(section);
    else
        return QString("%1").arg(section+1);
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

/* Функция получения ключей из ветки */
void TableModel::getKeys(const QModelIndex &index, QObject *)
{
    Item * item = static_cast<Item*>(index.internalPointer());
    QList<TableItem*> * keys = item->keys;
    this->list = new QList<TableItem*>();
    foreach (TableItem* key, *keys)
    {
        this->list->append(key);
    }
    emit layoutChanged();
}
