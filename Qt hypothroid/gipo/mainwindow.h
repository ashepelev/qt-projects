#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
//#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
struct gipoParam;
}

struct gipoParam {
        qreal bigR;
        qreal smallR;
        qreal d;
        qreal tmax;
        qreal deltat;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;

public slots:
    void showDialog();
    void showAbout();
};

class MyDrawWidget : public QWidget
{
    Q_OBJECT
public :
    MyDrawWidget(QWidget * parent = 0);
    void drawGipo(QPainter * scene, gipoParam param);
    void setParams(gipoParam param);
    void needResize();
    gipoParam last_params;
protected:
    void paintEvent(QPaintEvent * pEvent);
    void resizeEvent(QResizeEvent * rEvent);
private:
    QPixmap m_Pixmap;

};


#endif // MAINWINDOW_H
