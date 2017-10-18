#ifndef READBYTES_H
#define READBYTES_H

#include <QState>
#include <QDateTime>
#include "serialportworkerproperty.h"

class readBytes : public QState
{
public:
    readBytes(SerialPortWorkerProperty * instantProperty, quint16 ReadTimeOutInMilisecond = 300, QState *parent = Q_NULLPTR);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    SerialPortWorkerProperty * currentProperty = Q_NULLPTR;
    quint16 TimeOut4ReadInMilisecond = 0;
};

#endif // READBYTES_H
