#include "serialcommhandler.h"
#include "QVector"
#include "QString"

SerialCommHandler::SerialCommHandler()
{

}

SerialCommHandler::~SerialCommHandler()
{
    if(serial.isOpen())
    {
        serial.close();
    }
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
                                     const QString &dataBits,
                                     const QString &parity,
                                     const QString &stopBit,
                                     const QString &flow)
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if(portName == info.portName())
        {
            serial.setPort(info);
            if (serial.open(QIODevice::ReadWrite))
            {
                connected = true;
                QSerialPort::DataBits eDataBits = dataBitsFromString(dataBits);
                QSerialPort::Parity eParity = parityFromString(parity);
                QSerialPort::StopBits eStopBit = stopBitFromString(stopBit);
                QSerialPort::FlowControl eFlow = flowFromString(flow);
                if(setSerialCommParams(baud, eDataBits, eParity, eStopBit, eFlow))
                {
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

bool SerialCommHandler::isSerialOpened()
{
    return serial.isOpen();
}

QSerialPort::DataBits SerialCommHandler::dataBitsFromString(const QString &dataBits)
{
    if(dataBits == "5"){
        return QSerialPort::Data5;
    }else if(dataBits == "6"){
        return QSerialPort::Data6;
    }else if(dataBits == "7"){
        return QSerialPort::Data7;
    }else if(dataBits == "8"){
        return QSerialPort::Data8;
    }
    return QSerialPort::UnknownDataBits;
}

QSerialPort::Parity SerialCommHandler::parityFromString(const QString &parity)
{
    if(parity == "NoParity"){
        return QSerialPort::NoParity;
    }else if(parity == "EvenParity"){
        return QSerialPort::EvenParity;
    }else if(parity == "OddParity"){
        return QSerialPort::OddParity;
    }else if(parity == "SpaceParity"){
        return QSerialPort::SpaceParity;
    }else if(parity == "MarkParity"){
        return QSerialPort::MarkParity;
    }
    return QSerialPort::UnknownParity;
}

QSerialPort::StopBits SerialCommHandler::stopBitFromString(const QString &stopBit)
{
    if(stopBit == "OneStop"){
        return QSerialPort::OneStop;
    }else if(stopBit == "OneAndHalfStop"){
        return QSerialPort::OneAndHalfStop;
    }else if(stopBit == "TwoStop"){
        return QSerialPort::TwoStop;
    }
    return QSerialPort::UnknownStopBits;
}

QSerialPort::FlowControl SerialCommHandler::flowFromString(const QString &flow)
{
    if(flow == "NoFlowControl"){
        return QSerialPort::NoFlowControl;
    }else if(flow == "HardwareControl"){
        return QSerialPort::HardwareControl;
    }else if(flow == "SoftwareControl"){
        return QSerialPort::SoftwareControl;
    }
    return QSerialPort::UnknownFlowControl;
}

void SerialCommHandler::disconnectSerialDevice()
{
    if(connected){
        serial.close();
        connected = false;
    }
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

