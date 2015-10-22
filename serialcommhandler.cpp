#include "serialcommhandler.h"
#include "QVector"
#include "QString"

SerialCommHandler::SerialCommHandler()
{

}

SerialCommHandler::~SerialCommHandler()
{
    serial.close();
}

bool SerialCommHandler::getAvaliablePorts(QVector<QString> &ports) const
{
    bool portsAvailable = false;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ports.push_back(info.portName());
        portsAvailable = true;
    }
    return portsAvailable;
}

bool SerialCommHandler::getAvailableBaudRates(const QString &portName,
                                              QVector<qint32> &baudRates) const
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if(portName == info.portName())
        {
            baudRates = QVector<qint32>::fromList(info.standardBaudRates());
            return true;
        }
    }
    return false;
}


bool SerialCommHandler::openCommPort(const QString &portName,
                                     const qint32 baud,
                                     const QSerialPort::DataBits dataBits,
                                     const QSerialPort::Parity parity,
                                     const QSerialPort::StopBits stopBit,
                                     const QSerialPort::FlowControl flow)
                {
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if(portName == info.portName())
        {
            serial.setPort(info);
            if (serial.open(QIODevice::ReadWrite))
            {
                if(setSerialCommParams(baud, dataBits, parity, stopBit, flow))
                {
                    return true;
                }
            }
            return false;
        }
    }
    return false;

}



inline void SerialCommHandler::disconnectSerialDevice()
{
    serial.close();
}

bool SerialCommHandler::writeToSerialDevice(const QByteArray &data)
{
    if(serial.write(data))
    {
        return true;
    }
    return false;
}

bool SerialCommHandler::readFromSerialDevice(QByteArray &data)
{
    if(serial.waitForReadyRead(100))
    {
        data = serial.readAll();
        return true;
    }
    return false;
}

bool SerialCommHandler::setSerialCommParams(const qint32 baud,
                                            const QSerialPort::DataBits dataBits,
                                            const QSerialPort::Parity parity,
                                            const QSerialPort::StopBits stopBit,
                                            const QSerialPort::FlowControl flow)
{
    if(!serial.setBaudRate(baud))
    {
        return false;
    }

    if(!serial.setDataBits(dataBits))
    {
        return false;
    }
    if(!serial.setParity(parity))
    {
        return false;
    }
    if(!serial.setStopBits(stopBit))
    {
        return false;
    }
    if(!serial.setFlowControl(flow))
    {
        return false;
    }

    return true;
}

