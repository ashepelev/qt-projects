#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QStringList * paths;
    QMdiArea * mdiArea;
    QMap<QString,QMenu*> * menuMap;
    QMap<QAction*,QWidget*> * viewMap;
    QString last;
    QString examplePath;

    void CreateMenus();
    void LoadSubWindow(const QString& path);
    void LoadLast(const QString& file);
    void LoadNewToFile(const QString& file, const QString& newpath);
    void LoadIntoView(const QString& path, QWidget * widget);
    void LoadLastOpened();

public slots:
    void ShowDialog();
    void CloseSubWindows();
    void ShowAbout();
    void FocusWindow();
    void LoadFromLastMenu();
    void RemoveFromView();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
