#ifndef CAMERAWORKER_H
#define CAMERAWORKER_H
#include "opencv2/opencv.hpp"

#include <QThread>
#include <QtConcurrent/QtConcurrent>


class CameraWorker:public QObject
{
    Q_OBJECT
public:
    CameraWorker();
    void killWorker();
};

#endif // CAMERAWORKER_H
