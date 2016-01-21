#include "serialthread.h"
#include <QMutex>
#include <QDateTime>

SerialThread::SerialThread(QObject *parent, SerialBoostHandler *serial):
    QThread(parent),
    bSerialHandler(serial),
    dataSend(false),
    Stop(false),
    StartTimer(false)
{

}

void SerialThread::run()
{
    if(bSerialHandler->isConnected())
    {
        Stop = false;
        QMutex mutex;
        QByteArray totalData;
        quint64 timer = 0;
        bool timerStarted = false;
        while(!Stop)
        {
            mutex.lock();
            //position Monitoring
            if(!StartTimer && timerStarted)
            {
                timerStarted = false;
            }

            if(timerStarted)
            {
               if((QDateTime::currentMSecsSinceEpoch() - timer) >= 2500)
               {
                   QByteArray posRequest("WH");
                   posRequest += 13;
                   bSerialHandler->write(posRequest.constData(), posRequest.count());
                   timer = QDateTime::currentMSecsSinceEpoch();
               }
            }
            if(StartTimer)
            {
                timer = QDateTime::currentMSecsSinceEpoch();
                timerStarted = true;
            }

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

