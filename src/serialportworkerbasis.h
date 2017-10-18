#ifndef SERIALPORTWORKERBASIS_H
#define SERIALPORTWORKERBASIS_H

#define SerialPortWorkerBasisDbgEn 1

#include "shared/abstractstatemachinebasis.h"
#include <QSerialPortInfo>
#include <QSerialPort>

class SerialPortWorkerBasis : public AbstractStateMachineBasis
{
    Q_OBJECT
public:
    explicit SerialPortWorkerBasis(QObject *parent = nullptr);
    ~SerialPortWorkerBasis();

    enum Data
    {
        NoData = 0,
        requestPortName,
        replyPortName,
        clearBuffer,
        requestBytesTransmission,
        replyBytesWithTimeStamp
    };
    Q_ENUM(Data)

    enum Error
    {
        NoError = 0,
        QSerialPortErrorOccurred
    };
    Q_ENUM(Error)

    enum Warning
    {
        NoWarning = 0,
        BytesWrittenTimedOut,
        ReadyReadTimedOut,
    };
    Q_ENUM(Warning)

    enum Notification
    {
        NoNotification = 0,
        SerialPortDisconnected,
        SerialPortConnected,
        BytesWritten,
        readyToWork
    };
    Q_ENUM(Notification)

    static const QMetaEnum DataMetaEnum;
    static const QMetaEnum ErrorMetaEnum;
    static const QMetaEnum WarningMetaEnum;
    static const QMetaEnum NotificationMetaEnum;
    static const QMetaEnum QSerialPortErrorMetaEnum;

    QString PortName;
    QSerialPort SerialPort;
    Error ErrorType = NoError;
    QString ErrorInfo;

    void initialize(const QString &aPortName);
    void dispose();
    void setError(const Error & anErrorType, const QString & anErrorInfo);
    void clearError();
    void setPortName(const QString &newPortName);
    void emitRequestPortName();
    void queueNotificationReadyToWork();
    void executePrioritizedBuffer();
    void readAllDataFromSerialPort();
    void emitErrorGlobalSignal();
signals:
    void readingBytesSerialPortWorker();
public slots:
    void In(const GlobalSignal &aGlobalSignal);
    void SerialPortErrorOccurred(QSerialPort::SerialPortError error);
private:
    void configSerialPort();
    bool openSerialPort();
    void closeSerialPort();
    void clearCache();    
};

#endif // SERIALPORTWORKERBASIS_H
