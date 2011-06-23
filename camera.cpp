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
    QActionGroup *cameraGroup = new QActionGroup(this);

    cameraGroup->setExclusive(true);

    //Simply, adds camera devices to mbar
    foreach(const QByteArray &deviceName, QCamera::availableDevices())
    {
        QString description = camera->deviceDescription(deviceName);
        QAction *cameraGroupAction = new QAction(description, cameraGroup);

        cameraGroupAction->setCheckable(true);
        cameraGroupAction->setData(QVariant(deviceName));

        if (cameraDevice.isEmpty())
        {
            cameraDevice = deviceName;
            cameraGroupAction->setChecked(true);
        }
        mbar->addAction(cameraGroupAction);
    }

    connect(cameraGroup, SIGNAL(triggered(QAction*)), this, SLOT(updateCameraDevice(QAction*)));
    connect(ui->fullscreenButton, SIGNAL(clicked()), this, SLOT(toggleFullScreen()));

    //Initialize default camera device
    setCamera(cameraDevice);
}

Camera::~Camera()
{
    delete mediaRecorder;
    delete camera;
}

/**
  Initialize camera & viewfinder
*/
void Camera::setCamera(const QByteArray &cameraDevice)
{
    delete mediaRecorder;
    delete camera;

    if (cameraDevice.isEmpty())
        camera = new QCamera;
    else
        camera = new QCamera(cameraDevice);

    mediaRecorder = new QMediaRecorder(camera);

    camera->setViewfinder(ui->viewfinder);
    camera->setCaptureMode(QCamera::CaptureVideo);
    camera->start();
}

/**
  Start recording
*/
void Camera::record()
{
    mediaRecorder->record();
}

/**
  Pause recording process
*/
void Camera::pause()
{
    mediaRecorder->pause();
}

/**
  Stop recording
*/
void Camera::stop()
{
    mediaRecorder->stop();
}

/**
  Activate camera, use after initialization
*/
void Camera::startCamera()
{
    camera->start();
}

/**
  Deactivate camera
*/
void Camera::stopCamera()
{
    camera->stop();
}

void Camera::showAboutDialog()
{
    AboutDialog dialog;
    dialog.exec();
}

/**
  Action triggered by mbar's cameraGroup
*/
void Camera::updateCameraDevice(QAction *action)
{
    setCamera(action->data().toByteArray());
}

/**
  Turns fullscreen mode on and off.
*/
void Camera::toggleFullScreen()
{
    bool isFullScreen = windowState() & Qt::WindowFullScreen;

    if (isFullScreen)
        showNormal();
    else
        showFullScreen();
}
