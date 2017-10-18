#ifndef READBYTESSERIALPORTWORKER_H
#define READBYTESSERIALPORTWORKER_H

#include "serialportworkerbasis.h"

class readBytesSerialPortWorker : public QState
{
public:
    readBytesSerialPortWorker(SerialPortWorkerBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    SerialPortWorkerBasis * basisptr = nullptr;
};

#endif // READBYTESSERIALPORTWORKER_H
