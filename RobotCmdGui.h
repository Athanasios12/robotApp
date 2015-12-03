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

private:
    Ui::RobotCmdGui *robotUi;
    SerialGui serialUi;
    bool serialSetup;
};

#endif // ROBOTCMDGUI_H
