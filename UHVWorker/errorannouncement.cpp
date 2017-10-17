#include "errorannouncement.h"

ErrorAnnouncement::ErrorAnnouncement(UHVWorkerVarSet *VarSet, quint32 TimerIntervalInMilisecond)
    : VarSetPtr(VarSet), TimerIntervalMSecs(TimerIntervalInMilisecond)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("State Constructed !"));
    if (TimerIntervalInMilisecond > 0)
    {
        timer.setParent(this);
        timer.setInterval(TimerIntervalInMilisecond);
        QObject::connect(&timer, &QTimer::timeout
                        , this
                        , [VarSet](){
                                anIf(UHVWorkerVarSetDbgEn,
                                     anError("Emit <currentUHVWorkerVarSet>->Error!");
                                      anInfo("ErrorType : " +
                                               QString(UHVWorkerVarSet::ErrorMetaEnum.valueToKey(static_cast<int>(VarSet->ErrorType))));
                                      anInfo("ErrorInfo : " + VarSet->ErrorInfo);
                                     );
                                emit VarSet->Out(QVariant::fromValue(VarSet->ErrorType),
                                                   QVariant::fromValue(VarSet->ErrorInfo));
                            }
                        , UHVWorkerVarSet::uniqueQtConnectionType);
    }
}

void ErrorAnnouncement::onEntry(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("State Entered !"));
    if (TimerIntervalMSecs > 0)
        timer.start();
}

void ErrorAnnouncement::onExit(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("Leave State !"));
    if (TimerIntervalMSecs > 0)
        timer.stop();
    VarSetPtr->clearError();
}
