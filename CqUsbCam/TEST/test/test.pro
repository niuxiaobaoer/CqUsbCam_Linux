#-------------------------------------------------
#
# Project created by QtCreator 2017-10-13T23:43:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


LIBS += -L\
    /home/nxb/Desktop/USBCamera/USB30/FX3_SDK_Linux/FX3_SDK_Linux_v1.3.3/cyusb_linux_1.0.4/lib \
    -lcyusb \

LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lusb-1.0

LIBS += -L/home/nxb/Desktop/USBCamera/USB30/MyLinuxUsbSdkDemo/CqUsbCamAPI_general_cmake/build \
    -lCqUsbCam
