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

#ifdef Q_WS_MAEMO_5
#include <QtGui/QX11Info>
#include <X11/Xlib.h>
#endif

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

    timer = new QTimer(this);
    //Default time interval - 10m = 10 * 60 * 1000,
    //It should be configurable, and loaded on app startup
    timer->setInterval(10 * 60 * 1000);

    file = new File(CAM_DEFAULT_FILE_NAME);

    connect(timer, SIGNAL(timeout()), this, SLOT(changeUsedFile()));
    connect(cameraGroup, SIGNAL(triggered(QAction*)), this, SLOT(updateCameraDevice(QAction*)));
    connect(ui->fullscreenButton, SIGNAL(clicked()), this, SLOT(toggleFullScreen()));
    connect(ui->startrecordButton, SIGNAL(clicked()), this, SLOT(startRecording()));

    //Initialize default camera device
    setCamera(cameraDevice);

    toggleRecordButton = false;

#ifdef Q_WS_MAEMO_5
    QTimer::singleShot(700, this, SLOT(takeScreenshot()));
#endif
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
    setOutputLocation();

    camera->setViewfinder(ui->viewfinder);
    camera->setCaptureMode(QCamera::CaptureVideo);
    camera->start();
}

void Camera::changeUsedFile()
{
    qDebug() << "Changing used temp file for recording...";
    this->stop();
    file->changeFile();
    setOutputLocation();
    this->record();
    qDebug() << "Changing temp file: DONE";
}

void Camera::startRecording()
{
    if(toggleRecordButton) {
        stop();
        ui->startrecordButton->setText("Record");
        file->fileReady();
    }
    else {
        ui->startrecordButton->setText("Stop");
        record();
    }
}

/**
  Start recording
*/
void Camera::record()
{
    mediaRecorder->record();
    timer->start();
    toggleRecordButton = true;
}

/**
  Set Output Location
*/
void Camera::setOutputLocation()
{
    qDebug() << "setOutputLocation: " << QDir::toNativeSeparators(file->getActiveFile());
    mediaRecorder->setOutputLocation(QDir::toNativeSeparators(file->getActiveFile()));
}

/**
  Pause recording process
*/
void Camera::pause()
{
    timer->stop();
    toggleRecordButton = false;
    mediaRecorder->pause();
}

/**
  Stop recording
*/
void Camera::stop()
{
    timer->stop();
    toggleRecordButton = false;
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

#ifdef Q_WS_MAEMO_5
void Camera::takeScreenshot()
{
    // True takes a screenshot, false destroys it
    // See http://maemo.org/api_refs/5.0/5.0-final/hildon/hildon-Additions-to-GTK+.html#hildon-gtk-window-take-screenshot
    bool take = true;
    XEvent xev = { 0 };

    xev.xclient.type = ClientMessage;
    xev.xclient.serial = 0;
    xev.xclient.send_event = True;
    xev.xclient.display = QX11Info::display();
    xev.xclient.window = XDefaultRootWindow (xev.xclient.display);
    xev.xclient.message_type = XInternAtom (xev.xclient.display, "_HILDON_LOADING_SCREENSHOT", False);
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = take ? 0 : 1;
    xev.xclient.data.l[1] = this->winId();

    XSendEvent (xev.xclient.display,
                xev.xclient.window,
                False,
                SubstructureRedirectMask | SubstructureNotifyMask,
                &xev);

    XFlush (xev.xclient.display);
    XSync (xev.xclient.display, False);
}
#endif
