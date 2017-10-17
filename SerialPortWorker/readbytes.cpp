#include "readbytes.h"

readBytes::readBytes(SerialPortWorkerProperty *instantProperty, quint16 ReadTimeOutInMilisecond, QState *parent)
    : QState(parent), currentProperty(instantProperty), TimeOut4ReadInMilisecond(ReadTimeOutInMilisecond)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("readBytes Constructed"));
}

void readBytes::onEntry(QEvent *)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("readBytes Entered"));
    qApp->processEvents();
    if (currentProperty->SerialPort->waitForReadyRead(TimeOut4ReadInMilisecond*10))
    {
        QByteArray tmpRead(currentProperty->SerialPort->readAll());
        while (currentProperty->SerialPort->waitForReadyRead(TimeOut4ReadInMilisecond))
        {
            tmpRead+=currentProperty->SerialPort->readAll();
        }
        anIf(SerialPortWorkerPropertyDbgEn, anAck("Message Read !"));
        SerialPortWorkerProperty::DataMessage newGlobalSignalData;
        newGlobalSignalData.first = tmpRead;
        newGlobalSignalData.second = QDateTime::currentDateTime().toString(QStringLiteral("yyyy-MM-dd hh:mm:ss"));
        currentProperty->currentGlobalSignal.Type = QVariant::fromValue(SerialPortWorkerProperty::replyAGlobalSignal);
        currentProperty->currentGlobalSignal.Data = QVariant::fromValue(newGlobalSignalData);
    }
    else
    {
        anIf(SerialPortWorkerPropertyDbgEn, anWarn("Ready Read Timed Out !"));
        currentProperty->currentGlobalSignal.Type = QVariant::fromValue(SerialPortWorkerProperty::ReadyReadTimedOut);
    }
    emit currentProperty->Out(currentProperty->currentGlobalSignal);
    emit currentProperty->requestDirectTransitionForSerialPortWorkerState("writeBytes");
}

void readBytes::onExit(QEvent *)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("Leave readBytes"));
}

