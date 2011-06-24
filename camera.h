#ifndef CAMERA_H
#define CAMERA_H

#include <qcamera.h>
#include <qmediarecorder.h>

#include "aboutdialog.h"
#include "settings.h"

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

    void toggleFullScreen();

private:
    Ui::Camera *ui;

    QCamera *camera;
    QMediaRecorder* mediaRecorder;



};

#endif
