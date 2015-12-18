#include "serialthread.h"
#include <QMutex>

SerialThread::SerialThread(QObject *parent, SerialBoostHandler *serial):
    QThread(parent),
    bSerialHandler(serial),
    dataSend(false),
    Stop(false)
{

}

void SerialThread::run()
{
    if(bSerialHandler->isConnected())
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
                bSerialHandler->write(dataPtr->constData(), dataPtr->count());
                //serialPtr->writeToSerialDevice(*dataPtr);
            }
            std::string rData;
            totalData.clear();
            while(bSerialHandler->read_All(rData))
            {
                totalData += QByteArray(rData.c_str());
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

