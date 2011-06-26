#include <QtGui>
#include <QMaemo5InformationBox>
#include <QTextStream>

#include <qmediaservice.h>
#include <qmediarecorder.h>
#include <qcamera.h>
#include <qcameraviewfinder.h>

#include "camera.h"
#include "ui_camera.h"
#include "aboutdialog.h"
#include "settings.h"

Camera::Camera(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Camera),
    camera(0),
    mediaRecorder(0)
{
    ui->setupUi(this);

    ui->menuFile->addAction("About", this, SLOT(showAboutDialog()));
    ui->menuFile->addAction("Settings", this, SLOT(showSettings()));

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
        ui->menuDevices->addAction(cameraGroupAction);
    }

    connect(cameraGroup, SIGNAL(triggered(QAction*)), this, SLOT(updateCameraDevice(QAction*)));
    connect(ui->fullscreenButton, SIGNAL(clicked()), this, SLOT(toggleFullScreen()));
    connect(ui->startrecordButton, SIGNAL(clicked()), this, SLOT(coverClose()));

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
  Set Output Location
*/
void Camera::setOutputLocation(const QUrl &location)
{
    mediaRecorder->setOutputLocation(location);
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

void Camera::showSettings()
{
    delete settings;
    settings = new Settings(this);
    settings->show();

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
/**
  Cover clos nitification
*/
void Camera::coverClose()
{
    QFile *state = new QFile("/sys/devices/platform/gpio-switch/cam_shutter/state");
    if(state->open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream *in = new QTextStream(state);
        QString linia = in->readLine();
        if(linia.contains("closed"))
        {
            QMaemo5InformationBox *box = new QMaemo5InformationBox(this);
            box->information(this, "Open Lens Cover", QMaemo5InformationBox::DefaultTimeout);
        }
        state->close();
    }
}
