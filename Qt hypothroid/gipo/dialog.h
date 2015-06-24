#ifndef DIALOG_H
#define DIALOG_H

#include <QMainWindow>
#include <QWidget>
#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    gipoParam result;

    void setLast(gipoParam params);
    //void MainWindow::paintEvent(QPaintEvent * pEvent);
public slots:
    void getOutput();
protected:
    //void resizeEvent(QResizeEvent * pEvent);
    //void paintEvent(QPaintEvent * pEvent);
private:
    Ui::Dialog *ui;
};


#endif // DIALOG_H
