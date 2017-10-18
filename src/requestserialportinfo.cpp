#include "requestserialportinfo.h"

requestSerialPortInfo::requestSerialPortInfo(SerialPortWorkerProperty *currentProperty, quint32 TimerIntervalInMSecs, QState *parent) :
    QState(parent), TimerIntervalMSecs(TimerIntervalInMSecs)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("requestSerialPortInfo Constructed"));
    if (TimerIntervalInMSecs > 0)
    {
        timer.setParent(this);
        timer.setInterval(TimerIntervalInMSecs);
        timer.setSingleShot(true);
        QObject::connect(&timer, &QTimer::timeout
                        , this
                        , [currentProperty,this](){
                                anIf(SerialPortWorkerPropertyDbgEn, anInfo("Emit requestPortName"));
                                GlobalSignal requestDataPortName;
                                requestDataPortName.Type = QVariant::fromValue(SerialPortWorkerProperty::requestPortName);
                                requestDataPortName.Data = QVariant::fromValue(this->machine()->objectName());
                                emit currentProperty->Out(requestDataPortName);
                          }
                        , SerialPortWorkerProperty::uniqueQtConnectionType);
    }
}

void requestSerialPortInfo::onEntry(QEvent *)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("requestSerialPortInfo Entered"));
    if (TimerIntervalMSecs > 0)
        timer.start();
}

void requestSerialPortInfo::onExit(QEvent *)
{
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("Leave requestSerialPortInfo"));
    if (TimerIntervalMSecs > 0)
        timer.stop();
}
