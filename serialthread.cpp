#include "serialthread.h"
#include <QMutex>

SerialThread::SerialThread(QObject *parent, SerialCommHandler *serial):
    QThread(parent),
    serialPtr(serial),
    dataSend(false),
    Stop(false)
{

}

void SerialThread::run()
{
    if(serialPtr->isSerialOpened())
    {
        Stop = false;
        QMutex mutex;
        while(!Stop)
        {
            mutex.lock();
            if(dataSend)
            {
                dataSend = false;
                serialPtr->writeToSerialDevice(*dataPtr);
            }
            QByteArray readData;
            if(serialPtr->readFromSerialDevice(readData))
            {
                emit receivedData(readData);
            }
            mutex.unlock();
        }
    }
}

void SerialThread::sendData(const QByteArray &data)
{
    dataSend = true;
    dataPtr = &data;
}

