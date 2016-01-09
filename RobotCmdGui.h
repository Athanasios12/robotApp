#ifndef ROBOTCMDGUI_H
#define ROBOTCMDGUI_H

#include <QMainWindow>
#include "SerialGui.h"

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

public slots:
    void on_receivedData(const QByteArray &data);
signals:
    void writeData(QByteArray);

private:
    Ui::RobotCmdGui *robotUi;
    SerialGui *serialUi;
    SerialThread *commThread;
    SerialBoostHandler *bSerialHandler;

    void appendHistoryWindow(const QString &text);
    void appendRobotResponseWindow(const QString &text);
    bool sendData(const QByteArray &data);
};

#endif // ROBOTCMDGUI_H
