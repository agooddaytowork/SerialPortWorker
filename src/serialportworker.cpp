#include "serialportworker.h"

SerialPortWorker::SerialPortWorker(QObject *parent) : QStateMachine(parent)
{
    currentBasis = new SerialPortWorkerBasis(this);
    QObject::connect(currentBasis, &SerialPortWorkerBasis::Out, this, &SerialPortWorker::Out);

    QState *main = new QState();
    main->setObjectName(QStringLiteral("main"));

    uninitiatedSerialPortWorker *state0 = new uninitiatedSerialPortWorker(currentBasis,main);
    idleSerialPortWorker *state1 = new idleSerialPortWorker(currentBasis,main);
    runningSerialPortWorker *state2 = new runningSerialPortWorker(currentBasis,main);
    readBytesSerialPortWorker *state3 = new readBytesSerialPortWorker(currentBasis,main);

    state0->addTransition(currentBasis, &SerialPortWorkerBasis::goIdle, state1);
    state1->addTransition(currentBasis, &SerialPortWorkerBasis::GlobalSignalExecutionRequested, state2);
    state2->addTransition(currentBasis, &SerialPortWorkerBasis::goIdle, state1);
    state2->addTransition(currentBasis, &SerialPortWorkerBasis::GlobalSignalExecutionRequested, state2);
    state2->addTransition(currentBasis, &SerialPortWorkerBasis::readingBytesSerialPortWorker, state3);
    state3->addTransition(currentBasis, &SerialPortWorkerBasis::GlobalSignalExecutionRequested, state2);

    main->setInitialState(state0);

    errorSerialPortWorker *state7 = new errorSerialPortWorker(currentBasis);

    state7->addTransition(currentBasis, &SerialPortWorkerBasis::InitiationRequested, state0);

    main->addTransition(currentBasis, &SerialPortWorkerBasis::ErrorOccurred, state7);
    main->addTransition(currentBasis, &SerialPortWorkerBasis::InitiationRequested, state0);

    addState(main);
    addState(state7);
    setInitialState(main);
    anIf(SerialPortWorkerDbgEn, anAck("SerialPortWorker Constructed"));
}

void SerialPortWorker::In(const GlobalSignal &aGlobalSignal)
{
    if (isRunning())
    {
        currentBasis->In(aGlobalSignal);
    }
}
