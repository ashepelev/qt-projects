#include "mythread.h"
#include <iostream>
#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <QtWidgets>

using namespace std;
using namespace cv;


void MyObject::startWorkInAThread()
{
    WorkerThread *workerThread = new WorkerThread;
    connect(workerThread, SIGNAL(finished()),
            workerThread, SLOT(deleteLater()));
    workerThread->start();
}
