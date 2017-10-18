#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    uhv2pump0.SetBPNo(0);
    uhv4pump0.setWPNo(0);

    QObject::connect(ui->pushButton_UHVType, &QPushButton::clicked, this, [&](){
        emit killSerialPortThread();
        QThread *aNewThread = new QThread();
        SerialPortWorker *aSerialPortWorker = new SerialPortWorker();
        aSerialPortWorker->setObjectName(UHV2WorkerObjName);
        aSerialPortWorker->moveToThread(aNewThread);

        QObject::connect(aNewThread, &QThread::started, aSerialPortWorker, &SerialPortWorker::start);
        QObject::connect(this, &MainWindow::killSerialPortThread, aSerialPortWorker, &SerialPortWorker::stop);
        QObject::connect(aSerialPortWorker, &SerialPortWorker::stopped, aNewThread, &QThread::quit);
        QObject::connect(aNewThread, &QThread::finished, aSerialPortWorker, &SerialPortWorker::deleteLater);
        QObject::connect(aNewThread, &QThread::finished, aNewThread, &QThread::deleteLater);
        QObject::connect(this, &MainWindow::Out, aSerialPortWorker, &SerialPortWorker::In);
        QObject::connect(aSerialPortWorker, &SerialPortWorker::Out, this, &MainWindow::In);

        if (ui->pushButton_UHVType->text() == QStringLiteral("UHV2"))
        {
            isUHV2 = false;
            aSerialPortWorker->setObjectName(UHV4WorkerObjName);
            ui->pushButton_UHVType->setText(QStringLiteral("UHV4"));
        }
        else
        {
            isUHV2 = true;
            aSerialPortWorker->setObjectName(UHV2WorkerObjName);
            ui->pushButton_UHVType->setText(QStringLiteral("UHV2"));
        }
        aNewThread->start();
    });

    QObject::connect(ui->pushButton_CustomTest, &QPushButton::clicked, ui->pushButtonReadP, &QPushButton::click);
    QObject::connect(ui->pushButton_CustomTest, &QPushButton::clicked, ui->pushButtonReadV, &QPushButton::click);
    QObject::connect(ui->pushButton_CustomTest, &QPushButton::clicked, ui->pushButtonReadI, &QPushButton::click);

    foreach (QSerialPortInfo currentScan, QSerialPortInfo::availablePorts())
    {
        ui->comboBoxSerialPort->addItem(currentScan.portName());
    }    

    ui->pushButton_UHVType->animateClick();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::In(const GlobalSignal &aGlobalSignal)
{
    ui->statusBar->clearMessage();
    QString aGlobalSignalTypeTypeName = aGlobalSignal.Type.typeName();
    if (aGlobalSignalTypeTypeName == QStringLiteral("SerialPortWorkerBasis::Data"))
    {
        switch (aGlobalSignal.Type.toInt()) {
        case SerialPortWorkerBasis::requestPortName:
        {
            ui->statusBar->showMessage(aGlobalSignal.Data.toString() + " requires a PortName to run !");
            break;
        }
        case SerialPortWorkerBasis::replyBytesWithTimeStamp:
        {
            QByteArray coreRepMsg = aGlobalSignal.Data.toByteArray();
            if (isUHV2)
            {
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
            }
            else
            {
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
            }
            break;
        }
        default:
            break;
        }
    }
    else if (aGlobalSignalTypeTypeName == QStringLiteral("SerialPortWorkerBasis::Error"))
    {
        switch (aGlobalSignal.Type.toInt()) {
        case SerialPortWorkerBasis::QSerialPortErrorOccurred:
        {
            ui->statusBar->showMessage(QStringLiteral("QSerialPortErrorOccurred: ") + aGlobalSignal.Data.toString());
            anError("QSerialPortErrorOccurred: " << aGlobalSignal.Data.toString());
            break;
        }
        default:
            break;
        }
    }
    else if (aGlobalSignalTypeTypeName == QStringLiteral("SerialPortWorkerBasis::Warning"))
    {
        switch (aGlobalSignal.Type.toInt()) {
        case SerialPortWorkerBasis::ReadyReadTimedOut:
        {
            updateSENDlabel("",ui->labelSentMsg->text(),ui->labelSentMessage->text());
            updateREADlabel("QLabel { background-color : gray; color : red; }","","Timed Out To Read !");
            break;
        }
        case SerialPortWorkerBasis::BytesWrittenTimedOut:
        {
            updateSENDlabel("QLabel { background-color : gray; color : red; }","","Timed Out To Send !");
            updateREADlabel("",ui->labelReadMsg->text(),ui->labelReadMessage->text());
            break;
        }
        default:
            break;
        }
    }
    else if (aGlobalSignalTypeTypeName == QStringLiteral("SerialPortWorkerBasis::Notification"))
    {
        switch (aGlobalSignal.Type.toInt()) {
        case SerialPortWorkerBasis::readyToWork:
        {
            ui->statusBar->showMessage(aGlobalSignal.Data.toString() + " Is Ready To Work !");
            break;
        }
        case SerialPortWorkerBasis::SerialPortConnected:
        {
            ui->pushButton_UHVType->setEnabled(false);
            ui->pushButtonConnect->setText("Disconnect");
            break;
        }
        case SerialPortWorkerBasis::SerialPortDisconnected:
        {
            ui->pushButton_UHVType->setEnabled(true);
            ui->pushButtonConnect->setText("Connect");
            break;
        }
        case SerialPortWorkerBasis::BytesWritten:
        {
            QByteArray coreRepMsg = aGlobalSignal.Data.toByteArray();
            if (isUHV2)
            {
                BinaryProtocol & tmpUHV2 = BinaryProtocol::fromQByteArray(coreRepMsg);
                if (tmpUHV2.GetMessageDirection() == "To")
                {
                    anInfo("Sent: " << tmpUHV2.GetMessageTranslation());
                    updateREADlabel("",ui->labelReadMsg->text(),ui->labelReadMessage->text());
                    updateSENDlabel("QLabel { background-color : green; color : yellow; }",tmpUHV2.GetMsg().toHex(),tmpUHV2.GetMessageTranslation());
                }
            }
            else
            {
                WindowProtocol & tmpUHV = WindowProtocol::fromQByteArray(coreRepMsg);
                updateREADlabel("",ui->labelReadMsg->text(),ui->labelReadMessage->text());
                updateSENDlabel("QLabel { background-color : green; color : yellow; }",tmpUHV.getMSG().toHex(),tmpUHV.getMSGMean());
            }
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

void MainWindow::on_pushButtonHVonoff_clicked()
{
    GlobalSignal hvOnMsg;
    hvOnMsg.Type = QVariant::fromValue(SerialPortWorkerBasis::requestBytesTransmission);
    hvOnMsg.Priority = ui->spinBoxHVonoff->value();
    if (ui->pushButtonHVonoff->text() == "HV ON")
    {
        hvOnMsg.Data = QVariant::fromValue(isUHV2?uhv2pump0.HdrCmd().HVSwitch().Ch1().On().GenMsg():uhv4pump0.HVOnOffCh1().Write().setON().genMSG());
    }
    else if (ui->pushButtonHVonoff->text() == "HV OFF")
    {
        hvOnMsg.Data = QVariant::fromValue(isUHV2?uhv2pump0.HdrCmd().HVSwitch().Ch1().Off().GenMsg():uhv4pump0.HVOnOffCh1().Write().setOFF().genMSG());
    }
    for (quint8 index=0; index<=ui->spinBoxHVonoff_2->value(); ++index)
    {
        emit Out(hvOnMsg);
    }
}

void MainWindow::on_pushButtonReadV_clicked()
{
    GlobalSignal readVstatusMsg;
    readVstatusMsg.Type = QVariant::fromValue(SerialPortWorkerBasis::requestBytesTransmission);
    readVstatusMsg.Priority = ui->spinBoxReadV->value();
    readVstatusMsg.Data = QVariant::fromValue(isUHV2?uhv2pump0.HdrCmd().ReadV().Ch1().GenMsg():uhv4pump0.VMeasuredCh1().Read().clearDATA().genMSG());
    for (quint8 index=0; index<=ui->spinBoxReadV_2->value(); ++index)
    {
        emit Out(readVstatusMsg);
    }
}

void MainWindow::on_pushButtonReadI_clicked()
{
    GlobalSignal readIstatusMsg;
    readIstatusMsg.Type = QVariant::fromValue(SerialPortWorkerBasis::requestBytesTransmission);
    readIstatusMsg.Priority = ui->spinBoxReadI->value();
    readIstatusMsg.Data = QVariant::fromValue(isUHV2?uhv2pump0.HdrCmd().ReadI().Ch1().GenMsg():uhv4pump0.IMeasuredCh1().Read().clearDATA().genMSG());
    for (quint8 index=0; index<=ui->spinBoxReadI_2->value(); ++index)
    {
        emit Out(readIstatusMsg);
    }
}

void MainWindow::on_pushButtonReadP_clicked()
{
    GlobalSignal ReadPstatusMsg;
    ReadPstatusMsg.Type = QVariant::fromValue(SerialPortWorkerBasis::requestBytesTransmission);
    ReadPstatusMsg.Priority = ui->spinBoxReadP->value();
    ReadPstatusMsg.Data = QVariant::fromValue(isUHV2?uhv2pump0.HdrCmd().ReadP().Ch1().GenMsg():uhv4pump0.PMeasuredCh1().Read().clearDATA().genMSG());
    for (quint8 index=0; index<=ui->spinBoxReadP_2->value(); ++index)
    {
        emit Out(ReadPstatusMsg);
    }
}

void MainWindow::on_pushButtonClearBuffer_clicked()
{
    GlobalSignal clearBufferMsg;
    clearBufferMsg.Type = QVariant::fromValue(SerialPortWorkerBasis::clearBuffer);
    clearBufferMsg.Priority = 500;
    emit Out(clearBufferMsg);
}

void MainWindow::on_pushButtonConnect_clicked()
{
    GlobalSignal givePortName;
    givePortName.Type = QVariant::fromValue(SerialPortWorkerBasis::replyPortName);
    if (ui->pushButtonConnect->text() == QStringLiteral("Connect"))
    {
        givePortName.Data = QVariant::fromValue(ui->comboBoxSerialPort->currentText());
    }
    else if (ui->pushButtonConnect->text() == QStringLiteral("Disconnect"))
    {
        givePortName.Data = QVariant::fromValue(QStringLiteral());
    }
    ui->pushButtonConnect->setText("Plz Wait ...");
    emit Out(givePortName);
}

void MainWindow::on_pushButton_QUIT_clicked()
{
    emit killSerialPortThread();
    qApp->quit();
}
