#ifndef CANNYTHREAD_H
#define CANNYTHREAD_H

#include <QThread>
#include <QImage>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class Worker : public QObject
{
    Q_OBJECT

public:
    Worker(QString filename, double alphaOrig, int threshold = 0);
    ~Worker();
    cv::Mat image, grayScale;
    QImage result;
    int thresholdValue;
    double alphaOrig;
signals:
    void resultReady();
    void finished();

public slots:
    void onDoWork();

private:

};

#endif // CANNYTHREAD_H
