#ifndef RUNNINGSERIALPORTWORKER_H
#define RUNNINGSERIALPORTWORKER_H

#include "serialportworkerbasis.h"

class runningSerialPortWorker : public QState
{
public:
    runningSerialPortWorker(SerialPortWorkerBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    SerialPortWorkerBasis *basisptr = nullptr;
};

#endif // RUNNINGSERIALPORTWORKER_H
