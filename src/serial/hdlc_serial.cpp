/**
* LICENSE PLACEHOLDER
*/

#include "hdlc_serial.h"

using namespace openpst;

/**
 * @brief HdlcSerial::HdlcSerial
 * @param port
 * @param baudrate
 */
HdlcSerial::HdlcSerial(std::string port, int baudrate) :
    serial::Serial (port, baudrate, serial::Timeout::simpleTimeout(1000))
{

}

/**
 * @brief QcdmSerial::~QcdmSerial
 */
HdlcSerial::~HdlcSerial()
{

}

/**
 * Escapes the data and creates a CRC'ed HDLC packet
 * then writes the data
 *
 * @super Serial::write (uint8_t *data, size_t size);
 *
 * @brief write
 * @param data
 * @param size
 * @param bool encapsulate
 * @return
 */
size_t HdlcSerial::write (uint8_t *data, size_t size, bool encapsulate)
{
    if (!encapsulate) {
        return Serial::write(data, size);
    }

    uint32_t packetSize = 0;
    uint8_t* packet     = NULL;

    hdlc_request(data, size, &packet, &packetSize);

    size_t bytesWritten = Serial::write(packet, packetSize);

    printf("Dumping %u bytes written\n", bytesWritten);
    
    hexdump(packet, bytesWritten);

    if (packet != NULL) {
        free(packet);
    }

    return bytesWritten;    
}

/**
 * Reads and unescpaes theCRC'ed HDLC packet
 * read from the device
 *
 * @super Serial::read (uint8_t *buffer, size_t size);
 *
 * @brief read
 * @param uint8_t* buf
 * @param size_t size
 * @param bool unescape
 * @return
 */
size_t HdlcSerial::read (uint8_t *buf, size_t size, bool unescape )
{
    size_t bytesRead = Serial::read(buf, size);

    if (!unescape || !bytesRead) {
        return bytesRead;
    }

    printf("Dumping %u bytes read\n", bytesRead);

    hexdump(buffer, bytesRead);

    uint32_t respSize = 0;
    uint8_t* resp = NULL;

    hdlc_unescape(buffer, bytesRead, &resp, &respSize);

    if (respSize != bytesRead) {
        printf("Dumping %u bytes escaped\n", respSize);
        hexdump(resp, respSize);
    }

    memcpy(buf, resp, respSize);

    if (resp != NULL){
        free(resp);
    }

    return respSize;
}
