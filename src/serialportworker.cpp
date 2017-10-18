#include "serialportworker.h"

SerialPortWorker::SerialPortWorker()
{
    currentProperty = new SerialPortWorkerProperty(this);

    QState *main = new QState();
    main->setObjectName("main");

    requestSerialPortInfo * state1 = new requestSerialPortInfo(currentProperty,3000,main);
    state1->setObjectName("requestSerialPortInfo");
    validateSerialPortInfo *state2 = new validateSerialPortInfo(currentProperty,main);
    state2->setObjectName("validateSerialPortInfo");
    connectSerialPort *state3 = new connectSerialPort(currentProperty,main);
    state3->setObjectName("connectSerialPort");
    writeBytes *state4 = new writeBytes(currentProperty,100,main);
    state4->setObjectName("writeBytes");
    readBytes *state5 = new readBytes(currentProperty,300,main);
    state5->setObjectName("readBytes");
    wait4ErrorHandler4SerialPortWorker *state7 = new wait4ErrorHandler4SerialPortWorker(currentProperty,3000);
    state7->setObjectName("wait4ErrorHandler4SerialPortWorker");

    main->addTransition(currentProperty, &SerialPortWorkerProperty::PortNameChanged,state2);
    main->addTransition(currentProperty, &SerialPortWorkerProperty::ErrorOccurred,state7);
    main->setInitialState(state1);

    state2->addTransition(new directTransition4SerialPortWorkerState(currentProperty,state1));
    state2->addTransition(new directTransition4SerialPortWorkerState(currentProperty,state3));
    state3->addTransition(new directTransition4SerialPortWorkerState(currentProperty,state4));
    state4->addTransition(new directTransition4SerialPortWorkerState(currentProperty,state5));
    state4->addTransition(currentProperty, &SerialPortWorkerProperty::firstGlobalSignalAdded,state4);
    state5->addTransition(new directTransition4SerialPortWorkerState(currentProperty,state4));

    state7->addTransition(currentProperty, &SerialPortWorkerProperty::PortNameChanged,state2);
    state7->addTransition(currentProperty, &SerialPortWorkerProperty::restartSerialPortConnection,state3);

    this->addState(main);
    this->addState(state7);
    this->setInitialState(main);
    this->setErrorState(state7);

    QObject::connect(currentProperty, &SerialPortWorkerProperty::Out, this, &SerialPortWorker::Out);

    anIf(SerialPortWorkerDbgEn, anTrk("SerialPortWorker Constructed"));
}

void SerialPortWorker::In(const GlobalSignal &aGlobalSignal)
{
    anIf(SerialPortWorkerDbgEn, anTrk("SerialPortWorker Receives A Signal"));
    QString enumTypeName(aGlobalSignal.Type.typeName());
    if (enumTypeName == QStringLiteral("SerialPortWorkerProperty::Data"))
    {
        switch (aGlobalSignal.Type.toInt()) {
        case SerialPortWorkerProperty::requestPortName:
        {
            anIf(SerialPortWorkerPropertyDbgEn, anInfo("requestPortName"));
            GlobalSignal replyGlobalSignalPortName;
            replyGlobalSignalPortName.Type = QVariant::fromValue(SerialPortWorkerProperty::replyPortName);
            replyGlobalSignalPortName.Data = QVariant::fromValue(currentProperty->PortName);
            emit Out(replyGlobalSignalPortName);
            break;
        }
        case SerialPortWorkerProperty::replyPortName:
        {
            anIf(SerialPortWorkerPropertyDbgEn, anInfo("replyPortName"));
            QString newPortName = aGlobalSignal.Data.toString();
            if (newPortName.size())
            {
                currentProperty->PortName = newPortName;
                anIf(SerialPortWorkerPropertyDbgEn, anInfo("PortName=" << newPortName));
                emit currentProperty->PortNameChanged();
            }
            break;
        }
        case SerialPortWorkerProperty::addAGlobalSignal:
        {
            anIf(SerialPortWorkerPropertyDbgEn, anInfo("addAGlobalSignal"));
            currentProperty->addOneGlobalSignal(aGlobalSignal);
            break;
        }
        case SerialPortWorkerProperty::disconnectSerialPort:
        {
            anIf(SerialPortWorkerPropertyDbgEn, anInfo("disconnectSerialPort"));
            currentProperty->PortName.clear();
            emit currentProperty->PortNameChanged();
            break;
        }
        case SerialPortWorkerProperty::restartSerialPort:
        {
            anIf(SerialPortWorkerPropertyDbgEn, anInfo("restartSerialPort"));
            emit currentProperty->restartSerialPortConnection();
            break;
        }
        case SerialPortWorkerProperty::clearBuffer:
        {
            anIf(SerialPortWorkerPropertyDbgEn, anAck("clearBuffer"));
            currentProperty->clearPrioritizedBuffer();
            GlobalSignal notifyBufferCleared;
            notifyBufferCleared.Type = QVariant::fromValue(SerialPortWorkerProperty::BufferCleared);
            emit Out(notifyBufferCleared);
            break;
        }
        default:
            break;
        }
    }
}
