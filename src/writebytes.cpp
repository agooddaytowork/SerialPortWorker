#include "writebytes.h"

writeBytes::writeBytes(SerialPortWorkerProperty *instantProperty, quint16 WriteTimeOutInMilisecond, QState *parent)
    : QState(parent), currentProperty(instantProperty), TimeOut4WriteInMilisecond(WriteTimeOutInMilisecond)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("writeBytes Constructed"));
}

void writeBytes::onEntry(QEvent *)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("writeBytes Entered"));
    qApp->processEvents();
    currentProperty->clearEmptyList();
    if (currentProperty->prioritizedBuffer.size())
    {
        currentProperty->currentGlobalSignal = currentProperty->prioritizedBuffer.last().first();
        currentProperty->SerialPort->write(currentProperty->currentGlobalSignal.Data.value<SerialPortWorkerProperty::DataMessage>().first);
        if (currentProperty->SerialPort->waitForBytesWritten(TimeOut4WriteInMilisecond))
        {
            anIf(SerialPortWorkerPropertyDbgEn, anAck("Bytes Written !"));
            currentProperty->currentGlobalSignal.Type = QVariant::fromValue(SerialPortWorkerProperty::BytesWritten);
        }
        else
        {
            anIf(SerialPortWorkerPropertyDbgEn, anWarn("Bytes Written Timed Out !"));
            currentProperty->currentGlobalSignal.Type = QVariant::fromValue(SerialPortWorkerProperty::BytesWrittenTimedOut);
        }
        currentProperty->prioritizedBuffer.last().removeFirst();
        emit currentProperty->Out(currentProperty->currentGlobalSignal);
        emit currentProperty->requestDirectTransitionForSerialPortWorkerState("readBytes");
    }
    else
    {
        anIf(SerialPortWorkerPropertyDbgEn, anInfo("Buffer Is Empty !"));
    }
}

void writeBytes::onExit(QEvent *)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("Leave writeBytes"));
}
