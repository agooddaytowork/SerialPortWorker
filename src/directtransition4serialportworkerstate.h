#ifndef DIRECTTRANSITION4SERIALPORTWORKERSTATE_H
#define DIRECTTRANSITION4SERIALPORTWORKERSTATE_H

#include <QSignalTransition>
#include <QAbstractState>
#include "serialportworkerproperty.h"

class directTransition4SerialPortWorkerState : public QSignalTransition
{
public:
    directTransition4SerialPortWorkerState(SerialPortWorkerProperty *Sender, QAbstractState *destinationState);
protected:
    virtual bool eventTest(QEvent *e) override;
};

#endif // DIRECTTRANSITION4SERIALPORTWORKERSTATE_H
