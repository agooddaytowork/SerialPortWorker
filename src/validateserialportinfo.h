#ifndef VALIDATESERIALPORTINFO_H
#define VALIDATESERIALPORTINFO_H

#include <QState>
#include "serialportworkerproperty.h"

class validateSerialPortInfo : public QState
{
public:
    validateSerialPortInfo(SerialPortWorkerProperty * instantProperty, QState *parent = Q_NULLPTR);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    SerialPortWorkerProperty * currentProperty = Q_NULLPTR;
};

#endif // VALIDATESERIALPORTINFO_H
