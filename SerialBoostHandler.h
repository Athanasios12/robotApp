#ifndef SERIALBOOSTHANDLER_H
#define SERIALBOOSTHANDLER_H

#include <boost/asio/serial_port.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio.hpp>
#include <boost/asio/serial_port_base.hpp>
#include <QString>

class SerialBoostHandler
{
public:

    // Constructs a blocking reader, pass in an open serial_port and
    // a timeout in milliseconds.
    SerialBoostHandler();

    ~SerialBoostHandler();

    bool open_port(const std::string &portName,
                   uint32_t baud,
                   const QString &dataBits,
                   const QString &parity,
                   const QString &stopBit,
                   const QString &flow,
                   size_t timeout);

    void close_port();

    bool isConnected();

    std::string read_line();


    std::string read_All();

    void write(const std::string &data);

    void write(const char *data, size_t data_size);

private :

    boost::asio::io_service io;
    boost::asio::serial_port serial;
    size_t timeout;
    char c;
    boost::asio::deadline_timer timer;
    boost::system::error_code boost_error;
    bool read_error;

    // Reads a character or times out
    // returns false if the read times out
    bool read_char(char& val);

    // Called when an async read completes or has been cancelled
    void read_complete(const boost::system::error_code& error,
                       size_t bytes_transferred);

    // Called when the timer's deadline expires.
    void time_out(const boost::system::error_code& error);

    boost::asio::serial_port_base::flow_control::type flowFromStr(const QString &flow);
    boost::asio::serial_port_base::stop_bits::type stopBitsFromStr(const QString &stopBits);
    uint8_t dataBitsFromStr(const QString &dataBits);
    boost::asio::serial_port_base::parity::type parityFromStr(const QString &parity);

};

#endif // SERIALBOOSTHANDLER_H
