#ifndef SERIALBOOSTHANDLER_H
#define SERIALBOOSTHANDLER_H

#include <boost/asio/serial_port.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio.hpp>
#include <boost/asio/serial_port_base.hpp>
#include <boost/bind.hpp>



class SerialBoostHandler
{
public:

    // Constructs a blocking reader, pass in an open serial_port and
    // a timeout in milliseconds.
    SerialBoostHandler(const std::string &portName);


    ~SerialBoostHandler();

    void open_port(uint32_t baud_rate,
                   boost::asio::serial_port_base::flow_control::type flow,
                   boost::asio::serial_port_base::parity::type pair,
                   uint8_t dataBits,
                   boost::asio::serial_port_base::stop_bits::type stopBits,
                   size_t timeout);

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
    bool read_error;

    // Reads a character or times out
    // returns false if the read times out
    bool read_char(char& val);

    // Called when an async read completes or has been cancelled
    void read_complete(const boost::system::error_code& error,
                       size_t bytes_transferred);

    // Called when the timer's deadline expires.
    void time_out(const boost::system::error_code& error);

};

#endif // SERIALBOOSTHANDLER_H
