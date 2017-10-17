#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    uhvpump0(new WindowProtocol(0))
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
    connect(ui->pushButtonHVonoff, &QPushButton::clicked,
            this, [&](){
        anAck("Button " + ui->pushButtonHVonoff->text() + " Clicked !");
        if (ui->pushButtonHVonoff->text() == "HV ON")
        {
            UHVWorkerVarSet::PrioritizedCommandMessage hvOnMsg;
            hvOnMsg.first = ui->spinBoxHVonoff->value();
            hvOnMsg.second = UHVWorkerVarSet::CommandMessage(uhvpump0->HVOnOffCh2().Write().setON().genMSG(),QStringLiteral(""));
            for (quint8 index=0; index<=ui->spinBoxHVonoff_2->value(); ++index)
            {
                emit Out(QVariant::fromValue(UHVWorkerVarSet::addAnUHVPrioritizedCommandMessage),
                         QVariant::fromValue(hvOnMsg));
            }
        }
        else if (ui->pushButtonHVonoff->text() == "HV OFF")
        {
            UHVWorkerVarSet::PrioritizedCommandMessage hvOffMsg;
            hvOffMsg.first = ui->spinBoxHVonoff->value();
            hvOffMsg.second = UHVWorkerVarSet::CommandMessage(uhvpump0->HVOnOffCh2().Write().setOFF().genMSG(),QStringLiteral(""));
            for (quint8 index=0; index<=ui->spinBoxHVonoff_2->value(); ++index)
            {
                emit Out(QVariant::fromValue(UHVWorkerVarSet::addAnUHVPrioritizedCommandMessage),
                         QVariant::fromValue(hvOffMsg));
            }
        }
    });

    connect(ui->pushButtonReadI, &QPushButton::clicked,
            this, [&](){
        UHVWorkerVarSet::PrioritizedCommandMessage readIstatusMsg;
        readIstatusMsg.first = ui->spinBoxReadI->value();
        readIstatusMsg.second = UHVWorkerVarSet::CommandMessage(uhvpump0->IMeasuredCh2().Read().clearDATA().genMSG(),QStringLiteral(""));
        for (quint8 index=0; index<=ui->spinBoxReadI_2->value(); ++index)
        {
            emit Out(QVariant::fromValue(UHVWorkerVarSet::addAnUHVPrioritizedCommandMessage),
                     QVariant::fromValue(readIstatusMsg));
        }
    });

    connect(ui->pushButtonReadV, &QPushButton::clicked,
            this, [&](){
        UHVWorkerVarSet::PrioritizedCommandMessage ReadVstatusMsg;
        ReadVstatusMsg.first = ui->spinBoxReadV->value();
        ReadVstatusMsg.second = UHVWorkerVarSet::CommandMessage(uhvpump0->VMeasuredCh2().Read().clearDATA().genMSG(),QStringLiteral(""));
        for (quint8 index=0; index<=ui->spinBoxReadV_2->value(); ++index)
        {
            emit Out(QVariant::fromValue(UHVWorkerVarSet::addAnUHVPrioritizedCommandMessage),
                     QVariant::fromValue(ReadVstatusMsg));
        }
    });

    connect(ui->pushButtonReadP, &QPushButton::clicked,
            this, [&](){
        UHVWorkerVarSet::PrioritizedCommandMessage ReadPstatusMsg;
        ReadPstatusMsg.first = ui->spinBoxReadP->value();
        ReadPstatusMsg.second = UHVWorkerVarSet::CommandMessage(uhvpump0->PMeasuredCh2().Read().clearDATA().genMSG(),QStringLiteral(""));
        for (quint8 index=0; index<=ui->spinBoxReadP_2->value(); ++index)
        {
            emit Out(QVariant::fromValue(UHVWorkerVarSet::addAnUHVPrioritizedCommandMessage),
                     QVariant::fromValue(ReadPstatusMsg));
        }
    });

    connect(anUHVWorker, &UHVWorker::Out, this, &MainWindow::In);

    connect(ui->pushButtonClearBuffer, &QPushButton::clicked,
            this, [&](){
        emit Out(QVariant::fromValue(UHVWorkerVarSet::clearPendingMessageList));
    });
    connect(ui->pushButton, &QPushButton::clicked, ui->pushButtonReadI, &QPushButton::click);
    connect(ui->pushButton, &QPushButton::clicked, ui->pushButtonReadV, &QPushButton::click);
    connect(ui->pushButton, &QPushButton::clicked, ui->pushButtonReadP, &QPushButton::click);

    foreach (QSerialPortInfo currentScan, QSerialPortInfo::availablePorts())
    {
        ui->comboBoxSerialPort->addItem(currentScan.portName());
    }

    aNewThread->start();
}

