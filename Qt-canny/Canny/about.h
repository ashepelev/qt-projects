#ifndef ABOUT_H
#define ABOUT_H

//#include <QDialog>
#include "mainwindow.h"
#include "about.h"
#include "ui_about.h"

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
