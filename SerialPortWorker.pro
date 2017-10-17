#-------------------------------------------------
#
# Project created by QtCreator 2017-09-18T11:28:53
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += -std=c++11 console
TARGET = execSerialPortWorker
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    SerialPortWorker/serialportworker.cpp \
    SerialPortWorker/serialportworkerproperty.cpp \
    SerialPortWorker/requestserialportinfo.cpp \
    SerialPortWorker/validateserialportinfo.cpp \
    SerialPortWorker/connectserialport.cpp \
    SerialPortWorker/writebytes.cpp \
    SerialPortWorker/readbytes.cpp \
    SerialPortWorker/wait4errorhandler.cpp \
    SerialPortWorker/directtransition4serialportworkerstate.cpp \
    binaryprotocol.cpp

HEADERS += \
        mainwindow.h \
    anlogger.h \
    commonthings.h \
    SerialPortWorker/serialportworker.h \
    SerialPortWorker/serialportworkerproperty.h \
    SerialPortWorker/requestserialportinfo.h \
    SerialPortWorker/validateserialportinfo.h \
    SerialPortWorker/connectserialport.h \
    SerialPortWorker/writebytes.h \
    SerialPortWorker/readbytes.h \
    SerialPortWorker/wait4errorhandler.h \
    SerialPortWorker/directtransition4serialportworkerstate.h \
    binaryprotocol.h

FORMS += \
        mainwindow.ui
