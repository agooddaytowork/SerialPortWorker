#include "validateserialportinfo.h"

validateSerialPortInfo::validateSerialPortInfo(SerialPortWorkerProperty *instantProperty, QState *parent) :
    QState(parent), currentProperty(instantProperty)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("validateSerialPortInfo Constructed"));
}

void validateSerialPortInfo::onEntry(QEvent *)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("validateSerialPortInfo Entered"));
    qApp->processEvents();
    currentProperty->deleteSerialPort();
    QSerialPortInfo SerialPortInfo(currentProperty->PortName);
    if (SerialPortInfo.isNull())
    {
        anIf(SerialPortWorkerPropertyDbgEn, anWarn("SerialPortInfo IS NULL !"));
        emit currentProperty->requestDirectTransitionForSerialPortWorkerState("requestSerialPortInfo");
    }
    else
    {
        anIf(SerialPortWorkerPropertyDbgEn, anAck("SerialPortInfo Validated !"));
        emit currentProperty->requestDirectTransitionForSerialPortWorkerState("connectSerialPort");
    }
}

void validateSerialPortInfo::onExit(QEvent *)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("Leave validateSerialPortInfo"));
}
