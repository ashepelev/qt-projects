#include "mainwindow.h"
#include "dialog.h"
#include "about.h"
#include "math.h"
#include <QResizeEvent>
#include <QPainter>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->action_2,SIGNAL(triggered()),this,SLOT(showDialog()));
    connect(ui->action_3,SIGNAL(triggered()),this,SLOT(showAbout()));
    this->setCentralWidget(new MyDrawWidget(this));
}

void MyDrawWidget::drawGipo(QPainter * painter, gipoParam param)
{
    double t = 0.0;
    qreal minx = 1000, maxx = -1000, miny = 1000, maxy = -1000;
    qreal x = (param.bigR - param.smallR)*cos(t) + param.d*cos(((param.bigR-param.smallR)/param.smallR)*t);
    qreal y = (param.bigR - param.smallR)*sin(t) - param.d*sin(((param.bigR-param.smallR)/param.smallR)*t);
    QPen pen(Qt::black);
    while (t < param.tmax)
    {
        qreal x1 = (param.bigR - param.smallR)*cos(t) + param.d*cos((param.bigR/param.smallR-1)*t);
        qreal y1 = (param.bigR - param.smallR)*sin(t) - param.d*sin((param.bigR/param.smallR-1)*t);
        if (x1 > maxx)
            maxx = x1;
        if (x1 < minx)
            minx = x1;
        if (y1 > maxy)
            maxy = y1;
        if (y1 < miny)
            miny = y1;
        painter->drawLine((int)x,(int)y,(int)x1,(int)y1);
        t+=param.deltat;
        x = x1;
        y = y1;
    }
}


void MainWindow::showDialog()
{
    Dialog d;
    QWidget * central = this->centralWidget();
    d.setLast(((MyDrawWidget*)central)->last_params);
    if (d.exec())
    {
        ((MyDrawWidget*)central)->setParams(d.result);
        ((MyDrawWidget*)central)->needResize();
    }
}

void MainWindow::showAbout()
{
    About a;
    a.exec();
}


MainWindow::~MainWindow()
{
    delete ui;
}

MyDrawWidget::MyDrawWidget(QWidget *parent) : QWidget(parent)
{
    this->last_params.bigR = 200.0;
    this->last_params.smallR = 40.0;
    this->last_params.d = 90.0;
    this->last_params.tmax = 70.0;
    this->last_params.deltat = 0.1;
}

void MyDrawWidget::paintEvent(QPaintEvent *pEvent)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,m_Pixmap);
}

void MyDrawWidget::resizeEvent(QResizeEvent * rEvent)
{
    m_Pixmap = QPixmap(rEvent->size());
    m_Pixmap.fill(Qt::white);
    QPainter painter(&m_Pixmap);
    painter.scale(-1,1);
    painter.translate(-width()/2,height()/2);
    MyDrawWidget::drawGipo(&painter,this->last_params);
    this->resize(rEvent->size());
}

void MyDrawWidget::needResize()
{
    const QSize sz(this->width(),this->height());
    this->resizeEvent(new QResizeEvent(sz,sz));
}

void MyDrawWidget::setParams(gipoParam param)
{
    this->last_params = param;
}
