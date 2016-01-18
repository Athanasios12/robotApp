#include "RobotCmdGui.h"
#include "ui_RobotCmdGui.h"
#include <QDateTime>

const QString SEQUENCE_FILE_NAME = "Robot_Conf.xml";

RobotCmdGui::RobotCmdGui(QWidget *parent) :
    QMainWindow(parent),
    robotUi(new Ui::RobotCmdGui),
    bSerialHandler(new SerialBoostHandler())
{
    robotUi->setupUi(this);
    commThread = new SerialThread(this, bSerialHandler);
    serialUi = new SerialGui(this, commThread, bSerialHandler);

    seqeunceHandler = new XmlConfHandler(SEQUENCE_FILE_NAME);
    seqeunceHandler->startNewSession();
    //connect signals an slots between serialthread serial gui and robotcmdgui
    connect(commThread, SIGNAL(receivedData(QByteArray)),this, SLOT(on_receivedData(QByteArray)));
    //main thread emits signal when push_button send clicked and communication is established
    connect(this, SIGNAL(writeData(QByteArray)), commThread, SLOT(sendData(QByteArray)));
}

RobotCmdGui::~RobotCmdGui()
{
    commThread->Stop = true;
    delete robotUi, bSerialHandler, commThread, serialUi;
    delete seqeunceHandler;
}

void RobotCmdGui::on_btnSendCmd_clicked()
{
    sendData(robotUi->etCmdWindow->toPlainText().toStdString().c_str());
}

bool RobotCmdGui::sendData(const QByteArray &data)
{
    if(!bSerialHandler->isConnected())
    {
        appendRobotResponseWindow("\nConnection Error, start communication before sending data!\n");
        return false;
    }
    else
    {
        QDateTime now = QDateTime::currentDateTime();
        appendHistoryWindow(now.toString()+": " + data + "\n");
        emit writeData(data);
    }
    return true;
}

void RobotCmdGui::on_btnSerialSetup_clicked()
{
    serialUi->show();
}

void RobotCmdGui::extractPosition(const QString &posData)
{
    bool processed = false;
    QRegularExpression regexDigit("\\D\\d+.\\d\\d");
    quint32 pos = 0;
    quint8 counter = 0;
    while(!processed)
    {
        QRegularExpressionMatch match = regexDigit.match(posData, pos);
        processed = !match.hasMatch();
        if(match.hasMatch())
        {
            QString positionValue = match.captured(0);
            switch ( counter )
            {
             case 0:
                //set X lcd ;
                robotUi->lcdXPosition->display(positionValue);
                break;
             case 1:
                //set Y lcd
                robotUi->lcdYPosition->display(positionValue);
                break;
            case 2:
               //set Z ldc
                robotUi->lcdZPosition->display(positionValue);
               break;
            case 3:
               //set A lcd
                robotUi->lcdAAngle->display(positionValue);
               break;
            case 4:
               //set B lcd
                robotUi->lcdBAngle->display(positionValue);
               break;
            case 5:
               //set C lcd
                robotUi->lcdCAngle->display(positionValue);
               break;
            }
            pos += positionValue.size();
            ++counter;

        }
    }
}

void RobotCmdGui::on_receivedData(const QByteArray &data)
{
    appendRobotResponseWindow(data.toStdString().c_str());
    //check if received new position - in future check if received error message
    QRegularExpression regexPosition("\\D\\d.*O");
    QRegularExpressionMatch match = regexPosition.match(data);
    if (match.hasMatch())
    {
        QString positionStr = match.captured(0);
        currentRobotPosition = positionStr;
        //actualize the led display
        extractPosition(positionStr);
    }

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

void RobotCmdGui::appendSequenceWindow(const QString &text)
{
    robotUi->txSequenceWindow->setPlainText(robotUi->txSequenceWindow->toPlainText() +
                                     text);
}


void RobotCmdGui::on_btnAxis1_clicked()
{
    sendData("MJ 1,0,0,0,0,0");
}

void RobotCmdGui::on_btnAxis2_clicked()
{
    sendData("MJ 0,1,0,0,0,0");
}

void RobotCmdGui::on_btnAxis3_clicked()
{
    sendData("MJ 0,0,1,0,0,0");
}

void RobotCmdGui::on_btnAxis4_clicked()
{
    sendData("MJ 0,0,0,1,0,0");
}

void RobotCmdGui::on_btnAxis5_clicked()
{
    sendData("MJ 0,0,0,0,1,0");
}

void RobotCmdGui::on_btnAxis6_clicked()
{
    sendData("MJ 0,0,0,0,0,1");
}

void RobotCmdGui::sendPositions()
{
    QString positionCode;
    foreach(QString position, pendingPositionSequence)
    {
        positionCode += "MP " + position + "\n";
    }
    sendData(QByteArray(positionCode.toStdString().c_str()));
}

void RobotCmdGui::on_btnExecuteSequence_clicked()
{
    if(!pendingPositionSequence.empty())
    {
        sendPositions();
    }
}

void RobotCmdGui::on_btnLoadSequence_clicked()
{
    quint64 session_ID = robotUi->etSessionID->toPlainText().toULongLong();
    quint64 sequence_ID = robotUi->etSequenceID->toPlainText().toULongLong();
    QVector<QPair<quint64, quint64> > seqeunces;
    seqeunceHandler->getSessionSequenceIDs(seqeunces);
    if(!seqeunces.empty())
    {
        pendingPositionSequence.clear();
        seqeunceHandler->extractPositionList(session_ID, sequence_ID, pendingPositionSequence);
    }
}

typedef QPair<quint64, quint64> IDPair;
void RobotCmdGui::on_btnShowSequence_clicked()
{
    QVector<QPair<quint64, quint64> > seqeunces;
    seqeunceHandler->getSessionSequenceIDs(seqeunces);
    appendSequenceWindow("\nAvailable Teaching Sequences: \n");
    foreach(IDPair idPair, seqeunces)
    {
        appendSequenceWindow("Session = " + QString::number(idPair.first) +
                             " Sequence = " + QString::number(idPair.second )+ "\n");
    }
}

void RobotCmdGui::on_btnEndSequence_clicked()
{
    seqeunceHandler->endCurrentTeachingSequence();
}

void RobotCmdGui::on_btnAddPosition_clicked()
{
    seqeunceHandler->addPositionToTeach(currentRobotPosition);
    pendingPositionSequence.push_back(currentRobotPosition);
}

void RobotCmdGui::on_btnStartSequence_clicked()
{
    seqeunceHandler->startNewTeachingSequence();
    pendingPositionSequence.clear();
}
