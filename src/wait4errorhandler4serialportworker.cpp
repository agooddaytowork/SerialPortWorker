#include "wait4errorhandler4serialportworker.h"

wait4ErrorHandler4SerialPortWorker::wait4ErrorHandler4SerialPortWorker(SerialPortWorkerProperty *instantProperty, quint32 TimerIntervalInMilisecond)
    : currentProperty(instantProperty), TimerIntervalMSecs(TimerIntervalInMilisecond)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("wait4ErrorHandler4SerialPortWorker Constructed"));
    if (TimerIntervalInMilisecond > 0)
    {
        timer.setParent(this);
        timer.setInterval(TimerIntervalInMilisecond);
        timer.setSingleShot(true);
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

void wait4ErrorHandler4SerialPortWorker::onEntry(QEvent *)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("wait4ErrorHandler4SerialPortWorker Entered"));
    if (TimerIntervalMSecs > 0)
        timer.start();
}

void wait4ErrorHandler4SerialPortWorker::onExit(QEvent *)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("Leave wait4ErrorHandler4SerialPortWorker"));
    if (TimerIntervalMSecs > 0)
        timer.stop();
    currentProperty->clearError();
}
