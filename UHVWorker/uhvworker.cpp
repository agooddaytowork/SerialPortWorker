#include "uhvworker.h"

UHVWorker::UHVWorker()
{
    anIf(UHVWorkerDbgEn, anTrk("Object Constructed !"));
    WorkingVarSet = new UHVWorkerVarSet(this);
    SerialPortInfoRequest * state0 = new SerialPortInfoRequest(WorkingVarSet);
    state0->setObjectName("SerialPortInfoRequest");
    SerialPortInfoValidation * state1 = new SerialPortInfoValidation(WorkingVarSet);
    state1->setObjectName("SerialPortInfoValidation");
    SerialPortConnectionEstablishment * state2 = new SerialPortConnectionEstablishment(WorkingVarSet);
    state2->setObjectName("SerialPortConnectionEstablishment");
    SolitaryMessageTransmission * state3 = new SolitaryMessageTransmission(WorkingVarSet);
    state3->setObjectName("SolitaryMessageTransmission");
    MessageReceiveAndEmitOut * state4 = new MessageReceiveAndEmitOut(WorkingVarSet);
    state4->setObjectName("MessageReceiveAndEmitOut");
    ErrorAnnouncement * state7 = new ErrorAnnouncement(WorkingVarSet,1000);
    state7->setObjectName("ErrorAnnouncement");

    state0->addTransition(WorkingVarSet, &UHVWorkerVarSet::PortNameChanged, state1);    
    state1->addTransition(WorkingVarSet, &UHVWorkerVarSet::PortNameChanged, state1);
    state2->addTransition(WorkingVarSet, &UHVWorkerVarSet::PortNameChanged, state1);
    state3->addTransition(WorkingVarSet, &UHVWorkerVarSet::PortNameChanged, state1);
    state4->addTransition(WorkingVarSet, &UHVWorkerVarSet::PortNameChanged, state1);
    state7->addTransition(WorkingVarSet, &UHVWorkerVarSet::PortNameChanged, state1);

    state1->addTransition(new UHVWorkerDirectStateTransition(WorkingVarSet, state0));
    state1->addTransition(new UHVWorkerDirectStateTransition(WorkingVarSet, state2));

    state2->addTransition(WorkingVarSet, &UHVWorkerVarSet::ErrorOccurred, state7);
    state2->addTransition(new UHVWorkerDirectStateTransition(WorkingVarSet, state3));

    state3->addTransition(WorkingVarSet, &UHVWorkerVarSet::ErrorOccurred, state7);
    state3->addTransition(new UHVWorkerDirectStateTransition(WorkingVarSet, state4));
    state3->addTransition(WorkingVarSet, &UHVWorkerVarSet::AFirstPrioritizedCommandMessageReceived, state3);

    state4->addTransition(WorkingVarSet, &UHVWorkerVarSet::ErrorOccurred, state7);
    state4->addTransition(new UHVWorkerDirectStateTransition(WorkingVarSet, state3));

    state7->addTransition(WorkingVarSet, &UHVWorkerVarSet::restartSerialPortConnection, state2);

    this->addState(state0);
    this->addState(state1);
    this->addState(state2);
    this->addState(state3);
    this->addState(state4);
    this->addState(state7);
    this->setInitialState(state0);
    this->setErrorState(state7);

    connect(WorkingVarSet, &UHVWorkerVarSet::Out, this, &UHVWorker::Out);
}

void UHVWorker::In(QVariant enumVar, QVariant dataVar)
{
    anIf(UHVWorkerDbgEn, anTrk("Signal-To-UHVWorker Received !"));
    QString enumVarTypeName(enumVar.typeName());
    if (enumVarTypeName == QStringLiteral("UHVWorkerVarSet::Data"))
    {
        switch (enumVar.toInt()) {
        case UHVWorkerVarSet::requestPortName:
        {
            anIf(UHVWorkerDbgEn, anInfo("requestPortName"));
            emit Out(QVariant::fromValue(UHVWorkerVarSet::replyPortName),
                     QVariant::fromValue(WorkingVarSet->PortName));
            break;
        }
        case UHVWorkerVarSet::replyPortName:
        {
            anIf(UHVWorkerDbgEn, anInfo("replyPortName"));
            QString newPortName = dataVar.toString();
            if (newPortName.size())
            {
                WorkingVarSet->PortName = newPortName;
                anIf(UHVWorkerDbgEn, anInfo("PortName=" << newPortName));
                emit WorkingVarSet->PortNameChanged();
            }
            break;
        }
        case UHVWorkerVarSet::addAnUHVPrioritizedCommandMessage:
        {
            anIf(UHVWorkerDbgEn, anInfo("addAnUHVPrioritizedCommandMessage"));
            WorkingVarSet->addOneUHVPrioritizedCommandMessage(dataVar.value<UHVWorkerVarSet::PrioritizedCommandMessage>());
            break;
        }
        case UHVWorkerVarSet::disconnectSerialPort:
        {
            anIf(UHVWorkerDbgEn, anInfo("disconnectSerialPort"));
            WorkingVarSet->PortName.clear();
            emit WorkingVarSet->PortNameChanged();
            break;
        }
        case UHVWorkerVarSet::restartSerialPort:
        {
            anIf(UHVWorkerDbgEn, anInfo("restartSerialPort"));
            emit WorkingVarSet->restartSerialPortConnection();
            break;
        }
        case UHVWorkerVarSet::clearPendingMessageList:
        {
            anIf(UHVWorkerDbgEn, anAck("clearPendingMessageList"));
            WorkingVarSet->pendingMessageList.clear();
            emit Out(QVariant::fromValue(UHVWorkerVarSet::pendingMessageListCleared));
            break;
        }
        default:
            break;
        }
    }
}
