#ifndef UNINITIATEDSERIALPORTWORKER_H
#define UNINITIATEDSERIALPORTWORKER_H

#include "serialportworkerbasis.h"

class uninitiatedSerialPortWorker : public QState
{
public:
    uninitiatedSerialPortWorker(SerialPortWorkerBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    SerialPortWorkerBasis *basisptr = nullptr;
};

#endif // UNINITIATEDSERIALPORTWORKER_H
