#include "runningserialportworker.h"

runningSerialPortWorker::runningSerialPortWorker(SerialPortWorkerBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("runningSerialPortWorker"));
    anIf(SerialPortWorkerBasisDbgEn, anAck("runningSerialPortWorker Constructed"));
}

void runningSerialPortWorker::onEntry(QEvent *)
{
    anIf(SerialPortWorkerBasisDbgEn, anTrk("runningSerialPortWorker Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->executePrioritizedBuffer();
    if (basisptr->isCurrentRunningCycleCompleted)
    {
        if (basisptr->prioritizedBuffer.isEmpty())
        {
            emit basisptr->goToState1();
        }
        else
        {
            emit basisptr->goToState2();
        }
    }
}

void runningSerialPortWorker::onExit(QEvent *)
{
    anIf(SerialPortWorkerBasisDbgEn, anTrk("Leave runningSerialPortWorker"));
    basisptr->previousStateName = objectName();
}
