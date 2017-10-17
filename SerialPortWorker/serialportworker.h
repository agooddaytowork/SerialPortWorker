#ifndef SERIALPORTWORKER_H
#define SERIALPORTWORKER_H

#define SerialPortWorkerDbgEn 1

#include "serialportworkerproperty.h"
#include "directtransition4serialportworkerstate.h"
#include "requestserialportinfo.h"
#include "validateserialportinfo.h"
#include "connectserialport.h"
#include "writebytes.h"
#include "readbytes.h"
#include "wait4errorhandler.h"

class SerialPortWorker : public QStateMachine
{
    Q_OBJECT
public:
    SerialPortWorker();
signals:
    void Out(const GlobalSignal &);
public slots:
    void In(const GlobalSignal &aGlobalSignal);
private:
    SerialPortWorkerProperty * currentProperty = Q_NULLPTR;
};

#endif // SERIALPORTWORKER_H
