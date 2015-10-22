#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::appendDialogWindow(const QString &text)
{
    ui->txDialogWindow->setPlainText(ui->txDialogWindow->toPlainText() +
                                     text);
}

//ComboBox slots
void MainWindow::on_cbPortComm_currentIndexChanged(const QString &port)
{
    appendDialogWindow("Selected Port :" + port + "\n");
    //fill other comboBoxes with connection properietes
    QVector<qint32> baudList;
    if(!spiHandler.getAvailableBaudRates(port, baudList))
    {
        appendDialogWindow("Port does not support any boud rates");
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

void MainWindow::setCbOptions()
{
    ui->cbDataBits->addItem("5");
    ui->cbDataBits->addItem("6");
    ui->cbDataBits->addItem("7");
    ui->cbDataBits->addItem("8");

    ui->cbParityBit->addItem("NoParity");
    ui->cbParityBit->addItem("EvenParity");
    ui->cbParityBit->addItem("OddParity");
    ui->cbParityBit->addItem("SpaceParity");
    ui->cbParityBit->addItem("MarkParity");

    ui->cbFlowControl->addItem("NoFlowControl");
    ui->cbFlowControl->addItem("HardwareControl");
    ui->cbFlowControl->addItem("SoftwareControl");

    ui->cbStopBit->addItem("OneStop");
    ui->cbStopBit->addItem("OneAndHalfStop");
    ui->cbStopBit->addItem("TwoStop");
}

bool MainWindow::startSerialComm()
{
    if(!spiHandler.openCommPort(ui->cbPortComm->currentData().value<QString>(),
                            ui->cbBaudRate->currentData().value<qint32>(),
                            ui->cbDataBits->currentText(),
                            ui->cbParityBit->currentText(),
                            ui->cbStopBit->currentText(),
                            ui->cbFlowControl->currentText()))
    {
        appendDialogWindow("Connection Error, couldn't open port\n");
        return false;
    }
    return true;
}

//Push Button slots
void MainWindow::on_pbSendData_clicked()
{
    bool isConnected = spiHandler.isSerialOpened();
    if(!spiHandler.isSerialOpened())
    {
        isConnected = startSerialComm();
    }
    if(isConnected)
    {
        QByteArray data(ui->etTransferData->toPlainText().toStdString().c_str());
        spiHandler.writeToSerialDevice(data);
    }
}

void MainWindow::on_pbStartDataRead_clicked()
{
    bool isConnected = spiHandler.isSerialOpened();
    if(!spiHandler.isSerialOpened())
    {
        isConnected = startSerialComm();
    }
    if(isConnected)
    {
        QByteArray data;
        if(!spiHandler.readFromSerialDevice(data))
        {
            appendDialogWindow("Data read error\n");
        }else
        {
            appendDialogWindow(QString(data));
        }
    }
}

void MainWindow::on_StopDataRead_clicked()
{
    if(spiHandler.isSerialOpened())
    {
        spiHandler.disconnectSerialDevice();
    }
}

//Edit Text and text widget slots


