
#include <QtWidgets>
#include "worker.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

Worker::Worker(){
}

QImage Mat2QImage(cv::Mat const& src)
{
     cv::Mat temp; // make the same cv::Mat
     cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     dest.bits(); // enforce deep copy, see documentation
     return dest;
}
void Worker::doWork(){


    cvtColor( src, src_gray, CV_BGR2GRAY  );
    GaussianBlur( src_gray, src_gray, Size(3, 3), 2, 2 );
    vector<Vec3f> circles;
    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, 1, 200, 100, 0, 0 );
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
        circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
    emit resultReady(QPixmap::fromImage(Mat2QImage(src)));
    emit finished();
}
