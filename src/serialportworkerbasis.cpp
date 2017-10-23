#include "serialportworkerbasis.h"

SerialPortWorkerBasis::SerialPortWorkerBasis(QObject *parent) :
    AbstractStateMachineBasis(parent)
{
    registerGlobalSignal;
    SerialPort.setParent(this);
    QObject::connect(&SerialPort, &QSerialPort::errorOccurred, this, &SerialPortWorkerBasis::SerialPortErrorOccurred);
    QObject::connect(&SerialPort, &QSerialPort::aboutToClose, this, [&](){
        GlobalSignal notifySerialPortDisconnected;
        notifySerialPortDisconnected.Type = QVariant::fromValue(SerialPortDisconnected);
        notifySerialPortDisconnected.DstStrs.append(GlobalSignalCoordinatorObjName);
        emit Out(notifySerialPortDisconnected);
    });
    anIf(SerialPortWorkerBasisDbgEn, anAck("SerialPortWorkerBasis Constructed"));
}

SerialPortWorkerBasis::~SerialPortWorkerBasis()
{
    anIf(SerialPortWorkerBasisDbgEn, anWarn("Destroy SerialPortWorkerBasis"));
    dispose();
}

void SerialPortWorkerBasis::uninitiatedSerialPortWorkerOnEntry()
{
    if (!isInitiated)
    {
        if (PortName.isEmpty())
        {
            anIf(SerialPortWorkerBasisDbgEn, anAck("requestPortName Emitted"));
            GlobalSignal signalRequestPortName;
            signalRequestPortName.Type = QVariant::fromValue(requestPortName);
            signalRequestPortName.Data = QVariant::fromValue(parent()->objectName());
            signalRequestPortName.SignalPriority = 200;
            signalRequestPortName.DstStrs.append(GlobalSignalCoordinatorObjName);
            emit Out(signalRequestPortName);
        }
        else
        {
            QString aPortName = PortName;
            initiate(aPortName);
        }
    }
}

void SerialPortWorkerBasis::idleSerialPortWorkerOnEntry()
{
    if (previousStateName == QStringLiteral("uninitiatedSerialPortWorker"))
    {
        GlobalSignal iamReady;
        iamReady.Type = QVariant::fromValue(readyToWork);
        iamReady.Data = QVariant::fromValue(parent()->objectName());
        iamReady.TimeStamp = NOW2String;
        iamReady.DstStrs.append(GlobalSignalCoordinatorObjName);
        iamReady.SignalPriority = 200;
        pushAGlobalSignalIntoPrioritizedBuffer(iamReady);
        emit GlobalSignalExecutionRequested();
    }
}

void SerialPortWorkerBasis::runningSerialPortWorkerOnEntry()
{
    clearCache();
    if (prioritizedBuffer.size())
    {
        currentGlobalSignal = popMostPrioritizedGlobalSignalOutOfPrioritizedBuffer();
        QString currentGlobalSignalTypeTypeName = currentGlobalSignal.Type.typeName();
        if (currentGlobalSignalTypeTypeName == QStringLiteral("SerialPortWorkerBasis::Data"))
        {
            switch (currentGlobalSignal.Type.toInt()) {
            case requestBytesTransmission:
            {
                anIf(SerialPortWorkerBasisDbgEn, anAck("requestBytesTransmission"));
                SerialPort.write(currentGlobalSignal.Data.toByteArray());
                currentGlobalSignal.TimeStamp = NOW2String;
                currentGlobalSignal.Priority = currentGlobalSignal.Priority + 1;
                if (SerialPort.waitForBytesWritten(300))
                {
                    anIf(SerialPortWorkerBasisDbgEn, anAck("Bytes Written !"));
                    currentGlobalSignal.Type = QVariant::fromValue(BytesWritten);
                    emit readingBytesSerialPortWorker();
                    emit Out(GlobalSignal(currentGlobalSignal));
                    return;
                }
                else
                {
                    anIf(SerialPortWorkerBasisDbgEn, anWarn("Bytes Written Timed Out !"));
                    currentGlobalSignal.Type = QVariant::fromValue(BytesWrittenTimedOut);
                    emit Out(GlobalSignal(currentGlobalSignal));
                }
                break;
            }
            case clearBuffer:
            {
                anIf(SerialPortWorkerBasisDbgEn, anWarn("clearBuffer"));
                clearPrioritizedBuffer();
                break;
            }
            case replyPortName:
            {
                anIf(SerialPortWorkerBasisDbgEn, anWarn("replyPortName"));
                setPortName(currentGlobalSignal.Data.toString());
                break;
            }
            default:
                break;
            }
        }
        else if (currentGlobalSignalTypeTypeName == QStringLiteral("SerialPortWorkerBasis::Warning"))
        {
            anIf(SerialPortWorkerBasisDbgEn,anAck(WarningMetaEnum.valueToKey(static_cast<int>(currentGlobalSignal.Type.toInt()))));
            emit Out(GlobalSignal(currentGlobalSignal));
        }
        else if (currentGlobalSignalTypeTypeName == QStringLiteral("SerialPortWorkerBasis::Notification"))
        {
            anIf(SerialPortWorkerBasisDbgEn,anAck(NotificationMetaEnum.valueToKey(static_cast<int>(currentGlobalSignal.Type.toInt()))));
            emit Out(GlobalSignal(currentGlobalSignal));
        }
    }
    if (prioritizedBuffer.isEmpty())
    {
        emit goIdle();
    }
    else
    {
        emit GlobalSignalExecutionRequested();
    }
}

