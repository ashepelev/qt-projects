#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>

struct TableItem
{
    QVariant value;
    QString type;
    QString key;
    QString fullpath;
};

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TableModel(QObject * parent = 0);
    ~TableModel();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  //  QModelIndex index(int row, int column, const QModelIndex &parent) const;
 //   QModelIndex parent(const QModelIndex &index) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    QStringList header_data;
    QList<TableItem*> * list;
public slots:
    void getKeys(const QModelIndex &index, QObject * parent = 0);


};

#endif // TABLEMODEL_H
