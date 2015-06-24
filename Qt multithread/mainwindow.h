#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "worker.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool loadFile(const QString &fileName);
signals:
    void m_pActionStart();
private slots:
//
    void on_m_pActionStart_triggered();
    void on_HoughCircle_triggered();
    void onHandleFinish();
    void on_action_triggered();
    void onHandleResultReady(QPixmap chResult);

private:
    string filepath;
    volatile bool m_vbFinish;
    QThread m_aWorkThreads[2];
    Worker* w;
    QThread t;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
