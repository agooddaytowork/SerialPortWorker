#include "readbytesserialportworker.h"

readBytesSerialPortWorker::readBytesSerialPortWorker(SerialPortWorkerBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("readBytesSerialPortWorker"));
    anIf(SerialPortWorkerBasisDbgEn, anAck("readBytesSerialPortWorker Constructed"));
}

void readBytesSerialPortWorker::onEntry(QEvent *)
{
    anIf(SerialPortWorkerBasisDbgEn, anTrk("readBytesSerialPortWorker Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->readAllDataFromSerialPort();
    basisptr->isCurrentRunningCycleCompleted = true;
    emit basisptr->goToState2();
}

void readBytesSerialPortWorker::onExit(QEvent *)
{
    anIf(SerialPortWorkerBasisDbgEn, anTrk("Leave readBytesSerialPortWorker"));
    basisptr->previousStateName = objectName();
}
