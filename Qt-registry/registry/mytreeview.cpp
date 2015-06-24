#include "mytreeview.h"
#include "registrymodel.h"
#include <QDragEnterEvent>
#include <QTreeWidgetItem>
#include <QDrag>

myTreeView::myTreeView(QWidget *parent) : QTreeView(parent)
{

}

void myTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "Hello dragger";
    if (event->mimeData()->hasFormat("application/registryitem"))
    {
        qDebug() << "Format";
        event->acceptProposedAction();
    }
}

void myTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void myTreeView::dropEvent(QDropEvent *event)
{
    qDebug() << "Drop Event";
 //   textBrowser->setPlainText(event->mimeData()->text());
   //  mimeTypeCombo->clear();
   //  mimeTypeCombo->addItems(event->mimeData()->formats());
     event->acceptProposedAction();
}

/*
void myTreeView::mouseMoveEvent(QMouseEvent *event)
{
    QModelIndex index =  this->indexAt(event->pos());
    RegistryModel * model = this->model();
    QModelIndexList indexes;
    indexes.append(index);
    if (event->button() == Qt::LeftButton)
    {
        QMimeData * md = model->mimeData(indexes);
        QDrag * drag = new QDrag(this);
        drag->setMimeData(md);
        Qt::DropAction dropAction = drag->exec(Qt::MoveAction);
         if (dropAction == Qt::MoveAction)
         {
            UT::getInstance()->LogToFile("dropAction");
         }
    }
}*/
