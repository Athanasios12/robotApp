#include "RobotCmdGui.h"
#include "ui_RobotCmdGui.h"

RobotCmdGui::RobotCmdGui(QWidget *parent) :
    QMainWindow(parent),
    robotUi(new Ui::RobotCmdGui),
    serialSetup(false)

{
    robotUi->setupUi(this);
}

RobotCmdGui::~RobotCmdGui()
{
    delete robotUi;
}

void RobotCmdGui::on_btnSendCmd_clicked()
{
    if(!serialSetup)
    {
        serialUi.show();
    }
}
