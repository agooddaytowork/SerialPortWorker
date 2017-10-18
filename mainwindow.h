#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "shared/commonthings.h"
#include "src/serialportworker.h"
#include "BinaryProtocol/src/binaryprotocol.h"
#include "WindowProtocol/src/windowprotocol.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void Out(const GlobalSignal &);
    void killSerialPortThread();
private slots:
    void In(const GlobalSignal &aGlobalSignal);
    void on_pushButtonHVonoff_clicked();

    void on_pushButtonReadV_clicked();

    void on_pushButtonReadI_clicked();

    void on_pushButtonReadP_clicked();

    void on_pushButtonClearBuffer_clicked();

    void on_pushButtonConnect_clicked();

    void on_pushButton_QUIT_clicked();

private:
    Ui::MainWindow *ui;
    bool isUHV2 = true;
    BinaryProtocol uhv2pump0;
    WindowProtocol uhv4pump0;
    void updateSENDlabel(const QString &SENDstyleSheet = "",
                         const QString &SentMsgStr = "",
                         const QString &SentMessageStr = "");
    void updateREADlabel(const QString &READstyleSheet = "",
                         const QString &ReadMsgStr = "",
                         const QString &ReadMessageStr = "");
};

#endif // MAINWINDOW_H
