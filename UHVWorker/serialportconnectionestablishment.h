#ifndef SERIALPORTCONNECTIONESTABLISHMENT_H
#define SERIALPORTCONNECTIONESTABLISHMENT_H

#include <QState>
#include <QSerialPort>
#include "anlogger.h"
#include "uhvworkervarset.h"

class SerialPortConnectionEstablishment : public QState
{
public:
    SerialPortConnectionEstablishment(UHVWorkerVarSet * VarSet);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    UHVWorkerVarSet *VarSetPtr = Q_NULLPTR;
};

#endif // SERIALPORTCONNECTIONESTABLISHMENT_H
