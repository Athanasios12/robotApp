#ifndef SERIALGUI_H
#define SERIALGUI_H

#include <QMainWindow>
#include "SerialCommHandler.h"
#include "serialthread.h"

namespace Ui {
class SerialGui;
}

class SerialGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit SerialGui(QWidget *parent = 0);
    ~SerialGui();
private:
    void appendDialogWindow(const QString &text);
    void setCbOptions();
    bool startSerialComm();
    void removeAllItems();

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
    Ui::SerialGui *ui;
    SerialCommHandler spiHandler;
    SerialThread *serialThread;
};

#endif // SERIALGUI_H
