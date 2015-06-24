#ifndef WORKER_H
#define WORKER_H

#include <QtWidgets>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

using namespace std;

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker();
    Mat src, src_gray;
signals:
    void finished();
    void resultReady(QPixmap chResult);
public slots:
    void doWork();

private:
    string filepath;

};

#endif // WORKER_H
