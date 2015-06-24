#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->label->setText("<a href='mailto:shepelev.artem@gmail.com'>Написать разработчику</a>");
    ui->label->setOpenExternalLinks(true);
    ui->textEdit->setText("Программа рисует гипотрохоиду.\
                          Воспользуйтесь меню Правка->Входные данные, чтобы задать параметры гипотрохоиды.\
                          Разработчик: Артем Шепелев.");
    //connect(this,SIGNAL(accepted()),this,SLOT(getOutput()));

}

About::~About()
{
    delete ui;
}

