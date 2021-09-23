#-------------------------------------------------
#
# Project created by QtCreator 2019-08-28T17:52:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UserInterface
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        detection.cpp \
        game.cpp \
        grid.cpp \
        main.cpp \
        mainwindow.cpp \
        pickupitem.cpp \
        player.cpp

HEADERS += \
        game.h \
        grid.h \
        lineDetection.h \
        lineIntersects.h \
        mainwindow.h \
        maps.h \
        pickupitem.h \
        player.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += D:\opencv\build\install\include\

LIBS += D:\opencv\build\lib\libopencv_core411.dll.a
LIBS += D:\opencv\build\lib\libopencv_highgui411.dll.a
LIBS += D:\opencv\build\lib\libopencv_imgcodecs411.dll.a
LIBS += D:\opencv\build\lib\libopencv_imgproc411.dll.a
LIBS += D:\opencv\build\lib\libopencv_features2d411.dll.a
LIBS += D:\opencv\build\lib\libopencv_calib3d411.dll.a
LIBS += D:\opencv\build\lib\libopencv_videoio411.dll.a

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
