#ifndef CONNECTSERIALPORT_H
#define CONNECTSERIALPORT_H

#include <QState>
#include "serialportworkerproperty.h"

class connectSerialPort : public QState
{
public:
    connectSerialPort(SerialPortWorkerProperty * instantProperty, QState *parent = Q_NULLPTR);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    SerialPortWorkerProperty * currentProperty = Q_NULLPTR;
};

#endif // CONNECTSERIALPORT_H
