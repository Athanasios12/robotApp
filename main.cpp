#include "SerialGui.h"
#include "RobotCmdGui.h"
#include <QApplication>
#include <boost/asio.hpp> // boost serial library

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SerialGui r;
    r.show();

    return a.exec();
}