MainWindow::~MainWindow()
{
    if (uhvpump0)
    {
        delete uhvpump0;
        uhvpump0=Q_NULLPTR;
    }
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
            if (coreRepMsg.size() > 7)
            {
                WindowProtocol & tmpUHV = WindowProtocol::fromQByteArray(coreRepMsg);
                if (tmpUHV.getCOM()==0x30)//RD
                {
                    if (tmpUHV.getDATA().size()==0)
                    {
                        tmpUHV.setCMDFlag(true);
                    }
                    else
                    {
                        tmpUHV.setCMDFlag(false);
                    }
                }
                else if (tmpUHV.getCOM()==0x31)//WR
                {
                    tmpUHV.setCMDFlag(true);
                }
                else
                {
                    tmpUHV.setCMDFlag(false);
                }
                if (!tmpUHV.isCMDFlagSet())
                {
                    anInfo("Read: " << tmpUHV.getMSGMean());
                    updateSENDlabel("",ui->labelSentMsg->text(),ui->labelSentMessage->text());
                    updateREADlabel("QLabel { background-color : green; color : yellow; }",tmpUHV.getMSG().toHex(),tmpUHV.getMSGMean());
                    if (ui->labelSentMessage->text().contains("Off", Qt::CaseInsensitive)
                            && ui->labelSentMessage->text().contains("HVOnOff", Qt::CaseInsensitive))
                        ui->pushButtonHVonoff->setText("HV ON");
                }
                else
                {
                    anInfo("Sent: " << tmpUHV.getMSGMean());
                    updateREADlabel("",ui->labelReadMsg->text(),ui->labelReadMessage->text());
                    updateSENDlabel("QLabel { background-color : green; color : yellow; }",tmpUHV.getMSG().toHex(),tmpUHV.getMSGMean());
                }
            }
            else
            {
                WindowProtocol & tmpUHV = WindowProtocol::fromQByteArray(coreRepMsg);
                QString tmpMsgMean = tmpUHV.getMSGMean();
                anInfo("Read: " << coreRepMsg.toHex());
                updateSENDlabel("",ui->labelSentMsg->text(),ui->labelSentMessage->text());
                updateREADlabel("QLabel { background-color : green; color : yellow; }",coreRepMsg.toHex(),tmpMsgMean);
                if ((tmpMsgMean.contains("ACK", Qt::CaseInsensitive)) && ui->labelSentMessage->text().contains("HVOnOff", Qt::CaseInsensitive))
                {
                    if (ui->labelSentMessage->text().contains(" On", Qt::CaseInsensitive))
                    {
                        ui->pushButtonHVonoff->setText("HV OFF");
                    }
                    else if (ui->labelSentMessage->text().contains(" Off", Qt::CaseInsensitive))
                    {
                        ui->pushButtonHVonoff->setText("HV ON");
                    }
                }
            }
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
            WindowProtocol & tmpUHV = WindowProtocol::fromQByteArray(newRepMsg.second.first);
            updateREADlabel("",ui->labelReadMsg->text(),ui->labelReadMessage->text());
            updateSENDlabel("QLabel { background-color : green; color : yellow; }",tmpUHV.getMSG().toHex(),tmpUHV.getMSGMean());
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
