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
QcdmSerial::QcdmSerial(std::string port, int baudrate) :
    HdlcSerial (port, baudrate)
{

}

/**
 * @brief QcdmSerial::~QcdmSerial
 */
QcdmSerial::~QcdmSerial()
{

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
