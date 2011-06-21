#include "camera.h"
#include "ui_camera.h"

#include <qmediaservice.h>
#include <qmediarecorder.h>
#include <qcamera.h>
#include <qcameraviewfinder.h>

#include <qmessagebox.h>
#include <qpalette.h>

#include <QtGui>

#if (defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6)) && QT_VERSION >= 0x040700
#define HAVE_CAMERA_BUTTONS
#endif

Camera::Camera(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Camera),
    camera(0),
    mediaRecorder(0)
{
    ui->setupUi(this);

    //Camera devices:
    QByteArray cameraDevice;

    foreach(const QByteArray &deviceName, QCamera::availableDevices()) {
        if (cameraDevice.isEmpty()) {
            cameraDevice = deviceName;
        }
    }

    setCamera(cameraDevice);
}

Camera::~Camera()
{
    delete mediaRecorder;
    delete camera;
}

void Camera::setCamera(const QByteArray &cameraDevice)
{
    delete mediaRecorder;
    delete camera;

    camera = new QCamera;
    mediaRecorder = new QMediaRecorder(camera);
    camera->setViewfinder(ui->viewfinder);

    camera->setCaptureMode(QCamera::CaptureVideo);
    camera->start();
}

void Camera::record()
{
    mediaRecorder->record();
}

void Camera::pause()
{
    mediaRecorder->pause();
}

void Camera::stop()
{
    mediaRecorder->stop();
}

void Camera::startCamera()
{
    camera->start();
}

void Camera::stopCamera()
{
    camera->stop();
}
