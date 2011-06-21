#include <QtGui>

#include <qmediaservice.h>
#include <qmediarecorder.h>
#include <qcamera.h>
#include <qcameraviewfinder.h>

#include "camera.h"
#include "ui_camera.h"
#include "aboutdialog.h"

Camera::Camera(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Camera),
    camera(0),
    mediaRecorder(0)
{
    ui->setupUi(this);

    // hmm, we want a menubar:
    QMenuBar *mbar = new QMenuBar(this);
    mbar->addAction("About", this, SLOT(showAboutDialog()));

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

void Camera::showAboutDialog()
{
    AboutDialog dialog;
    dialog.exec();
}
