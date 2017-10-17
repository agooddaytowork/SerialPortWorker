#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    uhv2pump0(new BinaryProtocol(0))
{
    ui->setupUi(this);

    QThread *aNewThread = new QThread();
    SerialPortWorker *aSerialPortWorker = new SerialPortWorker();
    aSerialPortWorker->moveToThread(aNewThread);
    connect(aNewThread, &QThread::started, aSerialPortWorker, &SerialPortWorker::start);
    connect(this, &MainWindow::Out, aSerialPortWorker, &SerialPortWorker::In);
    connect(aSerialPortWorker, &SerialPortWorker::Out, this, &MainWindow::In);


    connect(ui->pushButtonConnect, &QPushButton::clicked,
            this, [&](){
        anAck("Button " + ui->pushButtonConnect->text() + " Clicked !");
        if (ui->pushButtonConnect->text() == "Connect")
        {
            GlobalSignal givePortName;
            givePortName.Type = QVariant::fromValue(SerialPortWorkerProperty::replyPortName);
            givePortName.Data = QVariant::fromValue(ui->comboBoxSerialPort->currentText());
            emit Out(givePortName);
        }
        else if (ui->pushButtonConnect->text() == "Disconnect")
        {
            GlobalSignal disconnectSerial;
            disconnectSerial.Type = QVariant::fromValue(SerialPortWorkerProperty::disconnectSerialPort);
            emit Out(disconnectSerial);
        }
        ui->pushButtonConnect->setText("Please Wait ...");
    });
    connect(ui->pushButtonHVonoff, &QPushButton::clicked,
            this, [&](){
        anAck("Button " + ui->pushButtonHVonoff->text() + " Clicked !");
        GlobalSignal hvOnMsg;
        hvOnMsg.Type = QVariant::fromValue(SerialPortWorkerProperty::addAGlobalSignal);
        hvOnMsg.Priority = ui->spinBoxHVonoff->value();
        if (ui->pushButtonHVonoff->text() == "HV ON")
        {
            hvOnMsg.Data = QVariant::fromValue(SerialPortWorkerProperty::DataMessage(uhv2pump0->HdrCmd().HVSwitch().Ch1().On().GenMsg(),QStringLiteral("")));
        }
        else if (ui->pushButtonHVonoff->text() == "HV OFF")
        {
            hvOnMsg.Data = QVariant::fromValue(SerialPortWorkerProperty::DataMessage(uhv2pump0->HdrCmd().HVSwitch().Ch1().Off().GenMsg(),QStringLiteral("")));
        }
        for (quint8 index=0; index<=ui->spinBoxHVonoff_2->value(); ++index)
        {
            emit Out(hvOnMsg);
        }
    });

    connect(ui->pushButtonReadI, &QPushButton::clicked,
            this, [&](){
        GlobalSignal readIstatusMsg;
        readIstatusMsg.Type = QVariant::fromValue(SerialPortWorkerProperty::addAGlobalSignal);
        readIstatusMsg.Priority = ui->spinBoxReadI->value();
        readIstatusMsg.Data = QVariant::fromValue(SerialPortWorkerProperty::DataMessage(uhv2pump0->HdrCmd().ReadI().Ch1().GenMsg(),QStringLiteral("")));
        for (quint8 index=0; index<=ui->spinBoxReadI_2->value(); ++index)
        {
            emit Out(readIstatusMsg);
        }
    });

    connect(ui->pushButtonReadV, &QPushButton::clicked,
            this, [&](){
        GlobalSignal ReadVstatusMsg;
        ReadVstatusMsg.Type = QVariant::fromValue(SerialPortWorkerProperty::addAGlobalSignal);
        ReadVstatusMsg.Priority = ui->spinBoxReadV->value();
        ReadVstatusMsg.Data = QVariant::fromValue(SerialPortWorkerProperty::DataMessage(uhv2pump0->HdrCmd().ReadV().Ch1().GenMsg(),QStringLiteral("")));
        for (quint8 index=0; index<=ui->spinBoxReadV_2->value(); ++index)
        {
            emit Out(ReadVstatusMsg);
        }
    });

    connect(ui->pushButtonReadP, &QPushButton::clicked,
            this, [&](){
        GlobalSignal ReadPstatusMsg;
        ReadPstatusMsg.Type = QVariant::fromValue(SerialPortWorkerProperty::addAGlobalSignal);
        ReadPstatusMsg.Priority = ui->spinBoxReadP->value();
        ReadPstatusMsg.Data = QVariant::fromValue(SerialPortWorkerProperty::DataMessage(uhv2pump0->HdrCmd().ReadP().Ch1().GenMsg(),QStringLiteral("")));
        for (quint8 index=0; index<=ui->spinBoxReadP_2->value(); ++index)
        {
            emit Out(ReadPstatusMsg);
        }
    });

    connect(ui->pushButtonClearBuffer, &QPushButton::clicked,
            this, [&](){
        GlobalSignal clearBufferMsg;
        clearBufferMsg.Type = QVariant::fromValue(SerialPortWorkerProperty::clearBuffer);
        emit Out(clearBufferMsg);
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
    if (uhv2pump0)
    {
        delete uhv2pump0;
        uhv2pump0=Q_NULLPTR;
    }
    delete ui;
}

void MainWindow::In(const GlobalSignal &aGlobalSignal)
{
    anTrk("MainWindow Receives A Signal");
    QString enumVarTypeName(aGlobalSignal.Type.typeName());
    if (enumVarTypeName == QStringLiteral("SerialPortWorkerProperty::Data"))
    {
        switch (aGlobalSignal.Type.toInt()) {
        case SerialPortWorkerProperty::replyAGlobalSignal:
        {
            anInfo("replyAGlobalSignal");
            SerialPortWorkerProperty::DataMessage newRepMsg
                    = aGlobalSignal.Data.value<SerialPortWorkerProperty::DataMessage>();
            QByteArray coreRepMsg = newRepMsg.first;
            if (coreRepMsg.size() > 7)
            {
                BinaryProtocol & tmpUHV2 = BinaryProtocol::fromQByteArray(coreRepMsg);
                if (tmpUHV2.GetMessageDirection() == "From")
                {
                    anInfo("Read: " << tmpUHV2.GetMessageTranslation());
                    updateSENDlabel("",ui->labelSentMsg->text(),ui->labelSentMessage->text());
                    updateREADlabel("QLabel { background-color : green; color : yellow; }",tmpUHV2.GetMsg().toHex(),tmpUHV2.GetMessageTranslation());
                    if (ui->labelSentMessage->text().contains("Off", Qt::CaseInsensitive)
                            && ui->labelSentMessage->text().contains("HVSwitch", Qt::CaseInsensitive))
                        ui->pushButtonHVonoff->setText("HV ON");
                }
            }
            else
            {
                anInfo("Read: " << coreRepMsg.toHex());
                updateSENDlabel("",ui->labelSentMsg->text(),ui->labelSentMessage->text());
                updateREADlabel("QLabel { background-color : green; color : yellow; }",coreRepMsg.toHex(),"");
                if ((QString(coreRepMsg.toHex()) == "06") && ui->labelSentMessage->text().contains("HVSwitch", Qt::CaseInsensitive))
                {
                    if (ui->labelSentMessage->text().contains("On", Qt::CaseInsensitive))
                    {
                        ui->pushButtonHVonoff->setText("HV OFF");
                    }
                    else if (ui->labelSentMessage->text().contains("Off", Qt::CaseInsensitive))
                    {
                        ui->pushButtonHVonoff->setText("HV ON");
                    }
                    ui->labelReadMessage->setText("Acknowledged");
                }
            }
        break;
        }
        default:
            break;
        }
    }
    else if (enumVarTypeName == QStringLiteral("SerialPortWorkerProperty::Error"))
    {
        switch (aGlobalSignal.Type.toInt()) {
        case SerialPortWorkerProperty::SerialPortError:
        {
            anError("SerialPortError: " << aGlobalSignal.Data.toString());
            break;
        }
        default:
            break;
        }
    }
    else if (enumVarTypeName == QStringLiteral("SerialPortWorkerProperty::Warning"))
    {
        switch (aGlobalSignal.Type.toInt()) {
        case SerialPortWorkerProperty::ReadyReadTimedOut:
        {
            anInfo("MessageReadTimedOut");
            updateSENDlabel("",ui->labelSentMsg->text(),ui->labelSentMessage->text());
            updateREADlabel("QLabel { background-color : gray; color : red; }","","Timed Out To Read !");
            break;
        }
        case SerialPortWorkerProperty::BytesWrittenTimedOut:
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
    else if (enumVarTypeName == QStringLiteral("SerialPortWorkerProperty::Notification"))
    {
        switch (aGlobalSignal.Type.toInt()) {
        case SerialPortWorkerProperty::SerialPortConnected:
        {
            anInfo("SerialPortConnected");
            ui->pushButtonConnect->setText("Disconnect");
            break;
        }
        case SerialPortWorkerProperty::SerialPortDisconnected:
        {
            anInfo("SerialPortDisconnected");
            ui->pushButtonConnect->setText("Connect");
            break;
        }
        case SerialPortWorkerProperty::BytesWritten:
        {
            anInfo("BytesWritten");
            SerialPortWorkerProperty::DataMessage newRepMsg
                    = aGlobalSignal.Data.value<SerialPortWorkerProperty::DataMessage>();
            BinaryProtocol & tmpUHV2 = BinaryProtocol::fromQByteArray(newRepMsg.first);
            if (tmpUHV2.GetMessageDirection() == "To")
            {
                anInfo("Sent: " << tmpUHV2.GetMessageTranslation());
                updateREADlabel("",ui->labelReadMsg->text(),ui->labelReadMessage->text());
                updateSENDlabel("QLabel { background-color : green; color : yellow; }",tmpUHV2.GetMsg().toHex(),tmpUHV2.GetMessageTranslation());
            }
            break;
        }
        case SerialPortWorkerProperty::BufferCleared:
        {
            anInfo("BufferCleared");
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
