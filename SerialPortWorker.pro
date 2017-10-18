#-------------------------------------------------
#
# Project created by QtCreator 2017-10-13T13:24:08
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += -std=c++11 console
TARGET = SerialPortWorker
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
        WindowProtocol/src/windowprotocol.cpp \
        shared/abstractstatemachinebasis.cpp \
        shared/commonthings.cpp \
        shared/directtransition.cpp \
        src/serialportworker.cpp \
    src/serialportworkerbasis.cpp \
    src/uninitiatedserialportworker.cpp \
    src/idleserialportworker.cpp \
    src/runningserialportworker.cpp \
    src/readbytesserialportworker.cpp \
    src/errorserialportworker.cpp

HEADERS += \
        mainwindow.h \
        anLogger/src/anlogger.h \
        BinaryProtocol/src/binaryprotocol.h \
        WindowProtocol/src/windowprotocol.h \
        shared/abstractstatemachinebasis.h \
        shared/commonthings.h \
        shared/directtransition.h \
        src/serialportworker.h \
    src/serialportworkerbasis.h \
    src/uninitiatedserialportworker.h \
    src/idleserialportworker.h \
    src/runningserialportworker.h \
    src/readbytesserialportworker.h \
    src/errorserialportworker.h

FORMS += \
        mainwindow.ui
