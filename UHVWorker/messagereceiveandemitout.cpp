#include "messagereceiveandemitout.h"

MessageReceiveAndEmitOut::MessageReceiveAndEmitOut(UHVWorkerVarSet *VarSet, quint16 ReadTimeOutInMilisecond)
    : VarSetPtr(VarSet), TimeOut4ReadInMilisecond(ReadTimeOutInMilisecond)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("State Constructed !"));
}

void MessageReceiveAndEmitOut::onEntry(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("State Entered !"));
    qApp->processEvents();
    if (VarSetPtr->SerialPort->waitForReadyRead(TimeOut4ReadInMilisecond*10))
    {
        QByteArray tmpRead(VarSetPtr->SerialPort->readAll());
        while (VarSetPtr->SerialPort->waitForReadyRead(TimeOut4ReadInMilisecond))
        {
            tmpRead+=VarSetPtr->SerialPort->readAll();
        }
        VarSetPtr->lastReceivedMessage = UHVWorkerVarSet::CommandMessage();
        VarSetPtr->lastReceivedMessage.first = tmpRead;
        VarSetPtr->lastReceivedMessage.second = VarSetPtr->lastTransmittedMessage.second;
        anIf(UHVWorkerVarSetDbgEn, anAck("Message Read !"));
        emit VarSetPtr->Out(QVariant::fromValue(UHVWorkerVarSet::replyUHVPrioritizedCommandMessage),
                            QVariant::fromValue(UHVWorkerVarSet::PrioritizedCommandMessage(VarSetPtr->lastTransmittedMessagePriority, VarSetPtr->lastReceivedMessage)));
    }
    else
    {
        anIf(UHVWorkerVarSetDbgEn, anWarn("Ready Read Timed Out !"));
        emit VarSetPtr->Out(QVariant::fromValue(UHVWorkerVarSet::ReadyReadTimedOut));
    }
    emit VarSetPtr->DirectStateTransitionRequest("SolitaryMessageTransmission");
}

void MessageReceiveAndEmitOut::onExit(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("Leave State !"));
}
