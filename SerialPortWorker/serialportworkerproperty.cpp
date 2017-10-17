#include "serialportworkerproperty.h"

SerialPortWorkerProperty::SerialPortWorkerProperty(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QSerialPort::SerialPortError>("QSerialPort::SerialPortError");
    qRegisterMetaType<SerialPortWorkerProperty::DataMessage>("SerialPortWorkerProperty::DataMessage");
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("SerialPortWorkerProperty Constructed"));
    initialize();
}

SerialPortWorkerProperty::~SerialPortWorkerProperty()
{
    dispose();
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("Destroy SerialPortWorkerProperty"));
}

void SerialPortWorkerProperty::initialize()
{
    dispose();
    anIf(SerialPortWorkerPropertyDbgEn, anTrk("SerialPortWorkerProperty Initialized"));
}

void SerialPortWorkerProperty::dispose()
{
    anIf(SerialPortWorkerPropertyDbgEn && SerialPort, anTrk("Clean SerialPortWorkerProperty"));
    deleteSerialPort();
    PortName.clear();
    clearPrioritizedBuffer();
    clearError();
}

void SerialPortWorkerProperty::configSerialPort()
{
    if (SerialPort)
    {
        SerialPort->setReadBufferSize(64);
        SerialPort->setDataBits(QSerialPort::Data8);
        SerialPort->setBaudRate(QSerialPort::Baud9600);
        SerialPort->setStopBits(QSerialPort::OneStop);
        SerialPort->setParity(QSerialPort::NoParity);
        SerialPort->setFlowControl(QSerialPort::NoFlowControl);
        SerialPort->setPortName(PortName);
        anIf(SerialPortWorkerPropertyDbgEn, anInfo("SerialPort Configured !"));
    }
    else
    {
        anIf(SerialPortWorkerPropertyDbgEn, anWarn("SerialPort IS NULL !"));
    }
}

void SerialPortWorkerProperty::deleteSerialPort()
{
    if (SerialPort)
    {
        if (SerialPort->isOpen())
        {
            SerialPort->close();
            GlobalSignal notifySerialPortDisconnected;
            notifySerialPortDisconnected.Type = QVariant::fromValue(SerialPortWorkerProperty::SerialPortDisconnected);
            emit Out(notifySerialPortDisconnected);
        }
        delete SerialPort;
        SerialPort = Q_NULLPTR;
    }
}

void SerialPortWorkerProperty::setError(const SerialPortWorkerProperty::Error &anErrorType, const QString &anErrorInfo)
{
    if (anErrorType!=NoError)
    {
        anIf(SerialPortWorkerPropertyDbgEn, anWarn("Error Occurred !"));
        ErrorType = anErrorType;
        ErrorInfo = anErrorInfo;
        emit ErrorOccurred();
    }
}

void SerialPortWorkerProperty::clearError()
{
    anIf(SerialPortWorkerPropertyDbgEn && (ErrorType!=NoError), anInfo("Clear Error !"));
    ErrorType = NoError;
    ErrorInfo.clear();
}

void SerialPortWorkerProperty::clearEmptyList()
{
    while (prioritizedBuffer.size())
    {
        if (prioritizedBuffer.last().size())
            break;
        else
        {
            prioritizedBuffer.remove(prioritizedBuffer.lastKey());
        }
    }
}

void SerialPortWorkerProperty::clearPrioritizedBuffer()
{
    if (prioritizedBuffer.size())
        prioritizedBuffer.clear();
}

void SerialPortWorkerProperty::addOneGlobalSignal(const GlobalSignal &aGlobalSignal)
{
    if (prioritizedBuffer.contains(aGlobalSignal.Priority))
    {
        prioritizedBuffer[aGlobalSignal.Priority].append(aGlobalSignal);
    }
    else
    {
        prioritizedBuffer.insert(aGlobalSignal.Priority, QList<GlobalSignal>({aGlobalSignal}));
    }
    if ((prioritizedBuffer.size() == 1) && (prioritizedBuffer.first().size() == 1))
    {
        emit firstGlobalSignalAdded();
    }
    else
    {
        emit oneGlobalSignalAdded();
    }
}

void SerialPortWorkerProperty::SerialPortErrorOccurred(QSerialPort::SerialPortError error)
{
    if ((error!=QSerialPort::NoError) && (error!=QSerialPort::TimeoutError))
        setError(SerialPortWorkerProperty::SerialPortError,QString(SerialPortWorkerProperty::QSerialPortErrorMetaEnum.valueToKey(static_cast<int>(error))));
}

const QMetaEnum SerialPortWorkerProperty::DataMetaEnum = QMetaEnum::fromType<SerialPortWorkerProperty::Data>();
const QMetaEnum SerialPortWorkerProperty::ErrorMetaEnum = QMetaEnum::fromType<SerialPortWorkerProperty::Error>();
const QMetaEnum SerialPortWorkerProperty::WarningMetaEnum = QMetaEnum::fromType<SerialPortWorkerProperty::Warning>();
const QMetaEnum SerialPortWorkerProperty::NotificationMetaEnum = QMetaEnum::fromType<SerialPortWorkerProperty::Notification>();
const QMetaEnum SerialPortWorkerProperty::QSerialPortErrorMetaEnum = QMetaEnum::fromType<QSerialPort::SerialPortError>();
