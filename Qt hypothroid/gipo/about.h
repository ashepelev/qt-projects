#ifndef ABOUT_H
#define ABOUT_H

#include <QMainWindow>
#include <QWidget>
#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();
protected:
private:
    Ui::About *ui;
};


#endif // ABOUT_H
