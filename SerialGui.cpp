#include "SerialGui.h"
#include "ui_SerialGui.h"
#include <QMutex>
#include <QDebug>

SerialGui::SerialGui(QWidget *parent,
                     SerialThread *commThread,
                     SerialBoostHandler *serialHandler) :
    QMainWindow(parent),
    ui(new Ui::SerialGui),
    serialThread(commThread),
    bSerialHandler(serialHandler)

{
    ui->setupUi(this);
    QVector<QString> commList;
    if(!spiHandler.getAvaliablePorts(commList))
    {
        appendDialogWindow("No Comm Ports Available");
    }else
    {
        appendDialogWindow("Available Ports:\n");
        foreach (QString port, commList)
        {
            appendDialogWindow(port + "\n");
            //initialize combo
            ui->cbPortComm->addItem(port);
        }
        //create serial thread and connect signals and slots
        //serial thread emits signal when received data form serial port
        connect(serialThread, SIGNAL(receivedData(QByteArray)),this, SLOT(on_receivedData(QByteArray)));
        //main thread emits signal when push_button send clicked and communication is established
        connect(this, SIGNAL(writeData(QByteArray)), serialThread, SLOT(sendData(QByteArray)));
    }
}

SerialGui::~SerialGui()
{
    delete ui;
}

void SerialGui::appendDialogWindow(const QString &text)
{
    ui->txDialogWindow->setPlainText(ui->txDialogWindow->toPlainText() +
                                     text);
}

void SerialGui::removeAllItems()
{
    ui->cbDataBits->clear();
    ui->cbParityBit->clear();
    ui->cbFlowControl->clear();
    ui->cbStopBit->clear();
    ui->cbBaudRate->clear();
}

//ComboBox slots
void SerialGui::on_cbPortComm_currentIndexChanged(const QString &port)
{
    removeAllItems();
    //fill other comboBoxes with connection properietes
    QVector<qint32> baudList;
    if(!spiHandler.getAvailableBaudRates(port, baudList))
    {
        appendDialogWindow("Port does not support any baud rates");
    }else
    {
        foreach(qint32 baud, baudList)
        {
            ui->cbBaudRate->addItem(QString::number(baud), baud);
        }
        //fill other comboBoxes with possible properities
        setCbOptions();
    }
}

void SerialGui::setCbOptions()
{
    ui->cbDataBits->addItem("5");
    ui->cbDataBits->addItem("6");
    ui->cbDataBits->addItem("7");
    ui->cbDataBits->addItem("8");

    ui->cbParityBit->addItem("NoParity");
    ui->cbParityBit->addItem("EvenParity");
    ui->cbParityBit->addItem("OddParity");

    ui->cbFlowControl->addItem("NoFlowControl");
    ui->cbFlowControl->addItem("HardwareControl");
    ui->cbFlowControl->addItem("SoftwareControl");

    ui->cbStopBit->addItem("OneStop");
    ui->cbStopBit->addItem("OneAndHalfStop");
    ui->cbStopBit->addItem("TwoStop");
}

bool SerialGui::startSerialComm()
{

    if(!bSerialHandler->open_port(ui->cbPortComm->currentText().toStdString(),
                                  ui->cbBaudRate->currentData().value<uint32_t>(),
                                  ui->cbFlowControl->currentText(),
                                  ui->cbParityBit->currentText(),
                                  ui->cbDataBits->currentText(),
                                  ui->cbStopBit->currentText(),
                                  100))
    {
            appendDialogWindow("Connection Error, couldn't open port\n");
            return false;
    }

    serialThread->start();
    return true;
}

//Push Button slots
void SerialGui::on_pbSendData_clicked()
{
    bool isConnected = bSerialHandler->isConnected();
    if(!bSerialHandler->isConnected())
    {
        appendDialogWindow("Connection Error, start communication before sending data!\n");
    }
    if(isConnected)
    {
        QByteArray data(ui->etTransferData->toPlainText().toStdString().c_str());
        emit writeData(data);
    }
}

void SerialGui::on_pbStartDataRead_clicked()
{

    if(!bSerialHandler->isConnected())
    {
        if(startSerialComm())
        {
            appendDialogWindow("Connected to port :" +
                               ui->cbPortComm->currentText() + "\n");
        }else
        {
            appendDialogWindow("Error, couldn't connect to port :" +
                               ui->cbPortComm->currentText() + "\n");
        }
    }

}

void SerialGui::on_StopDataRead_clicked()
{
    if(bSerialHandler->isConnected())
    {
        serialThread->Stop = true;
        bSerialHandler->close_port();
    }
}

void SerialGui::on_receivedData(const QByteArray &data)
{
    appendDialogWindow("Received data: ");
    appendDialogWindow(QString(data));
    appendDialogWindow("\n");
    qDebug() << data << "\n";
}

