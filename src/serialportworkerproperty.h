#ifndef SERIALPORTWORKERPROPERTY_H
#define SERIALPORTWORKERPROPERTY_H

#define SerialPortWorkerPropertyDbgEn 1

#include <QCoreApplication>
#include <QStateMachine>
#include <QObject>
#include <QPair>
#include <QMap>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMetaType>
#include <QMetaEnum>
#include "anLogger/src/anlogger.h"
#include "shared/commonthings.h"

class SerialPortWorkerProperty : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortWorkerProperty(QObject *parent = nullptr);
    ~SerialPortWorkerProperty();
    enum Data
    {
        NoData = 0,
        requestPortName,
        replyPortName,
        clearBuffer,
        disconnectSerialPort,
        restartSerialPort,
        addAGlobalSignal,
        replyAGlobalSignal
    };
    Q_ENUM(Data)

    enum Error
    {
        NoError = 0,
        SerialPortError
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
        BufferCleared,
        BytesWritten,
        readyToWork
    };
    Q_ENUM(Notification)

    typedef QPair<QByteArray,QString> DataMessage;

    QString PortName;
    QSerialPort * SerialPort = Q_NULLPTR;
    QMap<qint16,QList<GlobalSignal>> prioritizedBuffer;
    GlobalSignal currentGlobalSignal;
    Error ErrorType = NoError;
    QString ErrorInfo;

    void initialize();
    void dispose();
    void configSerialPort();
    void deleteSerialPort();
    void setError(const Error & anErrorType, const QString & anErrorInfo);
    void clearError();
    void clearEmptyList();
    void clearPrioritizedBuffer();
    void addOneGlobalSignal(const GlobalSignal &aGlobalSignal);

    static const QMetaEnum DataMetaEnum;
    static const QMetaEnum ErrorMetaEnum;
    static const QMetaEnum WarningMetaEnum;
    static const QMetaEnum NotificationMetaEnum;
    static const QMetaEnum QSerialPortErrorMetaEnum;
    static const Qt::ConnectionType uniqueQtConnectionType = static_cast<Qt::ConnectionType>(Qt::AutoConnection | Qt::UniqueConnection);

signals:
    void requestDirectTransitionForSerialPortWorkerState(const QString &);
    void ErrorOccurred();
    void PortNameChanged();
    void restartSerialPortConnection();
    void firstGlobalSignalAdded();
    void oneGlobalSignalAdded();
    void Out(const GlobalSignal &);
public slots:
    void SerialPortErrorOccurred(QSerialPort::SerialPortError error);
};
Q_DECLARE_METATYPE(SerialPortWorkerProperty::DataMessage)
Q_DECLARE_METATYPE(QSerialPort::SerialPortError)
#endif // SERIALPORTWORKERPROPERTY_H
