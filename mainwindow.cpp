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
            ui->cbBaudRate->addItem(QString::number(baud));
        }
        //fill other comboBoxes with possible properities
        setCbOptions();
    }
}

void MainWindow::setCbOptions()
{
    QStringList dataBits;
    dataBits << "5" << "6" << "7" << "8";

    QStringList parity;
    parity << "NoParity" << "EvenParity" << "OddParity"
           << "SpaceParity" << "MarkParity";

    QStringList flowControl;
    flowControl << "NoFlowControl" << "HardwareControl" << "SoftwareControl" ;

    QStringList stopBits;
    stopBits << "OneStop" << "OneAndHalfStop" << "TwoStop";

    foreach (const QString &val, dataBits)
    {
        ui->cbDataBits->addItem(val);
    }
    foreach (const QString &val, parity)
    {
       ui->cbParityBit->addItem(val);
    }
    foreach (const QString &val, flowControl)
    {
        ui->cbFlowControl->addItem(val);
    }
    foreach (const QString &val, stopBits)
    {
        ui->cbStopBit->addItem(val);
    }

}

//Push Button slots
void MainWindow::on_pbSendData_clicked()
{

}

void MainWindow::on_pbStartDataRead_clicked()
{

}

void MainWindow::on_StopDataRead_clicked()
{

}

//Edit Text and text widget slots


