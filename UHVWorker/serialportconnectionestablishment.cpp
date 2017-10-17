#include "serialportconnectionestablishment.h"

SerialPortConnectionEstablishment::SerialPortConnectionEstablishment(UHVWorkerVarSet *VarSet)
    : VarSetPtr(VarSet)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("State Constructed !"));
}

void SerialPortConnectionEstablishment::onEntry(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("State Entered !"));
    qApp->processEvents();
//    VarSetPtr->deleteSerialPort();
    VarSetPtr->SerialPort = new QSerialPort();
    VarSetPtr->configSerialPort();
    QObject::connect(VarSetPtr->SerialPort, &QSerialPort::errorOccurred, VarSetPtr, &UHVWorkerVarSet::SerialPortErrorOccurred);
    if (VarSetPtr->SerialPort->open(QIODevice::ReadWrite))
    {
        anIf(UHVWorkerVarSetDbgEn, anAck("SerialPort Opened !"));
        emit VarSetPtr->Out(QVariant::fromValue(UHVWorkerVarSet::SerialPortConnected));
        emit VarSetPtr->DirectStateTransitionRequest("SolitaryMessageTransmission");
    }
    else
    {
        anIf(UHVWorkerVarSetDbgEn, anWarn("Failed To Open SerialPort !"));
    }
}

void SerialPortConnectionEstablishment::onExit(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("Leave State !"));
}
