#ifndef WRITEBYTES_H
#define WRITEBYTES_H

#include <QState>
#include "serialportworkerproperty.h"

class writeBytes : public QState
{
public:
    writeBytes(SerialPortWorkerProperty * instantProperty, quint16 WriteTimeOutInMilisecond = 100, QState *parent = Q_NULLPTR);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    SerialPortWorkerProperty * currentProperty = Q_NULLPTR;
    quint16 TimeOut4WriteInMilisecond = 0;
};

#endif // WRITEBYTES_H
