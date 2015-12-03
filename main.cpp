#include "SerialGui.h"
#include "RobotCmdGui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RobotCmdGui r;
    r.show();

    return a.exec();
}
