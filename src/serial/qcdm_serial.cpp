/**
* LICENSE PLACEHOLDER
*
* @file qcdm_serial.h
* @class QcdmSerial
* @package OpenPST
* @brief QCDM serial port implementation
*
* @author Gassan Idriss <ghassani@gmail.com>
* @author Matteson Raab <mraabhimself@gmail.com>
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
 * @return SPC status response
 */
int QcdmSerial::sendSpc(const char* spc)
{
    if (!isOpen()) {
        return DIAG_CMD_PORT_CLOSED;
    }

    qcdm_spc_tx_t packet;
    packet.command = DIAG_SPC_F;
    memcpy(&packet.spc, spc, 6);

    lastTxSize = write((uint8_t*)&packet, sizeof(packet));

    if (!lastTxSize) {
        printf("Attempted to write to device but 0 bytes were written\n");
        return DIAG_CMD_TX_FAIL;
    }

    lastRxSize = read(buffer, DIAG_MAX_RX_PKT_SIZ);

    if (!lastRxSize) {
        printf("Device did not respond\n");
		return DIAG_CMD_RX_FAIL;
    }

    qcdm_spc_rx_t* rxPacket = (qcdm_spc_rx_t*) buffer;

    return rxPacket->status;
}

/**
* @brief send16Password
* @param password - a 16 digit password to unlock secure operations
* @return Password status response
*/
int QcdmSerial::send16Password(const char* password)
{
	if (!isOpen()) {
		return DIAG_CMD_PORT_CLOSED;
	}

	qcdm_16pw_tx_t packet;
	packet.command = DIAG_PASSWORD_F;
	memcpy(&packet.password, password, 16);

	lastTxSize = write((uint8_t*)&packet, sizeof(packet));

	if (!lastTxSize) {
		printf("Attempted to write to device but 0 bytes were written\n");
		return DIAG_CMD_TX_FAIL;
	}

	lastRxSize = read(buffer, DIAG_MAX_RX_PKT_SIZ);

	if (!lastRxSize) {
		printf("Device did not respond\n");
		return DIAG_CMD_RX_FAIL;
	}

	qcdm_16pw_rx_t* rxPacket = (qcdm_16pw_rx_t*)buffer;

	return rxPacket->status;
}

/**
* @brief getNvItem
* @param itemId = NV Item ID
* @param response = Full QCDM response
* @return QCDM command response status
*/
int QcdmSerial::getNvItem(int itemId, uint8_t** response)
{
	if (!isOpen()) {
		return DIAG_CMD_PORT_CLOSED;
	}

	qcdm_nv_tx_t packet;
	memset(&packet, 0, sizeof(packet));
	packet.cmd = DIAG_NV_READ_F;
	packet.nvItem = itemId;

	lastTxSize = write((uint8_t*)&packet, sizeof(packet));

	if (!lastTxSize) {
		printf("Attempted to write to device but 0 bytes were written\n");
		return DIAG_CMD_TX_FAIL;
	}

	lastRxSize = read(buffer, DIAG_MAX_RX_PKT_SIZ);

	if (!lastRxSize) {
		printf("Device did not respond\n");
		return DIAG_CMD_RX_FAIL;
	}

	*response = buffer;

	qcdm_nv_rx_t* rxPacket = (qcdm_nv_rx_t*)buffer;

	return rxPacket->cmd;
}

/**
* @brief setNvItem
* @param itemId = NV Item ID
* @param response = Full QCDM response
* @return QCDM command response status
*/
int QcdmSerial::setNvItem(int itemId, const char *data, int length, uint8_t** response)
{
	if (!isOpen()) {
		return DIAG_CMD_PORT_CLOSED;
	}

	qcdm_nv_tx_t packet;
	memset(&packet, 0, sizeof(packet));
	packet.cmd = DIAG_NV_WRITE_F;
	packet.nvItem = itemId;
	memcpy(&packet.data, data, length);

	lastTxSize = write((uint8_t*)&packet, sizeof(packet));

	if (!lastTxSize) {
		printf("Attempted to write to device but 0 bytes were written\n");
		return DIAG_CMD_TX_FAIL;
	}

	lastRxSize = read(buffer, DIAG_MAX_RX_PKT_SIZ);

	if (!lastRxSize) {
		printf("Device did not respond\n");
		return DIAG_CMD_RX_FAIL;
	}

	*response = buffer;

	qcdm_nv_rx_t* rxPacket = (qcdm_nv_rx_t*)buffer;

	return rxPacket->cmd;
}

/**
* @brief sendHtcNvUnlock
* @param response -
* @return
*/
int QcdmSerial::sendHtcNvUnlock(uint8_t** response)
{
	if (!isOpen()) {
		return DIAG_CMD_PORT_CLOSED;
	}

	unsigned char data[] = { DIAG_SPC_F, 0x74, 0x64, 0x77, 0x61, 0x6F, 0x70 };

	lastTxSize = write((uint8_t*)data, sizeof(data));

	if (!lastTxSize) {
		printf("Attempted to write to device but 0 bytes were written\n");
		return DIAG_CMD_TX_FAIL;
	}

	lastRxSize = read(buffer, DIAG_MAX_RX_PKT_SIZ);

	if (!lastRxSize) {
		printf("Device did not respond\n");
		return DIAG_CMD_RX_FAIL;
	}

	*response = buffer;

	return buffer[0] != DIAG_BAD_PARM_F ? 1 : 0;
}

/**
* @brief sendLgNvUnlock
* @param response - 
* @return
*/
int QcdmSerial::sendLgNvUnlock(uint8_t** response)
{
	if (!isOpen()) {
		return DIAG_CMD_PORT_CLOSED;
	}

	unsigned char data[] = { 0x33, 0x7D, 0x5F };

	lastTxSize = write((uint8_t*)data, sizeof(data));

	if (!lastTxSize) {
		printf("Attempted to write to device but 0 bytes were written\n");
		return DIAG_CMD_TX_FAIL;
	}

	lastRxSize = read(buffer, DIAG_MAX_RX_PKT_SIZ);

	if (!lastRxSize) {
		printf("Device did not respond\n");
		return DIAG_CMD_RX_FAIL;
	}

	*response = buffer;

	return buffer[0] != DIAG_BAD_CMD_F ? 1 : 0;
}

/**
* @brief getLgSpc
* @param response -
* @return
*/
int QcdmSerial::getLgSpc(uint8_t** response)
{
	if (!isOpen()) {
		return DIAG_CMD_PORT_CLOSED;
	}

	unsigned char data[] = { 0x11, 0x17, 0x00, 0x08 };

	lastTxSize = write((uint8_t*)data, sizeof(data));

	if (!lastTxSize) {
		printf("Attempted to write to device but 0 bytes were written\n");
		return DIAG_CMD_TX_FAIL;
	}

	lastRxSize = read(buffer, DIAG_MAX_RX_PKT_SIZ);

	if (!lastRxSize) {
		printf("Device did not respond\n");
		return DIAG_CMD_RX_FAIL;
	}

	*response = buffer;

	return buffer[0] != DIAG_BAD_PARM_F ? 1 : 0;
}

/**
* transformHexToString
* @param input
* @param length
*/
std::string QcdmSerial::transformHexToString(const char *input, int length) 
{
	char outputBuffer[128];
	std::string result;
	int i;

	for (i = 0; i <= length; i++) {
		sprintf(outputBuffer, "%c", hex_trans[input[i]]);
		
		result.append(outputBuffer);
	}

	return result;
}