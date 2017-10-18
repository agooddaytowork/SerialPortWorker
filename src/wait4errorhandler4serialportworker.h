#ifndef WAIT4ERRORHANDLER4SERIALPORTWORKER_H
#define WAIT4ERRORHANDLER4SERIALPORTWORKER_H

#include <QState>
#include <QTimer>
#include "serialportworkerproperty.h"

class wait4ErrorHandler4SerialPortWorker : public QState
{
public:
    wait4ErrorHandler4SerialPortWorker(SerialPortWorkerProperty * instantProperty, quint32 TimerIntervalInMilisecond = 0);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
    SerialPortWorkerProperty * currentProperty = Q_NULLPTR;
    quint32 TimerIntervalMSecs = 0;
};

#endif // WAIT4ERRORHANDLER4SERIALPORTWORKER_H
