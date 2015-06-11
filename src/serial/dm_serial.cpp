/**
* LICENSE PLACEHOLDER
*/

#include "dm_serial.h"

using namespace openpst;

DmSerial::DmSerial(std::string port, int baudrate ) :
    serial::Serial (port, baudrate, serial::Timeout::simpleTimeout(1000))
{

}

DmSerial::~DmSerial()
{

}
