TEMPLATE = app
TARGET = camera

INCLUDEPATH+=../../src/multimedia \
             ../../src/multimedia/video

include(../mobility_examples.pri)

maemo5 {
    CONFIG += mobility11
} else {
    CONFIG += mobility
}

MOBILITY = multimedia

HEADERS = \
    camera.h \
    aboutdialog.h

SOURCES = \
    main.cpp \
    camera.cpp \
    aboutdialog.cpp

FORMS += \
    camera.ui \
    aboutdialog.ui

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
    qtc_packaging/debian_fremantle/changelog

maemo5 {
    target.path = /opt/camera/bin
    INSTALLS += target
}
