#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SerialCommHandler.h"
#include "serialthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void appendDialogWindow(const QString &text);
    void setCbOptions();
    bool startSerialComm();

signals:
    void writeData(QByteArray);

private slots:
    void on_pbSendData_clicked();

    void on_pbStartDataRead_clicked();

    void on_StopDataRead_clicked();

    void on_cbPortComm_currentIndexChanged(const QString &port);

public slots:
    void on_receivedData(const QByteArray &data);

private:
    Ui::MainWindow *ui;
    SerialCommHandler spiHandler;
    SerialThread *serialThread;
};

#endif // MAINWINDOW_H
