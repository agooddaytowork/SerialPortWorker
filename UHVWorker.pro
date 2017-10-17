#-------------------------------------------------
#
# Project created by QtCreator 2017-09-10T22:54:03
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += -std=c++11 console
TARGET = UHVWorkerExec
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
    UHVWorker/errorannouncement.cpp \
    UHVWorker/messagereceiveandemitout.cpp \
    UHVWorker/serialportconnectionestablishment.cpp \
    UHVWorker/serialportinforequest.cpp \
    UHVWorker/serialportinfovalidation.cpp \
    UHVWorker/solitarymessagetransmission.cpp \
    UHVWorker/uhvworker.cpp \
    UHVWorker/uhvworkerdirectstatetransition.cpp \
    UHVWorker/uhvworkervarset.cpp

HEADERS += \
        mainwindow.h \
    anlogger.h \
    UHVWorker/errorannouncement.h \
    UHVWorker/messagereceiveandemitout.h \
    UHVWorker/serialportconnectionestablishment.h \
    UHVWorker/serialportinforequest.h \
    UHVWorker/serialportinfovalidation.h \
    UHVWorker/solitarymessagetransmission.h \
    UHVWorker/uhvworker.h \
    UHVWorker/uhvworkerdirectstatetransition.h \
    UHVWorker/uhvworkervarset.h

FORMS += \
        mainwindow.ui
