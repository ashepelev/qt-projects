#include <QtWidgets>
#include "mainwindow.h"
#include "worker.h"
#include "ui_mainwindow.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   ui-> label = new QLabel;
     ui->label->setBackgroundRole(QPalette::Base);
    ui-> label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui-> label->setScaledContents(true);

    ui-> scrollArea = new QScrollArea;
    ui-> scrollArea->setBackgroundRole(QPalette::Dark);
    ui-> scrollArea->setWidget(ui->label);
    setCentralWidget(ui->scrollArea);
}

MainWindow::~MainWindow()
{
    delete ui;
}
bool MainWindow::loadFile(const QString &fileName)
{
    QImage image(fileName);
    if (image.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        setWindowFilePath(QString());
        ui->label->setPixmap(QPixmap());
        ui->label->adjustSize();
        return false;
    }
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->label->adjustSize();
    setWindowFilePath(fileName);
    this->filepath=fileName.toUtf8().constData();
    return true;
}



void MainWindow::on_HoughCircle_triggered()
{
    w=new Worker();
    w->src = imread(this->filepath, 1 );

    w->moveToThread(&t);
    connect(&t, SIGNAL(started()), w, SLOT(doWork()), Qt::DirectConnection);
    connect(&t, &QThread::finished,w, &QObject::deleteLater);
    //connect(w, SIGNAL(finished()), &t, SLOT(quit()), Qt::DirectConnection);
    connect(w, &Worker::finished,this, &MainWindow::onHandleFinish);
    connect(w, &Worker::resultReady,this, &MainWindow::onHandleResultReady);

    t.start();
    //qDebug() << "start";
    //t.wait();
    //qDebug() << "ende";
    //ui->label->setPixmap(QPixmap::fromImage(Mat2QImage(w->src)));

}
void MainWindow::on_m_pActionStart_triggered()
{
    ui->statusBar->showMessage("Working...");
}
void MainWindow::onHandleFinish()
{

    ui->statusBar->showMessage("Complete...");
}
void MainWindow::onHandleResultReady(QPixmap chResult)
{
    ui->label->setPixmap(chResult);
}
void MainWindow::on_action_triggered()
{
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QFileDialog dialog(this, tr("Open File"),
                       picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}
