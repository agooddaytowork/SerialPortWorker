#ifndef MESSAGERECEIVEANDEMITOUT_H
#define MESSAGERECEIVEANDEMITOUT_H

#include <QState>
#include "anlogger.h"
#include "uhvworkervarset.h"

class MessageReceiveAndEmitOut : public QState
{
public:
    MessageReceiveAndEmitOut(UHVWorkerVarSet *VarSet, quint16 ReadTimeOutInMilisecond = 300);    
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:    
    UHVWorkerVarSet * VarSetPtr = Q_NULLPTR;
    quint16 TimeOut4ReadInMilisecond = 0;
};

#endif // MESSAGERECEIVEANDEMITOUT_H
