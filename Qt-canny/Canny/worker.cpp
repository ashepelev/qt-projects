#include "worker.h"

#include <QMessageBox>
#include <QDebug>
//#include <QtTest/QTest>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

Worker::Worker(QString filename, double alphaOrig, int threshold)
{
    // Считываем изображение
    this->image = cv::imread(filename.toStdString());
    // Устанавливаем параметры
    this->thresholdValue = threshold;
    this->alphaOrig = alphaOrig;
}

Worker::~Worker()
{

}

void Worker::onDoWork()
{
    // Переводим оригинал в серое
    cv::cvtColor(this->image,this->grayScale,CV_RGB2GRAY);
    cv::Mat edges, thres, blended, grayToColor;
    // Если порог не установлен вычисляем по Отцу
    if (this->thresholdValue == 0)
    {
        double threshold = cv::threshold(this->grayScale,thres,0,255,cv::THRESH_OTSU);
        this->thresholdValue = (int)threshold;
    }
    // Ищем края на сером изображение по заданному порогу
    // Параметры максимального порога и apertureSize заданы по рекомандациям автора алгоритма
    cv::Canny(this->grayScale,edges,this->thresholdValue,this->thresholdValue*3,3);
    // Устанавливаем доли цветного и серого изображение перед совмещением
    double alpha = this->alphaOrig;
    double beta = 1.0 - alpha;
    // Переводим серое в цветное
    cv::cvtColor(edges,grayToColor,cv::COLOR_GRAY2BGR);
    // Совмещаем
    cv::addWeighted(this->image,alpha,grayToColor,beta,0.0,blended);
    cv::Mat result(blended.cols,blended.rows,blended.type());
    // Переводим в RGB
    cv::cvtColor(blended,result,CV_BGR2RGB);
    // Переводим в QImage
    QImage dest((const uchar *) result.data, result.cols, result.rows, result.step, QImage::Format_RGB888);
    // Глубокое копирование
    dest.bits();
    this->result = dest;
    // Спим для проверки блокировки интерфейса
    QThread::msleep(2000);

    emit finished();
    emit resultReady();
}
