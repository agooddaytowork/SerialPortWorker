#include "wait4errorhandler.h"

wait4ErrorHandler::wait4ErrorHandler(SerialPortWorkerProperty *instantProperty, quint32 TimerIntervalInMilisecond)
    : currentProperty(instantProperty), TimerIntervalMSecs(TimerIntervalInMilisecond)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("wait4ErrorHandler Constructed"));
    if (TimerIntervalInMilisecond > 0)
    {
        timer.setParent(this);
        timer.setInterval(TimerIntervalInMilisecond);
        QObject::connect(&timer, &QTimer::timeout
                        , this
                        , [instantProperty](){
                                anIf(SerialPortWorkerPropertyDbgEn,
                                     anError("Emit SerialPortWorkerProperty::Error");
                                      anInfo("ErrorType : " +
                                               QString(SerialPortWorkerProperty::ErrorMetaEnum.valueToKey(static_cast<int>(instantProperty->ErrorType))));
                                      anInfo("ErrorInfo : " + instantProperty->ErrorInfo);
                                     );
                                GlobalSignal notifyError;
                                notifyError.Type = QVariant::fromValue(instantProperty->ErrorType);
                                notifyError.Data = QVariant::fromValue(instantProperty->ErrorInfo);
                                emit instantProperty->Out(notifyError);
                            }
                        , SerialPortWorkerProperty::uniqueQtConnectionType);
    }
}

void wait4ErrorHandler::onEntry(QEvent *)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("wait4ErrorHandler Entered"));
    if (TimerIntervalMSecs > 0)
        timer.start();
}

void wait4ErrorHandler::onExit(QEvent *)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("Leave wait4ErrorHandler"));
    if (TimerIntervalMSecs > 0)
        timer.stop();
    currentProperty->clearError();
}
