#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "registrymodel.h"
#include "tablemodel.h"
#include "dialog.h"
#include "about.h"
#include "mytreeview.h"
#include "spinboxdelegate.h"
#include <QTreeView>
#include <QSplitter>
#include <QTableView>
#include <QDesktopWidget>
#include <QMdiArea>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include <QWidget>
#include <QMdiSubWindow>
#include <QFile>

MainWindow::MainWindow(
        QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    paths = new QStringList();
    this->CreateMenus();
    this->last = "last_opened.txt"; // Файл с последними открытыми ветвями
    this->examplePath = "HKEY_CURRENT_USER\\Software\\Microsoft"; // Первая запись в этом файле
    paths->append(this->examplePath);
    this->LoadLast(this->last);
    viewMap = new QMap<QAction*,QWidget*>();

    mdiArea = new QMdiArea();
    setCentralWidget(mdiArea);
    resize(QDesktopWidget().availableGeometry(this).size()*0.7);  // using 70% free screen space
    QString title = MainWindow::tr("Registry editor");
    this->setWindowTitle(title);

}

/* Создает все необходимые меню и помечает их для дальнейшего перевода */
void MainWindow::CreateMenus()
{
    menuMap = new QMap<QString,QMenu*>;

    QMenu *pMenuFile = ui->menuBar->addMenu(MainWindow::tr("&File"));
    menuMap->insert("File",pMenuFile);
    pMenuFile->addAction(MainWindow::tr("&Open"),this,
                                                SLOT(ShowDialog()),
                                                QKeySequence::Open);
    QAction * m_pActionSeparator = pMenuFile->addSeparator();
    m_pActionSeparator->setVisible(true);
    pMenuFile->addAction(MainWindow::tr("E&xit"),this,SLOT(close()),
                                                   QKeySequence::Quit);
    QMenu *pMenuWindow = ui->menuBar->addMenu(MainWindow::tr("&View"));
    menuMap->insert("View",pMenuWindow);
    pMenuWindow->addAction(MainWindow::tr("&Close all windows"),this,
                                            SLOT(CloseSubWindows()),
                                            QKeySequence::Close);
    m_pActionSeparator = pMenuFile->addSeparator();
    m_pActionSeparator->setVisible(true);
    QMenu *pMenuAbout = ui->menuBar->addMenu(MainWindow::tr("&Help"));
    pMenuAbout->addAction(MainWindow::tr("&About"),this,
                                           SLOT(ShowAbout()),
                                           QKeySequence::HelpContents);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mdiArea;
    delete paths;
    delete menuMap;
    delete viewMap;
}

void MainWindow::ShowAbout()
{
    About a;
    a.exec();
}

/* Событие закрытия все окон по нажатию кнопки */
void MainWindow::CloseSubWindows()
{
    foreach (QMdiSubWindow * win, mdiArea->subWindowList())
    {
        win->close();
    }
}

/* Событие фокусировки на окно по нажатию кнопки */
void MainWindow::FocusWindow()
{
    QAction * sender = (QAction*)QObject::sender();
    QWidget * widget = (*viewMap)[sender];
    widget->setFocus();
}

/* Загружает последние ветви из файла */
void MainWindow::LoadLast(const QString& file)
{
    QFile paths(file);
    if (!paths.open(QIODevice::ReadOnly)) // Если не удалось открыть - создаем новый
    {
        paths.close();
        paths.open(QIODevice::WriteOnly | QIODevice::ReadWrite);
        QTextStream out(&paths);
        paths.seek(0);
        out << this->examplePath;
        paths.close();
        LoadLastOpened();
        return;
    }
    QTextStream in(&paths);
    QStringList * list = new QStringList();
    while (!in.atEnd())
    {
        QString line = in.readLine();
        list->append(line);
    }
    paths.close();
    this->paths = list;
    LoadLastOpened();
}

/* Загружает из paths все ранее открытые ветви. Инф-ция взята из файла */
void MainWindow::LoadLastOpened()
{
    if (!paths->empty())
    {
        QMenu * menu = (*menuMap)["File"];

        QAction * exit = menu->actions()[menu->actions().size()-1];
        menu->insertSeparator(exit);
        QAction * separ = menu->actions()[2];
        int counter = 0;
        foreach (QString path, *paths)
        {
            QAction * p = new QAction(path,menu);
            connect(p,SIGNAL(triggered()),this,SLOT(LoadFromLastMenu()));
            menu->insertAction(separ,p);
            if (++counter > 5)
                break;
        }
    }
}