void SerialPortWorkerBasis::readBytesSerialPortWorkerOnEntry()
{
    readAllDataFromSerialPort();
    emit GlobalSignalExecutionRequested();
}

void SerialPortWorkerBasis::errorSerialPortWorkerOnEntry()
{
    anIf(SerialPortWorkerBasisDbgEn,
         anError("Emit SerialPortWorkerBasis::Error");
         anInfo("ErrorType: " + QString(ErrorMetaEnum.valueToKey(static_cast<int>(ErrorType))));
         anInfo("ErrorInfo: " + ErrorInfo);
    );
    GlobalSignal errorGlobalSignal;
    errorGlobalSignal.Type = QVariant::fromValue(ErrorType);
    errorGlobalSignal.Data = QVariant::fromValue(ErrorInfo);
    errorGlobalSignal.Priority = 200;
    errorGlobalSignal.SignalPriority = 200;
    errorGlobalSignal.DstStrs.append(GlobalSignalCoordinatorObjName);
    emit Out(errorGlobalSignal);
}

void SerialPortWorkerBasis::initiate(const QString &aPortName)
{
    dispose();
    PortName = aPortName;
    if (openSerialPort())
    {
        isInitiated = true;
        GlobalSignal notifySerialPortConnected;
        notifySerialPortConnected.Type = QVariant::fromValue(SerialPortConnected);
        notifySerialPortConnected.DstStrs.append(GlobalSignalCoordinatorObjName);
        pushAGlobalSignalIntoPrioritizedBuffer(notifySerialPortConnected);
        emit goIdle();
    }
    anIf(SerialPortWorkerBasisDbgEn && isInitiated, anAck("SerialPortWorkerBasis Initialized"));
}

void SerialPortWorkerBasis::dispose()
{
    anIf(SerialPortWorkerBasisDbgEn && isInitiated, anWarn("Clean SerialPortWorkerBasis"));
    closeSerialPort();
    PortName.clear();
    clearPrioritizedBuffer();
    clearError();
    clearCache();
    isInitiated = false;
}

void SerialPortWorkerBasis::setError(const SerialPortWorkerBasis::Error &anErrorType, const QString &anErrorInfo)
{
    if (anErrorType != NoError)
    {
        anIf(SerialPortWorkerBasisDbgEn, anError("Error Occurred !"));
        ErrorType = anErrorType;
        ErrorInfo = anErrorInfo;
        emit ErrorOccurred();
    }
}

void SerialPortWorkerBasis::clearError()
{
    anIf(SerialPortWorkerBasisDbgEn && (ErrorType != NoError), anWarn("Clear Error"));
    ErrorType = NoError;
    ErrorInfo.clear();
}

void SerialPortWorkerBasis::setPortName(const QString &newPortName)
{
    if (newPortName != PortName)
    {
        anIf(SerialPortWorkerBasisDbgEn, anWarn("Port Name Changed !");anVar(newPortName));
        dispose();
        PortName = newPortName;
        emit InitiationRequested();
    }
}

