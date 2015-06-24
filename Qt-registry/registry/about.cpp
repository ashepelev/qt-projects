#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    /* Окно помощи */
    ui->setupUi(this);
    QString writeDev = About::tr("Write author");
    ui->label->setText("<a href='mailto:shepelev.artem@gmail.com'>+" + writeDev + "</a>");
    ui->label->setOpenExternalLinks(true);
    this->setWindowTitle(About::tr("About programm"));
    QString desc = About::tr("\
This programm allows you to work with registry and settings.\n\
To open the registry select File -> Open.\n\
Enter the registry path with back slashes \'\\\' or select the example path.\n\
Recently opened registry path displayed in File menu.\n\
To close the application choose File -> Exit.\n\
View menu allows to switch focus on opened registry paths\n\
or to close all the subwindows.\nAuthor: Artem Shepelev");
    ui->textEdit->setText(desc);
}

About::~About()
{
    delete ui;
}

