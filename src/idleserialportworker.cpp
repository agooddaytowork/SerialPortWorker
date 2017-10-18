#include "idleserialportworker.h"

idleSerialPortWorker::idleSerialPortWorker(SerialPortWorkerBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("idleSerialPortWorker"));
    anIf(SerialPortWorkerBasisDbgEn, anAck("idleSerialPortWorker Constructed"));
}

void idleSerialPortWorker::onEntry(QEvent *)
{
    anIf(SerialPortWorkerBasisDbgEn, anTrk("idleSerialPortWorker Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    if (basisptr->previousStateName == QStringLiteral("uninitiatedSerialPortWorker"))
    {
        basisptr->queueNotificationReadyToWork();
        emit basisptr->goToState2();
    }
}

void idleSerialPortWorker::onExit(QEvent *)
{
    anIf(SerialPortWorkerBasisDbgEn, anTrk("Leave idleSerialPortWorker"));
    basisptr->previousStateName = objectName();
}
