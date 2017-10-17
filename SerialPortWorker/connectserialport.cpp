#include "connectserialport.h"

connectSerialPort::connectSerialPort(SerialPortWorkerProperty *instantProperty, QState *parent) :
    QState(parent), currentProperty(instantProperty)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("connectSerialPort Constructed"));
}

void connectSerialPort::onEntry(QEvent *)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("connectSerialPort Entered"));
    qApp->processEvents();
    currentProperty->deleteSerialPort();
    currentProperty->SerialPort = new QSerialPort();
    currentProperty->configSerialPort();
    QObject::connect(currentProperty->SerialPort, &QSerialPort::errorOccurred,
                     currentProperty, &SerialPortWorkerProperty::SerialPortErrorOccurred);
    if (currentProperty->SerialPort->open(QIODevice::ReadWrite))
    {
        anIf(SerialPortWorkerPropertyDbgEn, anAck("SerialPort Opened !"));
        GlobalSignal notifySerialPortConnected;
        notifySerialPortConnected.Type = QVariant::fromValue(SerialPortWorkerProperty::SerialPortConnected);
        emit currentProperty->Out(notifySerialPortConnected);
        emit currentProperty->requestDirectTransitionForSerialPortWorkerState("writeBytes");
    }
    else
    {
        anIf(SerialPortWorkerPropertyDbgEn, anWarn("Failed To Open SerialPort !"));
    }
}

void connectSerialPort::onExit(QEvent *)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("Leave connectSerialPort"));
}
