#ifndef SERIALPORTWORKER_H
#define SERIALPORTWORKER_H

#define SerialPortWorkerDbgEn 1

#include <QStateMachine>
#include "serialportworkerbasis.h"
#include "shared/directtransition.h"
#include "errorserialportworker.h"
#include "idleserialportworker.h"
#include "readbytesserialportworker.h"
#include "runningserialportworker.h"
#include "uninitiatedserialportworker.h"

class SerialPortWorker : public QStateMachine
{
    Q_OBJECT
public:
    explicit SerialPortWorker(QObject *parent = nullptr);
signals:
    void Out(const GlobalSignal &);
public slots:
    void In(const GlobalSignal &aGlobalSignal);
private:
    SerialPortWorkerBasis *currentBasis = nullptr;
};

#endif // SERIALPORTWORKER_H
