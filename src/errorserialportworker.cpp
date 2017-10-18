#include "errorserialportworker.h"

errorSerialPortWorker::errorSerialPortWorker(SerialPortWorkerBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("errorSerialPortWorker"));
    anIf(SerialPortWorkerBasisDbgEn, anAck("errorSerialPortWorker Constructed"));
}

void errorSerialPortWorker::onEntry(QEvent *)
{
    anIf(SerialPortWorkerBasisDbgEn, anTrk("errorSerialPortWorker Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->emitErrorGlobalSignal();
}

void errorSerialPortWorker::onExit(QEvent *)
{
    anIf(SerialPortWorkerBasisDbgEn, anTrk("Leave errorSerialPortWorker"));
    basisptr->clearError();
    basisptr->previousStateName = objectName();
}
