#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    /* Окно помощи */
    ui->setupUi(this);
    QString writeDev = "Write author";
    ui->label->setText("<a href='mailto:shepelev.artem@gmail.com'>+" + writeDev + "</a>");
    ui->label->setOpenExternalLinks(true);
    this->setWindowTitle("About programm");
    QString desc = About::tr("\
Данная программа находит края у изображения\n\
Сначала выполняется приведение к серому\n\
Затем выполняется поиск порога бинарного изображения по методу Отцу\n\
После этого выполняется поиск краев Canny\n\
Затем выполняется совмещение оригинального изображения и найденной карты с краев.\n\
Пользователь может изменить параметр найденного порога и пересчитать края.\n\
Автор: Артем Шепелев");
    ui->textEdit->setText(desc);
}

About::~About()
{
    delete ui;
}

