#include "directtransition4serialportworkerstate.h"

directTransition4SerialPortWorkerState::directTransition4SerialPortWorkerState(SerialPortWorkerProperty *Sender, QAbstractState *destinationState)
    : QSignalTransition(Sender, &SerialPortWorkerProperty::requestDirectTransitionForSerialPortWorkerState)
{
    this->setTargetState(destinationState);
}

bool directTransition4SerialPortWorkerState::eventTest(QEvent *e)
{
    if (!(QSignalTransition::eventTest(e) && this->targetState()))
        return false;
    QStateMachine::SignalEvent * se = static_cast<QStateMachine::SignalEvent *>(e);
    return (se->arguments().at(0).toString() == this->targetState()->objectName());
}
