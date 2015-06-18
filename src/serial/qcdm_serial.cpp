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
 * @param spc - a 6 digit SPC code to unlock service programming
 * @return
 */
int QcdmSerial::sendSpc(const char* spc)
{
    if (!isOpen()) {
        return -1;
    }

    qcdm_spc_tx_t packet;
    packet.command = DIAG_SPC_F;
    memcpy(&packet.spc, spc, 6);

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

/**
* @brief send16Password
* @param password - a 16 digit password to unlock secure operations
* @return
*/
int QcdmSerial::send16Password(const char* password)
{
	if (!isOpen()) {
		return -1;
	}

	qcdm_16pw_tx_t packet;
	packet.command = DIAG_PASSWORD_F;
	memcpy(&packet.password, password, 16);

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

	qcdm_16pw_rx_t* rxPacket = (qcdm_16pw_rx_t*)buffer;

	return rxPacket->status == 1 ? 1 : 0;
}

QString QcdmSerial::getNvItemString(int itemId)
{
	if (!isOpen()) {
		return "";
	}

	qcdm_nv_tx_t packet;
	packet.cmd = DIAG_NV_READ_F;
	packet.nvItem = itemId;

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

	qcdm_nv_rx_t* rxPacket = (qcdm_nv_rx_t*)buffer;

	return "TESTING123456789";
}