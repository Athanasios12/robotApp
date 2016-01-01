#include "RobotCmdGui.h"
#include "ui_RobotCmdGui.h"
#include <QDateTime>

RobotCmdGui::RobotCmdGui(QWidget *parent) :
    QMainWindow(parent),
    robotUi(new Ui::RobotCmdGui),
    bSerialHandler(new SerialBoostHandler())
{
    robotUi->setupUi(this);
    commThread = new SerialThread(this, bSerialHandler);
    serialUi = new SerialGui(this, commThread, bSerialHandler);

    //connect signals an slots between serialthread serial gui and robotcmdgui
    connect(commThread, SIGNAL(receivedData(QByteArray)),this, SLOT(on_receivedData(QByteArray)));
    //main thread emits signal when push_button send clicked and communication is established
    connect(this, SIGNAL(writeData(QByteArray)), commThread, SLOT(sendData(QByteArray)));
}

RobotCmdGui::~RobotCmdGui()
{
    delete robotUi, bSerialHandler, commThread, serialUi;
}

void RobotCmdGui::on_btnSendCmd_clicked()
{
    if(!bSerialHandler->isConnected())
    {
        appendRobotResponseWindow("\nConnection Error, start communication before sending data!\n");
    }
    else
    {
        QByteArray data(robotUi->etCmdWindow->toPlainText().toStdString().c_str());
        QDateTime now = QDateTime::currentDateTime();
        appendHistoryWindow(now.toString()+": "+ robotUi->etCmdWindow->toPlainText() + "\n");
        emit writeData(data);
    }
}


void RobotCmdGui::on_btnSerialSetup_clicked()
{
    serialUi->show();
}

void RobotCmdGui::on_receivedData(const QByteArray &data)
{
    appendRobotResponseWindow(data.toStdString().c_str());
}

void RobotCmdGui::appendHistoryWindow(const QString &text)
{
    robotUi->txCmdHistory->setPlainText(robotUi->txCmdHistory->toPlainText() +
                                     text);
}

void RobotCmdGui::appendRobotResponseWindow(const QString &text)
{
    robotUi->txRobotResponse->setPlainText(robotUi->txRobotResponse->toPlainText() +
                                     text);
}

