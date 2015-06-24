#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(this,SIGNAL(accepted()),this,SLOT(getOutput()));

}

void Dialog::setLast(gipoParam params)
{
    ui->lineEdit->setText(QString::number(params.bigR));
    ui->lineEdit_2->setText(QString::number(params.smallR));
    ui->lineEdit_3->setText(QString::number(params.d));
    ui->lineEdit_4->setText(QString::number(params.tmax));
    ui->lineEdit_5->setText(QString::number(params.deltat));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::getOutput()
{
    this->result.bigR = (ui->lineEdit->text()).toDouble();
    this->result.smallR = (ui->lineEdit_2->text()).toDouble();
    this->result.d = (ui->lineEdit_3->text()).toDouble();
    this->result.tmax = (ui->lineEdit_4->text()).toDouble();
    this->result.deltat = (ui->lineEdit_5->text()).toDouble();
}