void SerialPortWorkerBasis::readAllDataFromSerialPort()
{
    if (SerialPort.waitForReadyRead(2700))
    {
        QByteArray tmpRead(SerialPort.readAll());
        while (SerialPort.waitForReadyRead(300))
        {
            tmpRead += SerialPort.readAll();
        }
        anIf(SerialPortWorkerBasisDbgEn, anAck("Bytes Read !"));
        currentGlobalSignal.Type = QVariant::fromValue(replyBytesWithTimeStamp);
        currentGlobalSignal.Data = QVariant::fromValue(tmpRead);
        currentGlobalSignal.TimeStamp = NOW2String;
    }
    else
    {
        anIf(SerialPortWorkerBasisDbgEn, anWarn("Ready Read Timed Out !"));
        currentGlobalSignal.Type = QVariant::fromValue(ReadyReadTimedOut);
    }
    emit Out(GlobalSignal(currentGlobalSignal));
}

void SerialPortWorkerBasis::In(const GlobalSignal &aGlobalSignal)
{
    if (currentStateName != QStringLiteral("runningSerialPortWorker")
            && aGlobalSignal.Type.typeName() == QStringLiteral("SerialPortWorkerBasis::Data")
            && aGlobalSignal.Type.toInt() == replyPortName)
    {
        setPortName(aGlobalSignal.Data.toString());
    }
    else
    {
        pushAGlobalSignalIntoPrioritizedBuffer(aGlobalSignal);
        if (currentStateName == QStringLiteral("idleSerialPortWorker"))
        {
            emit GlobalSignalExecutionRequested();
        }
    }
}

void SerialPortWorkerBasis::SerialPortErrorOccurred(QSerialPort::SerialPortError error)
{
    if ((error!=QSerialPort::NoError) && (error!=QSerialPort::TimeoutError))
    {
        setError(QSerialPortErrorOccurred,QString(QSerialPortErrorMetaEnum.valueToKey(static_cast<int>(error))));
    }
}

void SerialPortWorkerBasis::configSerialPort()
{
    closeSerialPort();
    SerialPort.setReadBufferSize(64);
    SerialPort.setDataBits(QSerialPort::Data8);
    SerialPort.setBaudRate(QSerialPort::Baud9600);
    SerialPort.setStopBits(QSerialPort::OneStop);
    SerialPort.setParity(QSerialPort::NoParity);
    SerialPort.setFlowControl(QSerialPort::NoFlowControl);
    SerialPort.setPortName(PortName);
    anIf(SerialPortWorkerBasisDbgEn, anAck("SerialPort Configured !"));
}

bool SerialPortWorkerBasis::openSerialPort()
{
    configSerialPort();
    if (SerialPort.open(QIODevice::ReadWrite))
    {
        anIf(SerialPortWorkerBasisDbgEn, anAck("SerialPort Opened !"));
        return true;
    }
    anIf(SerialPortWorkerBasisDbgEn, anWarn("Failed To Open SerialPort !"));
    return false;
}

void SerialPortWorkerBasis::closeSerialPort()
{
    if (SerialPort.isOpen())
    {
        anIf(SerialPortWorkerBasisDbgEn, anWarn("Close SerialPort"));
        SerialPort.close();
    }
}

void SerialPortWorkerBasis::clearCache()
{
    currentGlobalSignal = GlobalSignal();
}

const QMetaEnum SerialPortWorkerBasis::DataMetaEnum = QMetaEnum::fromType<SerialPortWorkerBasis::Data>();
const QMetaEnum SerialPortWorkerBasis::ErrorMetaEnum = QMetaEnum::fromType<SerialPortWorkerBasis::Error>();
const QMetaEnum SerialPortWorkerBasis::WarningMetaEnum = QMetaEnum::fromType<SerialPortWorkerBasis::Warning>();
const QMetaEnum SerialPortWorkerBasis::NotificationMetaEnum = QMetaEnum::fromType<SerialPortWorkerBasis::Notification>();
const QMetaEnum SerialPortWorkerBasis::QSerialPortErrorMetaEnum = QMetaEnum::fromType<QSerialPort::SerialPortError>();
