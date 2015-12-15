#include "SerialBoostHandler.h"
#include <QString>

SerialBoostHandler::SerialBoostHandler(const std::string &portName):
io(),
read_error(true),
serial(io, portName),
timer(serial.get_io_service())
{

}

SerialBoostHandler::~SerialBoostHandler()
{

}

boost::asio::serial_port_base::flow_control::type flowFromStr(const QString &flow)
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

boost::asio::serial_port_base::stop_bits::type stopBitsFromStr(const QString &stopBits)
{
    if(stopBit == "OneStop"){
        return boost::asio::serial_port_base::stop_bits::one;
    }else if(stopBit == "OneAndHalfStop"){
        return boost::asio::serial_port_base::stop_bits::onepointfive;
    }else if(stopBit == "TwoStop"){
        return boost::asio::serial_port_base::stop_bits::two;
    }
    return boost::asio::serial_port_base::stop_bits::one;
}

uint8_t dataBitsFromStr(const QString &dataBits)
{
   return dataBits.toUtf8().constData();
}

boost::asio::serial_port_base::parity::type parityFromStr(const QString &parity)
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

void SerialBoostHandler::open_port(uint32_t baud_rate,
               boost::asio::serial_port_base::flow_control::type flow,
               boost::asio::serial_port_base::parity::type pair,
               uint8_t dataBits,
               boost::asio::serial_port_base::stop_bits::type stopBits,
               size_t timeout)
{
    //set port settings
    serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    serial.set_option(boost::asio::serial_port_base::flow_control(flow));
    serial.set_option(boost::asio::serial_port_base::parity(pair));
    serial.set_option(boost::asio::serial_port_base::stop_bits(stopBits));
    serial.set_option(boost::asio::serial_port_base::character_size(dataBits));

    //set read timeout
    this->timeout = timeout; //ms
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

