#ifndef MYTREEVIEW_H
#define MYTREEVIEW_H

#include <QTreeView>
#include <QDebug>

class myTreeView :public QTreeView
{

    Q_OBJECT
public:
    myTreeView(QWidget *parent = 0);


public slots:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
};

#endif // MYTREEVIEW_H
