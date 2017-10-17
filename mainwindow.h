#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "UHVWorker/uhvworker.h"

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
    void Out(QVariant, QVariant = QVariant());
private slots:
    void In(QVariant enumVar, QVariant dataVar = QVariant());
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    void updateSENDlabel(const QString &SENDstyleSheet = "",
                         const QString &SentMsgStr = "",
                         const QString &SentMessageStr = "");
    void updateREADlabel(const QString &READstyleSheet = "",
                         const QString &ReadMsgStr = "",
                         const QString &ReadMessageStr = "");
};

#endif // MAINWINDOW_H
