#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QThread *aNewThread = new QThread();
    UHVWorker * anUHVWorker = new UHVWorker();
    anUHVWorker->moveToThread(aNewThread);
    connect(aNewThread, &QThread::started, anUHVWorker, &UHVWorker::start);
    connect(this, &MainWindow::Out, anUHVWorker, &UHVWorker::In);
    connect(ui->pushButtonConnect, &QPushButton::clicked,
            this, [&](){
        anAck("Button " + ui->pushButtonConnect->text() + " Clicked !");
        if (ui->pushButtonConnect->text() == "Connect")
        {
            emit Out(QVariant::fromValue(UHVWorkerVarSet::replyPortName),
                     QVariant::fromValue(ui->comboBoxSerialPort->currentText()));
        }
        else if (ui->pushButtonConnect->text() == "Disconnect")
        {
            emit Out(QVariant::fromValue(UHVWorkerVarSet::disconnectSerialPort));
        }
        ui->pushButtonConnect->setText("Please Wait ...");
    });


    connect(anUHVWorker, &UHVWorker::Out, this, &MainWindow::In);

    connect(ui->pushButtonClearBuffer, &QPushButton::clicked,
            this, [&](){
        emit Out(QVariant::fromValue(UHVWorkerVarSet::clearPendingMessageList));
    });

    foreach (QSerialPortInfo currentScan, QSerialPortInfo::availablePorts())
    {
        ui->comboBoxSerialPort->addItem(currentScan.portName());
    }

    aNewThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::In(QVariant enumVar, QVariant dataVar)
{    
    QString enumVarTypeName(enumVar.typeName());
    anAck("Signal-From-" << enumVarTypeName << " Received !");
    if (enumVarTypeName == QStringLiteral("UHVWorkerVarSet::Data"))
    {
        switch (enumVar.toInt()) {
        case UHVWorkerVarSet::replyUHVPrioritizedCommandMessage:
        {
            anInfo("replyUHVPrioritizedCommandMessage");
            UHVWorkerVarSet::PrioritizedCommandMessage newRepMsg
                    = dataVar.value<UHVWorkerVarSet::PrioritizedCommandMessage>();
            QByteArray coreRepMsg = newRepMsg.second.first;
            anInfo("Read: " << coreRepMsg.toHex());
            updateSENDlabel("",ui->labelSentMsg->text(),ui->labelSentMessage->text());
            updateREADlabel("QLabel { background-color : green; color : yellow; }",coreRepMsg.toHex(),coreRepMsg);
        break;
        }
        default:
            break;
        }
    }
    else if (enumVarTypeName == QStringLiteral("UHVWorkerVarSet::Error"))
    {
        switch (enumVar.toInt()) {
        case UHVWorkerVarSet::SerialPortError:
        {
            anError("SerialPortError: " << dataVar.value<QString>());
            break;
        }
        default:
            break;
        }
    }
    else if (enumVarTypeName == QStringLiteral("UHVWorkerVarSet::Warning"))
    {
        switch (enumVar.toInt()) {
        case UHVWorkerVarSet::ReadyReadTimedOut:
        {
            anInfo("MessageReadTimedOut");
            updateSENDlabel("",ui->labelSentMsg->text(),ui->labelSentMessage->text());
            updateREADlabel("QLabel { background-color : gray; color : red; }","","Timed Out To Read !");
            break;
        }
        case UHVWorkerVarSet::BytesWrittenTimedOut:
        {
            anInfo("BytesWrittenTimedOut");
            updateSENDlabel("QLabel { background-color : gray; color : red; }","","Timed Out To Send !");
            updateREADlabel("",ui->labelReadMsg->text(),ui->labelReadMessage->text());
            break;
        }
        default:
            break;
        }
    }
    else if (enumVarTypeName == QStringLiteral("UHVWorkerVarSet::Notification"))
    {
        switch (enumVar.toInt()) {
        case UHVWorkerVarSet::SerialPortConnected:
        {
            anInfo("SerialPortConnected");
            ui->pushButtonConnect->setText("Disconnect");
            break;
        }
        case UHVWorkerVarSet::SerialPortDisconnected:
        {
            anInfo("SerialPortDisconnected");
            ui->pushButtonConnect->setText("Connect");
            break;
        }
        case UHVWorkerVarSet::MessageTransmitted:
        {
            anInfo("MessageTransmitted");
            UHVWorkerVarSet::PrioritizedCommandMessage newRepMsg
                    = dataVar.value<UHVWorkerVarSet::PrioritizedCommandMessage>();
            anInfo("Sent: " << newRepMsg.second.first.toHex());
            updateREADlabel("",ui->labelReadMsg->text(),ui->labelReadMessage->text());
            updateSENDlabel("QLabel { background-color : green; color : yellow; }",newRepMsg.second.first.toHex(),newRepMsg.second.first);

            break;
        }
        case UHVWorkerVarSet::pendingMessageListCleared:
        {
            anInfo("pendingMessageListCleared");
            break;
        }
        default:
            break;
        }
    }
}

void MainWindow::updateSENDlabel(const QString &SENDstyleSheet, const QString &SentMsgStr, const QString &SentMessageStr)
{
    ui->labelSEND->setStyleSheet(SENDstyleSheet);
    ui->labelSentMsg->setStyleSheet(SENDstyleSheet);
    ui->labelSentMsg->setText(SentMsgStr);
    ui->labelSentMessage->setStyleSheet(SENDstyleSheet);
    ui->labelSentMessage->setText(SentMessageStr);
    ui->labelSEND->update();
    ui->labelSentMsg->update();
    ui->labelSentMessage->update();
}

void MainWindow::updateREADlabel(const QString &READstyleSheet, const QString &ReadMsgStr, const QString &ReadMessageStr)
{
    ui->labelREAD->setStyleSheet(READstyleSheet);
    ui->labelReadMsg->setStyleSheet(READstyleSheet);
    ui->labelReadMsg->setText(ReadMsgStr);
    ui->labelReadMessage->setStyleSheet(READstyleSheet);
    ui->labelReadMessage->setText(ReadMessageStr);
    ui->labelREAD->update();
    ui->labelReadMsg->update();
    ui->labelReadMessage->update();
}

void MainWindow::on_pushButton_clicked()
{
    QByteArray randomQByteArray;
    for (quint8 i=1; i<3;++i)
    {
        randomQByteArray.append(QByteArray::number(qrand(),16));
    }
    anAck("Send Random Message ...");
    anInfo("hex: " << randomQByteArray.toHex());
    UHVWorkerVarSet::PrioritizedCommandMessage randomMsg;
    randomMsg.first = 0;
    randomMsg.second = UHVWorkerVarSet::CommandMessage(randomQByteArray,QStringLiteral(""));
    emit Out(QVariant::fromValue(UHVWorkerVarSet::addAnUHVPrioritizedCommandMessage),
             QVariant::fromValue(randomMsg));
}
