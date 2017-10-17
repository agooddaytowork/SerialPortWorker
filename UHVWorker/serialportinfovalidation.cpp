#include "serialportinfovalidation.h"

SerialPortInfoValidation::SerialPortInfoValidation(UHVWorkerVarSet *VarSet)
    : VarSetPtr(VarSet)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("State Constructed !"));
}

void SerialPortInfoValidation::onEntry(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("State Entered !"));
    qApp->processEvents();
    VarSetPtr->deleteSerialPort();
    QSerialPortInfo SerialPortInfo(VarSetPtr->PortName);
    if (SerialPortInfo.isNull())
    {
        anIf(UHVWorkerVarSetDbgEn, anWarn("SerialPortInfo IS NULL !"));
        emit VarSetPtr->DirectStateTransitionRequest("SerialPortInfoRequest");
    }
    else
    {
        anIf(UHVWorkerVarSetDbgEn, anAck("SerialPortInfo Validated !"));
        emit VarSetPtr->DirectStateTransitionRequest("SerialPortConnectionEstablishment");
    }
}

void SerialPortInfoValidation::onExit(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("Leave State !"));
}
