#ifndef UHVWORKERDIRECTSTATETRANSITION_H
#define UHVWORKERDIRECTSTATETRANSITION_H

#include <QSignalTransition>
#include <QAbstractState>
#include "uhvworkervarset.h"

class UHVWorkerDirectStateTransition : public QSignalTransition
{
public:
    UHVWorkerDirectStateTransition(UHVWorkerVarSet *Sender, QAbstractState *destinationState);
protected:
    virtual bool eventTest(QEvent *e) override;
};

#endif // UHVWORKERDIRECTSTATETRANSITION_H
