#ifndef SERIALCOMMHANDLER_H
#define SERIALCOMMHANDLER_H
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
///
/// \brief The SerialCommHandler class,
///        Enables establishing serial port
///        communication and settings it's parameters
///
class SerialCommHandler
{
public:
    /// class constructor
    SerialCommHandler();
    /// class destructor
    ~SerialCommHandler();

    /// function returns list of all available comm ports
    /// \param ports - vector of strings holding port names
    bool getAvaliablePorts(QVector<QString> &ports) const;

    ///  function return list of all available baud rates for selected port
    ///  \param portName - QString holding selected portName
    ///  \param baudRates - vector of qint32 values representing baud rates
    bool getAvailableBaudRates(const QString &portName,
                               QVector<qint32> &baudRates) const;

    /// function opens comm port and sets connection parameters
    /// \param portName - QString holding selected portName
    /// \param baud - baud rate, connection speed
    /// \param dataBits - data bits (5,6,7 or 8(most common))
    /// \param parity - parity bit
    /// \param stopBit - stop bit
    /// \param flow - flow control
    bool openCommPort(const QString &portName,
                      const qint32 baud,
                      const QSerialPort::DataBits dataBits,
                      const QSerialPort::Parity parity,
                      const QSerialPort::StopBits stopBit,
                      const QSerialPort::FlowControl flow);


    ///function to disconnect from serial device
    void disconnectSerialDevice();

    /// function writes data to comm device
    /// \param data - ByteArray holding write data
    bool writeToSerialDevice(const QByteArray &data);
    /// function reads data from comm device
    /// \param data - ByteArray holds read data
    bool readFromSerialDevice(QByteArray &data);
private:
    /// private variable
    /// tells if serial connection is established
    bool connected;
    /// class member QSerialport class
    /// interace for serial connection
    QSerialPort serial;

    /// sets serial port connection parameters
    bool setSerialCommParams(const qint32 baud,
                             const QSerialPort::DataBits dataBits,
                             const QSerialPort::Parity parity,
                             const QSerialPort::StopBits stopBit,
                             const QSerialPort::FlowControl flow);

    SerialCommHandler( const SerialCommHandler& other ); // non construction-copyable
    SerialCommHandler& operator=( const SerialCommHandler& ); // non copyable

};

#endif // SERIALCOMMHANDLER_H
