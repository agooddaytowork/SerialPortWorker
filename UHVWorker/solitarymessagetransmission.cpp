#include "solitarymessagetransmission.h"

SolitaryMessageTransmission::SolitaryMessageTransmission(UHVWorkerVarSet *VarSet, quint16 WriteTimeOutInMilisecond)
    : VarSetPtr(VarSet), TimeOut4WriteInMilisecond(WriteTimeOutInMilisecond)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("State Constructed !"));
}

void SolitaryMessageTransmission::onEntry(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("State Entered !"));
    qApp->processEvents();
    while (VarSetPtr->pendingMessageList.size())
    {
        if (VarSetPtr->pendingMessageList.last()->size())
            break;
        else
            VarSetPtr->pendingMessageList.remove(VarSetPtr->pendingMessageList.lastKey());
    }
    if (VarSetPtr->pendingMessageList.size())
    {
        VarSetPtr->lastTransmittedMessagePriority = VarSetPtr->pendingMessageList.lastKey();
        VarSetPtr->lastTransmittedMessage = VarSetPtr->pendingMessageList.last()->takeFirst();
        VarSetPtr->SerialPort->write(VarSetPtr->lastTransmittedMessage.first);
        if (VarSetPtr->SerialPort->waitForBytesWritten(TimeOut4WriteInMilisecond))
        {
            anIf(UHVWorkerVarSetDbgEn, anAck("Bytes Written !"));
            emit VarSetPtr->Out(QVariant::fromValue(UHVWorkerVarSet::MessageTransmitted),
                                QVariant::fromValue(UHVWorkerVarSet::PrioritizedCommandMessage(VarSetPtr->lastTransmittedMessagePriority, VarSetPtr->lastTransmittedMessage)));
        }
        else
        {
            anIf(UHVWorkerVarSetDbgEn, anWarn("Bytes Written Timed Out !"));
            emit VarSetPtr->Out(QVariant::fromValue(UHVWorkerVarSet::BytesWrittenTimedOut));
        }
        emit VarSetPtr->DirectStateTransitionRequest("MessageReceiveAndEmitOut");
    }
    else
    {
        anIf(UHVWorkerVarSetDbgEn, anInfo("No Message In Buffer !"));
    }
}

void SolitaryMessageTransmission::onExit(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anTrk("Leave State !"));
}
