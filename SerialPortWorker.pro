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
    anLogger/src/anlogger.cpp \
    BinaryProtocol/src/binaryprotocol.cpp \
    shared/commonthings.cpp \
    src/connectserialport.cpp \
    src/directtransition4serialportworkerstate.cpp \
    src/readbytes.cpp \
    src/requestserialportinfo.cpp \
    src/serialportworker.cpp \
    src/serialportworkerproperty.cpp \
    src/validateserialportinfo.cpp \
    src/wait4errorhandler4serialportworker.cpp \
    src/writebytes.cpp

HEADERS += \
        mainwindow.h \
    anLogger/src/anlogger.h \
    BinaryProtocol/src/binaryprotocol.h \
    shared/commonthings.h \
    src/connectserialport.h \
    src/directtransition4serialportworkerstate.h \
    src/readbytes.h \
    src/requestserialportinfo.h \
    src/serialportworker.h \
    src/serialportworkerproperty.h \
    src/validateserialportinfo.h \
    src/wait4errorhandler4serialportworker.h \
    src/writebytes.h

FORMS += \
        mainwindow.ui
