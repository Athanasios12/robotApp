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
        QByteArray totalData;
        while(!Stop)
        {
            mutex.lock();
            if(dataSend)
            {
                dataSend = false;
                *dataPtr += 13; // /r character ascii number
                serialPtr->writeToSerialDevice(*dataPtr);
            }
            QByteArray readData;
            totalData.clear();
            while(serialPtr->readFromSerialDevice(readData))
            {
                totalData += readData;
            }
            if(!totalData.isEmpty())
            {
                emit receivedData(totalData);
            }
            mutex.unlock();
        }
    }
}

void SerialThread::sendData(const QByteArray &data)
{
    dataSend = true;
    dataPtr = new QByteArray(data);
}

