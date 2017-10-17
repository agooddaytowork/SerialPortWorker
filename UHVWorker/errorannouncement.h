#ifndef ERRORANNOUNCEMENT_H
#define ERRORANNOUNCEMENT_H

#include <QState>
#include <QTimer>
#include "anlogger.h"
#include "uhvworkervarset.h"

class ErrorAnnouncement : public QState
{
public:
    ErrorAnnouncement(UHVWorkerVarSet *VarSet, quint32 TimerIntervalInMilisecond = 0);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
    UHVWorkerVarSet * VarSetPtr = Q_NULLPTR;
    quint32 TimerIntervalMSecs = 0;
};

#endif // ERRORANNOUNCEMENT_H
