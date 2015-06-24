#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;


class MyThread : public QThread
{
    Q_OBJECT
public:
    Mat src;
    string filepath;
    MyThread();
    void run();
};

#endif // MYTHREAD_H
