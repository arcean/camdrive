#ifndef CAMERA_H
#define CAMERA_H

#include <QMainWindow>
#include <QDir>
#include <QTimer>

#include <qcamera.h>
#include <qmediarecorder.h>

#include "aboutdialog.h"
#include "settings.h"
#include "file.h"

#define CAM_DEFAULT_FILE_NAME "camdrive_file"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Camera;
}

class QCameraViewfinder;

QT_END_NAMESPACE

QT_USE_NAMESPACE

class Camera : public QMainWindow
{
    Q_OBJECT
public:
    Settings *settings;

    Camera(QWidget *parent = 0);
    ~Camera();

private slots:
    void setCamera(const QByteArray &cameraDevice);

    void startCamera();
    void stopCamera();

    void record();
    void pause();
    void stop();

    void showAboutDialog();
    void showSettings();

    void updateCameraDevice(QAction *action);
    void startRecording();
    void setOutputLocation();
    void coverClose();
    void changeUsedFile();
    void toggleFullScreen();
#ifdef Q_WS_MAEMO_5
    void takeScreenshot();
#endif

private:
    Ui::Camera *ui;

    QCamera *camera;
    QMediaRecorder* mediaRecorder;
    QTimer *timer;
    File *file;

    //Should we start recording or just stop?
    bool toggleRecordButton;



};

#endif
