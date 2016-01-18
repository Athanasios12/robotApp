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
                m_data += 13;
                bSerialHandler->write(m_data.constData(), m_data.count());
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
    m_data = data;
}

