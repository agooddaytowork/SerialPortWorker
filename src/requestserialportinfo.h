#ifndef REQUESTSERIALPORTINFO_H
#define REQUESTSERIALPORTINFO_H

#include <QState>
#include <QTimer>
#include "serialportworkerproperty.h"

class requestSerialPortInfo : public QState
{
public:
    requestSerialPortInfo(SerialPortWorkerProperty * currentProperty, quint32 TimerIntervalInMSecs = 0, QState *parent = Q_NULLPTR);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
    quint32 TimerIntervalMSecs = 0;
};

#endif // REQUESTSERIALPORTINFO_H
