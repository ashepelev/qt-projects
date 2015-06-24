#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "worker.h"
#include "about.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QScrollArea>
#include <QDebug>
#include <QToolBar>
#include <QSpinBox>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Canny method");
    createMenus();
}

// Создаем меню
void MainWindow::createMenus()
{
    // Создаем меню-бар
    QMenu *pMenuFile = ui->menuBar->addMenu("&File");
    QAction * open = pMenuFile->addAction("&Open",this, SLOT(openFile()),
                                                QKeySequence::Open);
    this->open = open;
    QAction * m_pActionSeparator = pMenuFile->addSeparator();
    m_pActionSeparator->setVisible(true);
    QAction * exit = pMenuFile->addAction("E&xit",this,SLOT(close()),
                                                   QKeySequence::Quit);
    this->exit = exit;
    QMenu *pMenuHelp = ui->menuBar->addMenu("&Help");
    pMenuHelp->addAction("About",this,SLOT(launchAbout()),QKeySequence::HelpContents);
    // Создаем кнопки и виджеты ввода для тулбара
    QPushButton * launch = new QPushButton("Launch",this); // Кнопка старта
    this->launch = launch;
    connect(this->launch,SIGNAL(clicked()),this,SLOT(onHandleStart())); // Связываем с её кликом блокировку интерфейса
    this->launch->setEnabled(false);
    connect(launch,SIGNAL(clicked()),this,SLOT(Launch()));
    QLabel * label = new QLabel(); // Label на котором выводить будем
    this->label = label;
    QScrollArea * scrollArea = new QScrollArea(); // ScrollArea на которую поместим label
    this->scrollArea = scrollArea;
    scrollArea->setWidget(label);
    this->setCentralWidget(scrollArea);
    QLabel * thres = new QLabel();
    thres->setText("Threshold value: ");
    thres->adjustSize();
    QToolBar * toolbar = new QToolBar("Toolbar");
    addToolBar(toolbar);
    QSpinBox * spinbox = new QSpinBox(); // Spinbox для задания порога бинаризации
    this->spinbox = spinbox;
    spinbox->setMinimum(0);
    spinbox->setMaximum(255);
    spinbox->setValue(0);
    QDoubleSpinBox * doubleSpinBox = new QDoubleSpinBox(); // DoubleSpinBox для задания доли цвета
    doubleSpinBox->setMaximum(1.0);
    doubleSpinBox->setMinimum(0.0);
    doubleSpinBox->setValue(0.5);
    doubleSpinBox->setSingleStep(0.05);
    this->doubleSpinBox = doubleSpinBox;
    QLabel * alphaVal = new QLabel();
    alphaVal->setText(" Color share: ");
    alphaVal->adjustSize();
    // Добавляем виджеты на тулбар
    toolbar->addWidget(thres);
    toolbar->addWidget(spinbox);
    toolbar->addWidget(alphaVal);
    toolbar->addWidget(doubleSpinBox);
    toolbar->addWidget(launch);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete open;
    delete exit;
    delete label;
    delete spinbox;
    delete doubleSpinBox;
}

void MainWindow::openFile()
{
    // Приоткрытиии указываем каталог "Изображения" текущего пользователя
    // И ставим фильтр на png, jpg, bmp
    QFileDialog * fd = new QFileDialog(this,"Choose Image",QDir::homePath()+"\\Pictures","Image File (*.png *.jpeg *.jpg *.bmp)");
    fd->exec();
    // Если никакой файл не выбрали
    if (fd->selectedFiles().isEmpty()) {
        return;
    }
    QString fileName = fd->selectedFiles().first();
    QImage image(fileName);
    // Если выбранный файл не изображение
    if (image.isNull())
    {
        QMessageBox mb;
        mb.setText("Wrong image");
        mb.exec();
        return;
    }
    // Помещаем изображение
    this->label->setScaledContents(true);
    this->label->setPixmap(QPixmap::fromImage(image));
    this->label->adjustSize();
    this->label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    this->filename = fileName;
    // Изменяем размеры основного окна под изображение
    myResize(image.width(),image.height());
    this->launch->setEnabled(true); // Делаем доступной кнопку старта
    this->spinbox->setValue(0);
    this->doubleSpinBox->setValue(0.5);
}

// Функция изменения размера основанная на разнице
// Основного окна и центрального виджета
void MainWindow::myResize(int w, int h)
{
  //  if (!this->isVisible())
  //      this->update();
    int dx = this->width() - this->scrollArea->width();
    int dy = this->height() - this->scrollArea->height();
    this->resize(w + dx + 10, h + dy + 10);
}

void MainWindow::Launch()
{
    // Считываем данные с input-виджетов и создаем работника
    double alpha = this->doubleSpinBox->value();
    if (this->spinbox->value() != 0)
        worker = new Worker(this->filename,alpha,this->spinbox->value());
    else
        worker = new Worker(this->filename,alpha);
    this->workThread = new QThread();
    worker->moveToThread(this->workThread); // Помещаем его в поток
    // Связываем сигналы старта и завершения с обработчиками
    connect(this->workThread, SIGNAL(started()), worker, SLOT(onDoWork()), Qt::DirectConnection);
    connect(this->workThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(worker, SIGNAL(finished()),this, SLOT(onHandleFinish()));
    connect(worker, SIGNAL(resultReady()), this, SLOT(onHandleResultReady()));
    this->workThread->start();

 //   this->workThread->usleep(20000);
}

// Обработчик старта
// Блокирует интерфейс
void MainWindow::onHandleStart()
{
    ui->statusBar->showMessage("Working...");
    QApplication::processEvents();
    this->launch->setEnabled(false);
    this->open->setEnabled(false);
    this->exit->setEnabled(false);
    this->spinbox->setEnabled(false);
    this->doubleSpinBox->setEnabled(false);
    ui->menuBar->setEnabled(false);
    ui->mainToolBar->setEnabled(false);
  //  this->setEnabled(false);
}

// Обработчик конца
// Разблокирует интерфейс
void MainWindow::onHandleFinish()
{
    QApplication::processEvents();
    this->launch->setEnabled(true);
    this->open->setEnabled(true);
    this->exit->setEnabled(true);
    this->spinbox->setEnabled(true);
    this->doubleSpinBox->setEnabled(true);
    ui->menuBar->setEnabled(true);
    ui->mainToolBar->setEnabled(true);
    ui->statusBar->showMessage("Complete");
}

// Выводит готовый результат на экран
void MainWindow::onHandleResultReady()
{
    // Получаем объект по отправителю
    Worker * sender = (Worker*)QObject::sender();
    // Копируем изображение
    QImage canny = sender->result.copy();
    // Устанавливаем изображение
    this->label->setPixmap(QPixmap::fromImage(canny));
    // Получаем вычисленное (по Отцу) значение порога
    this->spinbox->setValue(sender->thresholdValue);
    // Завершаем поток и удаляем связанные переменные
    this->workThread->terminate();
    delete this->worker;
    delete this->workThread;
    QApplication::processEvents();
    ui->statusBar->showMessage("Ready");
}

// Запускает о программе
void MainWindow::launchAbout()
{
    About a;
    a.exec();
}
