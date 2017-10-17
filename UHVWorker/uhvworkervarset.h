#ifndef UHVWORKERVARSET_H
#define UHVWORKERVARSET_H

#define UHVWorkerVarSetDbgEn 1

#include <QCoreApplication>
#include <QObject>
#include <QPair>
#include <QThread>
#include <QMap>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStateMachine>
#include <QMetaType>
#include "anlogger.h"

class UHVWorkerVarSet: public QObject
{
    Q_OBJECT
public:
    explicit UHVWorkerVarSet(QObject * parent = Q_NULLPTR);
    ~UHVWorkerVarSet();

    enum Data
    {
        NoData = 0,
        requestPortName,
        replyPortName,
        clearPendingMessageList,
        disconnectSerialPort,
        restartSerialPort,
        addAnUHVPrioritizedCommandMessage,
        replyUHVPrioritizedCommandMessage
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
        pendingMessageListCleared,
        MessageTransmitted
    };
    Q_ENUM(Notification)

    typedef QPair<QByteArray,QString> CommandMessage;
    typedef QPair<qint8,CommandMessage> PrioritizedCommandMessage;

    QString PortName;
    QSerialPort * SerialPort = Q_NULLPTR;
    QMap<qint8,QList<CommandMessage>*> pendingMessageList;
    qint8 lastTransmittedMessagePriority=0;
    CommandMessage lastTransmittedMessage;
    CommandMessage lastReceivedMessage;
    Error ErrorType = NoError;
    QString ErrorInfo;

    void initialize();
    void dispose();
    void configSerialPort();
    void deleteSerialPort();
    void setError(const Error & anErrorType, const QString & anErrorInfo);
    void clearError();
    void addOneUHVPrioritizedCommandMessage(const UHVWorkerVarSet::PrioritizedCommandMessage &newCmdMsg);

    static const QMetaEnum DataMetaEnum;
    static const QMetaEnum ErrorMetaEnum;
    static const QMetaEnum WarningMetaEnum;
    static const QMetaEnum NotificationMetaEnum;
    static const QMetaEnum QSerialPortErrorMetaEnum;
    static const Qt::ConnectionType uniqueQtConnectionType = static_cast<Qt::ConnectionType>(Qt::AutoConnection | Qt::UniqueConnection);

public slots:
    void SerialPortErrorOccurred(QSerialPort::SerialPortError error);
signals:
    void DirectStateTransitionRequest(const QString &);
    void ErrorOccurred();
    void PortNameChanged();
    void restartSerialPortConnection();
    void AFirstPrioritizedCommandMessageReceived();
    void Out(QVariant , QVariant = QVariant());
};
Q_DECLARE_METATYPE(QSerialPort::SerialPortError)
Q_DECLARE_METATYPE(UHVWorkerVarSet::PrioritizedCommandMessage)
#endif // UHVWORKERVARSET_H
