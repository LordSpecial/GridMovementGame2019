#include "cameraworker.h"
#include <QtConcurrent/QtConcurrent>

using namespace std;
using namespace cv;

extern void TestFunction() {
    Mat image;

//    if (cap.isOpened()) {
//        cap >> image;
//        imshow("test", image);
//        qDebug() << "we good";

//    } else {
//        qDebug() << "we're fucked";
//    }


}

void startFeed() {
    QFuture<void> future = QtConcurrent::run(TestFunction);
    future.waitForFinished();
    startFeed();
}
CameraWorker::CameraWorker()
{
//    VideoCapture cap(0);
//    namedWindow("test");
//    startFeed();

//    Mat image;
//    int i = 0;
//    for (;;) {
//i++;
//        if (cap.isOpened()) {
//            cap >> image;
////            imshow("test", image);
//            qDebug() << "we good" << i;

//        } else {
//            qDebug() << "we're fucked";
//        }
//        QThread::msleep(10);
//    };



    for (int i =0; i<=200; i++) {
        qDebug() << "RUNNING | "<<i <<" | " << i;
    }
}

void CameraWorker::killWorker()
{

    qDebug() << "Worker Being sent to the Guillotine";
    //    future.cancel();
    //    future.waitForFinished();
    qDebug() << "Worker Killed";
}

