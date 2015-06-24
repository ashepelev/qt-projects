#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(this,SIGNAL(accepted()),this,SLOT(getOutput()));
    this->setWindowTitle(Dialog::tr("Open registry"));
    ui->label->setText(Dialog::tr("Enter register root path"));
    ui->label_2->setText(Dialog::tr("Path examples"));

    // Примеры веток
    QStringList * list = new QStringList();
    list->append("HKEY_CURRENT_USER\\Software\\Microsoft");
    list->append("HKEY_CURRENT_USER\\Control Panel\\Desktop");
    list->append("HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP");

    ui->comboBox->addItems(*list);
    ui->comboBox->setCurrentIndex(-1);
    connect(ui->comboBox,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setPath(const QString&)));
}

void Dialog::getOutput()
{
    this->path = (ui->lineEdit->text());
}

void Dialog::setPath(const QString& path)
{
    ui->lineEdit->setText(path);
}

Dialog::~Dialog()
{

}
