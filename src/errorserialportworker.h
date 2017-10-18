#ifndef ERRORSERIALPORTWORKER_H
#define ERRORSERIALPORTWORKER_H

#include "serialportworkerbasis.h"

class errorSerialPortWorker : public QState
{
public:
    errorSerialPortWorker(SerialPortWorkerBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    SerialPortWorkerBasis *basisptr = nullptr;
};

#endif // ERRORSERIALPORTWORKER_H
