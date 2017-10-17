#include "serialportinforequest.h"

SerialPortInfoRequest::SerialPortInfoRequest(UHVWorkerVarSet *VarSet, quint32 TimerIntervalInMilisecond) :
    TimerIntervalMSecs(TimerIntervalInMilisecond)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("State Constructed !"));
    if (TimerIntervalInMilisecond > 0)
    {
        timer.setParent(this);
        timer.setInterval(TimerIntervalInMilisecond);
        QObject::connect(&timer, &QTimer::timeout
                        , this
                        , [VarSet](){
                                anIf(UHVWorkerVarSetDbgEn, anAck("Emit requestPortName !"));
                                emit VarSet->Out(QVariant::fromValue(UHVWorkerVarSet::requestPortName));
                          }
                        , UHVWorkerVarSet::uniqueQtConnectionType);
    }
}

void SerialPortInfoRequest::onEntry(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("State Entered !"));
    if (TimerIntervalMSecs > 0)
        timer.start();
}

void SerialPortInfoRequest::onExit(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("Leave State !"));
    if (TimerIntervalMSecs > 0)
        timer.stop();
}
