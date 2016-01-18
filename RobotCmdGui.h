#ifndef ROBOTCMDGUI_H
#define ROBOTCMDGUI_H

#include <QMainWindow>
#include "SerialGui.h"
#include "XmlConfHandler.h"

namespace Ui {
class RobotCmdGui;
}

class RobotCmdGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit RobotCmdGui(QWidget *parent = 0);
    ~RobotCmdGui();

private slots:
    void on_btnSendCmd_clicked();
    void on_btnSerialSetup_clicked();

    //manual control buttons
    void on_btnAxis1_clicked();
    void on_btnAxis2_clicked();
    void on_btnAxis3_clicked();
    void on_btnAxis4_clicked();
    void on_btnAxis5_clicked();
    void on_btnAxis6_clicked();

    //positioning seqeunce buttons
    void on_btnExecuteSequence_clicked();
    void on_btnLoadSequence_clicked();
    void on_btnShowSequence_clicked();
    void on_btnEndSequence_clicked();
    void on_btnAddPosition_clicked();
    void on_btnStartSequence_clicked();

public slots:
    void on_receivedData(const QByteArray &data);
signals:
    void writeData(QByteArray);

private:
    Ui::RobotCmdGui *robotUi;
    SerialGui *serialUi;
    SerialThread *commThread;
    SerialBoostHandler *bSerialHandler;
    XmlConfHandler *seqeunceHandler;
    QString currentRobotPosition;
    QVector<QString> pendingPositionSequence;

    void appendHistoryWindow(const QString &text);
    void appendRobotResponseWindow(const QString &text);
    void appendSequenceWindow(const QString &text);

    bool sendData(const QByteArray &data);

    void extractPosition(const QString &posData);
    void sendPositions();
};

#endif // ROBOTCMDGUI_H