/* По кнопке на ранее открытую ветвь открывает её в под-окне */
void MainWindow::LoadFromLastMenu()
{
    QAction * button = (QAction*)QObject::sender();
    LoadSubWindow(button->iconText());
}

/* Загружает новую ветвь в файл */
void MainWindow::LoadNewToFile(const QString& file, const QString& newpath)
{
    QFile to(file);
    if (to.open(QFile::Append))
    {
        QTextStream out(&to);
        out << "\n";
        out << newpath;
        to.close();
    }
}

/* Загружает информацию о под окне в меню View */
void MainWindow::LoadIntoView(const QString& path, QWidget * widget)
{
    QMenu * menu = (*menuMap)["File"];

    if (menu->actions().size()<3)
    {
        QAction * exit = menu->actions()[menu->actions().size()-1];
        menu->insertSeparator(exit);
    }
    QAction * before = menu->actions()[2];
    QAction * p = new QAction(path,menu);
    connect(p,SIGNAL(triggered()),this,SLOT(LoadFromLastMenu()));
    if (!this->paths->contains(path))
        menu->insertAction(before,p); // а также добавляет информацию в меня Файл

    menu = (*menuMap)["View"];
    QAction * last = menu->actions()[menu->actions().size()-1];
    p = new QAction(path,menu);

    connect(p,SIGNAL(triggered()),this,SLOT(FocusWindow()));
    viewMap->insert(p,widget);
    menu->insertAction(last,p);
}

/* Основная функция загрузки подокна */
void MainWindow::LoadSubWindow(const QString& path)
{
    // Если ранее не встречали такой путь - добавляем
    if (!this->paths->contains(path))
    {
        qDebug() << "Not contains";
        LoadNewToFile(this->last,path);
        paths->append(path);
    }

    QSplitter *pSplitter = new QSplitter;
    LoadIntoView(path,pSplitter); // Загружаем информацию об окне в меню View

    // Создаем нужные модели
    RegistryModel *pModel = new RegistryModel(path);
    TableModel * tModel = new TableModel();

    QTreeView *pTreeView = new QTreeView(pSplitter);
    // Параметры для drag'n'drop
    pTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    pTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    pTreeView->setDragEnabled(true);
    pTreeView->setAcceptDrops(true);
    pTreeView->setDropIndicatorShown(true);
    pTreeView->setDragDropMode(QAbstractItemView::InternalMove);
    pTreeView->setDefaultDropAction(Qt::MoveAction);
    QTableView *tView = new QTableView(pSplitter);
    // Делегат для изменения значений
    SpinBoxDelegate * spd = new SpinBoxDelegate();
    tView->setItemDelegate(spd);
    // Обработка событий клика по ветке
    // Выполняется разворачивание этой ветки на один уровень в глубину
    // А также загрузка ключей этой ветки в таблицу
    QObject::connect(
        pTreeView, SIGNAL(clicked(const QModelIndex &)),
        pModel, SLOT(myClickProcess(const QModelIndex &)));
    QObject::connect(
        pTreeView, SIGNAL(clicked(const QModelIndex &)),
        tModel, SLOT(getKeys(const QModelIndex &)));

    pTreeView->setModel(pModel);
    tView->setModel(tModel);

    QMdiSubWindow * subWindow = mdiArea->addSubWindow(pSplitter,Qt::SubWindow);
    // Опции изменения размера окна
    subWindow->setOption(QMdiSubWindow::RubberBandResize, true);
    subWindow->setOption(QMdiSubWindow::AllowOutsideAreaHorizontally, false);
    subWindow->setOption(QMdiSubWindow::AllowOutsideAreaVertically, false);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->setWindowTitle(path); // устанавливаем имя окна - имя открытого корня ветки реестра
    subWindow->show();
    mdiArea->cascadeSubWindows(); // все окна располагаем каскадом
    connect(pSplitter,SIGNAL(destroyed()),this,SLOT(RemoveFromView())); // в случае закрытия окна - удаляем запись из меню View

}

/* Удаление из меню View записи об окне */
void MainWindow::RemoveFromView()
{
    QWidget * splitter = (QWidget*)QObject::sender();
    QMap<QAction*,QWidget*>::iterator i;
    for (i = viewMap->begin(); i != viewMap->end(); ++i)
    {
        if (i.value() == splitter)
        {
            QMenu * menu = (*menuMap)["View"];
            menu->removeAction(i.key());
            viewMap->remove(i.key());
            break;
        }
    }
}

void MainWindow::ShowDialog()
{
    Dialog d;
    if (d.exec())
    {
        if (d.path == "")
            return;
         LoadSubWindow(d.path);
    }
}


