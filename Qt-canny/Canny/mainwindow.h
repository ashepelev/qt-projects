#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "worker.h"
#include <QMainWindow>
#include <QThread>
#include <QLabel>
#include <QScrollArea>
#include <QSpinBox>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void myResize(int w, int h);

public slots:
    void openFile();
    void Launch();
    void createMenus();

private slots:
    void onHandleStart();
    void onHandleResultReady();
    void onHandleFinish();
    void launchAbout();

private:
    QThread * workThread;
    Ui::MainWindow *ui;
    QString filename;
    QLabel * label;
    Worker * worker;
    QScrollArea * scrollArea;
    QAction *open,*exit;
    QPushButton * launch;
    QSpinBox * spinbox;
    QDoubleSpinBox * doubleSpinBox;
};

#endif // MAINWINDOW_H
