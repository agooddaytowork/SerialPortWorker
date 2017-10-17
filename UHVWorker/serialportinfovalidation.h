#ifndef SERIALPORTINFOVALIDATION_H
#define SERIALPORTINFOVALIDATION_H

#include <QState>
#include <QSerialPortInfo>
#include "anlogger.h"
#include "uhvworkervarset.h"

class SerialPortInfoValidation : public QState
{
public:
    SerialPortInfoValidation(UHVWorkerVarSet *VarSet);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    UHVWorkerVarSet *VarSetPtr = Q_NULLPTR;
};

#endif // SERIALPORTINFOVALIDATION_H
