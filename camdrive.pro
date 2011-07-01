QT += gui maemo5
TEMPLATE = app
TARGET = camdrive

INCLUDEPATH+=../../src/multimedia \
             ../../src/multimedia/video

maemo5 {
    CONFIG += mobility11
} else {
    CONFIG += mobility
}

MOBILITY = multimedia

HEADERS = \
    camera.h \
    aboutdialog.h \
    settings.h \
    file.h

SOURCES = \
    main.cpp \
    camera.cpp \
    aboutdialog.cpp \
    settings.cpp \
    file.cpp

FORMS += \
    camera.ui \
    aboutdialog.ui \
    settings.ui

icons.path = /opt/camdrive/share/pixmaps/
icons.files = data/camera.png

maemo_specific.path = /usr/share/dbus-1/services/
maemo_specific.files = org.maemo.camdrive.service

INSTALLS += icons maemo_specific

symbian {
    include(camerakeyevent_symbian/camerakeyevent_symbian.pri)
    TARGET.CAPABILITY += UserEnvironment WriteUserData ReadUserData
    TARGET.EPOCHEAPSIZE = 0x20000 0x3000000
    LIBS += -lavkon -leiksrv -lcone -leikcore
}

OTHER_FILES += \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog \
    camera.png

include(deployment.pri)
qtcAddDeployment()

maemo5 {
    target.path = /opt/camdrive/bin
    INSTALLS += target
}
