#include "SerialBoostHandler.h"
#include <QByteArray>
#include <boost/bind.hpp>
#include<boost/system/error_code.hpp>

SerialBoostHandler::SerialBoostHandler():
io(),
read_error(true),
serial(io),
timer(serial.get_io_service())
{

}

SerialBoostHandler::~SerialBoostHandler()
{
    if(serial.is_open())
    {
        serial.close();
    }
}

boost::asio::serial_port_base::flow_control::type SerialBoostHandler::flowFromStr(const QString &flow)
{
    if(flow == "NoFlowControl"){
        return boost::asio::serial_port_base::flow_control::none;
    }else if(flow == "HardwareControl"){
        return boost::asio::serial_port_base::flow_control::hardware;
    }else if(flow == "SoftwareControl"){
        return boost::asio::serial_port_base::flow_control::software;
    }
    return boost::asio::serial_port_base::flow_control::none;
}

boost::asio::serial_port_base::stop_bits::type SerialBoostHandler::stopBitsFromStr(const QString &stopBits)
{
    if(stopBits == "OneStop"){
        return boost::asio::serial_port_base::stop_bits::one;
    }else if(stopBits == "OneAndHalfStop"){
        return boost::asio::serial_port_base::stop_bits::onepointfive;
    }else if(stopBits == "TwoStop"){
        return boost::asio::serial_port_base::stop_bits::two;
    }
    return boost::asio::serial_port_base::stop_bits::one;
}

uint8_t SerialBoostHandler::dataBitsFromStr(const QString &dataBits)
{
   return (uint8_t)(*dataBits.toUtf8().constData());
}

boost::asio::serial_port_base::parity::type SerialBoostHandler::parityFromStr(const QString &parity)
{
    if(parity == "NoParity"){
        return boost::asio::serial_port_base::parity::none;
    }else if(parity == "EvenParity"){
        return boost::asio::serial_port_base::parity::even;
    }else if(parity == "OddParity"){
        return boost::asio::serial_port_base::parity::odd;
    }
    return boost::asio::serial_port_base::parity::none;
}

bool SerialBoostHandler::open_port(const std::string &portName,
                                   uint32_t baud_rate,
                                   const QString &flow,
                                   const QString &pair,
                                   const QString &dataBits,
                                   const QString &stopBits,
                                   size_t timeout)
{
    //set port settings
    boost::asio::serial_port_base::flow_control::type bFlow = flowFromStr(flow);
    boost::asio::serial_port_base::stop_bits::type bStopBits = stopBitsFromStr(stopBits);
    boost::asio::serial_port_base::parity::type bParity = parityFromStr(pair);
    uint8_t bDataBits = dataBitsFromStr(dataBits);

    serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    serial.set_option(boost::asio::serial_port_base::flow_control(bFlow));
    serial.set_option(boost::asio::serial_port_base::parity(bParity));
    serial.set_option(boost::asio::serial_port_base::stop_bits(bStopBits));
    serial.set_option(boost::asio::serial_port_base::character_size(bDataBits));

    //set read timeout
    this->timeout = timeout; //ms

    serial.open(portName, boost_error);

    if(boost_error != boost::system::errc::success)
    {
        return false;
    }

    return true;
}

void SerialBoostHandler::close_port()
{
    if(serial.is_open())
    {
        serial.close();
    }
}

bool SerialBoostHandler::isConnected()
{
    return serial.is_open();
}

std::string SerialBoostHandler::read_line()
{
    char ch;
    std::string response;

    while (read_char(ch) && ch != '\n')
    {
        response += ch;
    }
    return response;
}

std::string SerialBoostHandler::read_All()
{
    char ch;
    std::string response;

    while (read_char(ch))
    {
        response += ch;
    }
    return response;
}

void SerialBoostHandler::write(const std::string &data)
{
    boost::asio::write(serial, boost::asio::buffer(data.c_str(), data.size()));
}

void SerialBoostHandler::write(const char *data, size_t data_size)
{
     boost::asio::write(serial, boost::asio::buffer(data, data_size));
}

// Reads a character or times out
// returns false if the read times out
bool SerialBoostHandler::read_char(char& val) {

    val = c = '\0';

    // After a timeout & cancel it seems we need
    // to do a reset for subsequent reads to work.
    serial.get_io_service().reset();

    // Asynchronously read 1 character.
    boost::asio::async_read(serial, boost::asio::buffer(&c, 1),
                            boost::bind(&SerialBoostHandler::read_complete,
                            this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));

    // Setup a deadline time to implement our timeout.
    timer.expires_from_now(boost::posix_time::milliseconds(timeout));
    timer.async_wait(boost::bind(&SerialBoostHandler::time_out,
                     this, boost::asio::placeholders::error));

    // This will block until a character is read
    // or until the it is cancelled.
    serial.get_io_service().run();

    if (!read_error)
        val = c;

    return !read_error;
}

// Called when an async read completes or has been cancelled
void SerialBoostHandler::read_complete(const boost::system::error_code& error,
                   size_t bytes_transferred) {

    read_error = (error || bytes_transferred == 0);

    // Read has finished, so cancel the
    // timer.
    timer.cancel();
}

// Called when the timer's deadline expires.
void SerialBoostHandler::time_out(const boost::system::error_code& error) {

    // Was the timeout cancelled?
    if (error) {
        // yes
        return;
    }

    // no, we have timed out, so kill
    // the read operation
    // The read callback will be called
    // with an error
    serial.cancel();
}

