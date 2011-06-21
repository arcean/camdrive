#ifndef CAMERA_H
#define CAMERA_H

#include <qcamera.h>
#include <qmediarecorder.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Camera;
}

class QCameraViewfinder;

QT_END_NAMESPACE

#include <QMainWindow>
#include <QDir>

QT_USE_NAMESPACE

class Camera : public QMainWindow
{
    Q_OBJECT
public:
    Camera(QWidget *parent = 0);
    ~Camera();

private slots:
    void setCamera(const QByteArray &cameraDevice);

    void startCamera();
    void stopCamera();

    void record();
    void pause();
    void stop();

private:
    Ui::Camera *ui;

    QCamera *camera;
    QMediaRecorder* mediaRecorder;
};

#endif
