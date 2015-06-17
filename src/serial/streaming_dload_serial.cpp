/**
* LICENSE PLACEHOLDER
*/

#include "streaming_dload_serial.h"

using namespace openpst;

/**
 * @brief StreamingDloadSerial::StreamingDloadSerial
 * @param port
 * @param baudrate
 */
StreamingDloadSerial::StreamingDloadSerial(std::string port, int baudrate) :
    HdlcSerial (port, baudrate)
{

}

/**
 * @brief StreamingDloadSerial::~StreamingDloadSerial
 */
StreamingDloadSerial::~StreamingDloadSerial()
{

}
