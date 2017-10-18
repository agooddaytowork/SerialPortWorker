#ifndef IDLESERIALPORTWORKER_H
#define IDLESERIALPORTWORKER_H

#include "serialportworkerbasis.h"

class idleSerialPortWorker : public QState
{
public:
    idleSerialPortWorker(SerialPortWorkerBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    SerialPortWorkerBasis *basisptr = nullptr;
};

#endif // IDLESERIALPORTWORKER_H
