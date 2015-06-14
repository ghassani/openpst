/**
* LICENSE PLACEHOLDER
*/

#include "qcdm_serial.h"

using namespace openpst;

/**
 * @brief QcdmSerial::QcdmSerial
 * @param port
 * @param baudrate
 */
QcdmSerial::QcdmSerial(std::string port, int baudrate ) :
    serial::Serial (port, baudrate, serial::Timeout::simpleTimeout(1000))
{

}

/**
 * @brief QcdmSerial::~QcdmSerial
 */
QcdmSerial::~QcdmSerial()
{

}

/**
 * Escapes the data and creates a CRC'ed HDLC packet
 * then writes the data
 *
 * @brief write
 * @param data
 * @param size
 * @return
 */
size_t QcdmSerial::write (uint8_t *data, size_t size)
{
    uint32_t packetSize = 0;
    uint8_t* packet     = NULL;

    hdlc_request(data, size, &packet, &packetSize);

    lastTxSize = Serial::write(packet, packetSize);

    printf("Dumping %zd bytes written\n", lastTxSize);
    hexdump(packet, lastTxSize);

    if (packet != NULL) {
        free(packet);
    }

    return lastTxSize;
}

/**
 * Reads and unescpaes theCRC'ed HDLC packet
 * read from the device
 *
 * @super Serial::read (uint8_t *buffer, size_t size);
 *
 * @brief write
 * @param data
 * @param size
 * @return
 */
size_t QcdmSerial::read (uint8_t *buf, size_t size)
{
    lastRxSize = Serial::read(buffer, DIAG_MAX_RX_PKT_SIZ);

    if (!lastRxSize) {
        printf("Device did not respond\n");
        return 0;
    }

    printf("Dumping %zd bytes read\n", lastRxSize);
    hexdump(buffer, lastRxSize);

    uint32_t respSize = 0;
    uint8_t* resp = NULL;

    hdlc_unescape(buffer, lastRxSize, &resp, &respSize);

    if (respSize != lastRxSize) {
        printf("Dumping %zd bytes escaped\n", respSize);
        hexdump(resp, respSize);
    }

    memcpy(buffer, resp, respSize);

    if (resp != NULL){
        free(resp);
    }

    lastRxSize = respSize;
    return respSize;
}

/**
 * @brief QcdmSerial::sendSpc
 * @param spc - a 6 digit SPC code to unlock
 * @return
 */
int QcdmSerial::sendSpc(const char* spc)
{
    if (!isOpen()) {
        return -1;
    }

    qcdm_spc_tx_t packet;
    packet.command = DIAG_SPC_F;
    memcpy(&packet.spc, spc, sizeof(spc));

    lastTxSize = write((uint8_t*)&packet, sizeof(packet));

    if (!lastTxSize) {
        printf("Attempted to write to device but 0 bytes were written\n");
        return -1;
    }

    lastRxSize = read(buffer, DIAG_MAX_RX_PKT_SIZ);

    if (!lastRxSize) {
        printf("Device did not respond\n");
        return -1;
    }

    qcdm_spc_rx_t* rxPacket = (qcdm_spc_rx_t*) buffer;

    return rxPacket->status == 1 ? 1 : 0;
}
