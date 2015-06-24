#ifndef REGISTRYMODEL_H
#define REGISTRYMODEL_H

#include <QAbstractItemModel>
#include <QMimeData>
#include "tablemodel.h"

class Item
{
public:
    QVariant * data;
    int columnCount;
    int rowCount;
    QString * type;
    bool expanded;
    QString * fullpath;
    QList<Item*> * childs;
    Item * parent;
    int row;
    int column;
    QString * directory;
    QList<TableItem*> * keys;
    Item();
    ~Item();
};


class RegistryModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    RegistryModel(const QString &data, QObject * parent = 0);
    ~RegistryModel();
    // По индексу получаем содержимое
    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation,
                     int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(int row, int column,
                   const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    void CopyGroup(Item * from, const QString& directoryTo);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    Qt::DropActions supportedDropActions() const;
    QMimeData* mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    QStringList mimeTypes() const;

 //   QAbstractItemModel::m
    void storeItems(Item * item);


public slots:
    void myClickProcess(const QModelIndex &index);

private:
     Item * rootItem;
     QMap<QString,Item*> * treeMap;

     // QAbstractItemModel interface
public:
     Qt::DropActions supportedDragActions() const;
};



#endif // REGISTRY_MODEL_H
