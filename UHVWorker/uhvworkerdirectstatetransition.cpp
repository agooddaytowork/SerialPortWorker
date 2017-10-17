#include "uhvworkerdirectstatetransition.h"

UHVWorkerDirectStateTransition::UHVWorkerDirectStateTransition(UHVWorkerVarSet *Sender, QAbstractState *destinationState)
    : QSignalTransition(Sender, &UHVWorkerVarSet::DirectStateTransitionRequest)
{
    this->setTargetState(destinationState);
}

bool UHVWorkerDirectStateTransition::eventTest(QEvent *e)
{
    if (!(QSignalTransition::eventTest(e) && this->targetState()))
        return false;
    QStateMachine::SignalEvent * se = static_cast<QStateMachine::SignalEvent *>(e);
    return (se->arguments().at(0).toString() == this->targetState()->objectName());
}
