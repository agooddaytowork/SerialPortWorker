#ifndef SERIALPORTINFOREQUEST_H
#define SERIALPORTINFOREQUEST_H

#include <QState>
#include <QTimer>
#include "anlogger.h"
#include "uhvworkervarset.h"

class SerialPortInfoRequest : public QState
{
public:
    SerialPortInfoRequest(UHVWorkerVarSet *VarSet, quint32 TimerIntervalInMilisecond = 0);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
    quint32 TimerIntervalMSecs = 0;
};

#endif // SERIALPORTINFOREQUEST_H
