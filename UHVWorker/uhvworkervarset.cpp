#include "uhvworkervarset.h"

UHVWorkerVarSet::UHVWorkerVarSet(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<QSerialPort::SerialPortError>("QSerialPort::SerialPortError");
    qRegisterMetaType<UHVWorkerVarSet::PrioritizedCommandMessage>("UHVWorkerVarSet::PrioritizedCommandMessage");
    anIf(UHVWorkerVarSetDbgEn, anTrk("Object Constructed !"));
    initialize();    
}

UHVWorkerVarSet::~UHVWorkerVarSet()
{
    dispose();
    anIf(UHVWorkerVarSetDbgEn, anTrk("Object Destroyed !"));
}

void UHVWorkerVarSet::configSerialPort()
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
        anIf(UHVWorkerVarSetDbgEn, anInfo("SerialPort Configured !"));
    }
    else
    {
        anIf(UHVWorkerVarSetDbgEn, anWarn("SerialPort IS NULL !"));
    }
}

void UHVWorkerVarSet::deleteSerialPort()
{
    if (SerialPort)
    {
        if (SerialPort->isOpen())
        {
            SerialPort->close();
            emit Out(QVariant::fromValue(UHVWorkerVarSet::SerialPortDisconnected));
        }
        delete SerialPort;
        SerialPort = Q_NULLPTR;
    }
}

void UHVWorkerVarSet::setError(const UHVWorkerVarSet::Error &anErrorType, const QString &anErrorInfo)
{
    if (anErrorType!=NoError)
    {
        anIf(UHVWorkerVarSetDbgEn, anWarn("Error Occurred !"));
        ErrorType = anErrorType;
        ErrorInfo = anErrorInfo;
        emit ErrorOccurred();
    }
}

void UHVWorkerVarSet::clearError()
{
    anIf(UHVWorkerVarSetDbgEn && (ErrorType!=NoError), anInfo("Clear Error !"));
    ErrorType = NoError;
    ErrorInfo.clear();
}

void UHVWorkerVarSet::SerialPortErrorOccurred(QSerialPort::SerialPortError error)
{
    if ((error!=QSerialPort::NoError) && (error!=QSerialPort::TimeoutError))
        setError(UHVWorkerVarSet::SerialPortError,QString(UHVWorkerVarSet::QSerialPortErrorMetaEnum.valueToKey(static_cast<int>(error))));
}

void UHVWorkerVarSet::initialize()
{
    dispose();
    PortName.clear();
    if (pendingMessageList.size())
        pendingMessageList.clear();
    lastTransmittedMessagePriority = 0;
    clearError();
    anIf(UHVWorkerVarSetDbgEn, anTrk("Object Initialized !"));
}

void UHVWorkerVarSet::dispose()
{
    anIf(UHVWorkerVarSetDbgEn && SerialPort, anTrk("Object Cleaned !"));
    deleteSerialPort();
}

const QMetaEnum UHVWorkerVarSet::DataMetaEnum = QMetaEnum::fromType<UHVWorkerVarSet::Data>();
const QMetaEnum UHVWorkerVarSet::ErrorMetaEnum = QMetaEnum::fromType<UHVWorkerVarSet::Error>();
const QMetaEnum UHVWorkerVarSet::WarningMetaEnum = QMetaEnum::fromType<UHVWorkerVarSet::Warning>();
const QMetaEnum UHVWorkerVarSet::NotificationMetaEnum = QMetaEnum::fromType<UHVWorkerVarSet::Notification>();
const QMetaEnum UHVWorkerVarSet::QSerialPortErrorMetaEnum = QMetaEnum::fromType<QSerialPort::SerialPortError>();
