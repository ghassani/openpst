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

using namespace OpenPST;

/**
* @brief QcdmSerial
*
* @param std::string port
* @param int baudrate
* @param serial::Timeout - Timeout, defaults to 1000ms
*/
QcdmSerial::QcdmSerial(std::string port, int baudrate, serial::Timeout timeout) :
    HdlcSerial (port, baudrate, timeout)
{

}

/**
 * @brief QcdmSerial::~QcdmSerial
 */
QcdmSerial::~QcdmSerial()
{

}

int QcdmSerial::getVersion(QcdmVersionResponse& response)
{
	if (!isOpen()) {
		return kQcdmIOError;
	}

	int result = sendCommand(DIAG_VERNO_F);

	if (result != kQcdmSuccess) {
		return result;
	}

	std::memcpy(&response, buffer, sizeof(response));

	return kQcdmSuccess;
}

/**
 * @brief QcdmSerial::sendSpc
 * @param spc - a 6 digit SPC code to unlock service programming
 * @return SPC status response
 */
int QcdmSerial::sendSpc(const char* spc)
{
    if (!isOpen()) {
		return kQcdmIOError;
    }

	operationMutex.lock();

	if (strlen(spc) != 6) {
		LOGE("SPC must be 6 digits\n");
		return kQcdmError;
	}

	QcdmSpcRequest packet = { };
	packet.command = DIAG_SPC_F;
    std::memcpy(&packet.spc, spc, 6);

	if (!write((uint8_t*)&packet, sizeof(packet))) {
		operationMutex.unlock(); 
		LOGE("Attempted to write to device but 0 bytes were written\n");
        return kQcdmIOError;
    }

    size_t rxSize = read(buffer, DIAG_MAX_PACKET_SIZE);

	if (!rxSize) {
		operationMutex.unlock();
        LOGE("Device did not respond\n");		
		return kQcdmIOError;
    }

    QcdmSpcResponse* rxPacket = (QcdmSpcResponse*) buffer;

	operationMutex.unlock();

    return rxPacket->status ? kQcdmSuccess : kQcdmError;
}

/**
* @brief send16Password
* @param password - a 16 digit password to unlock secure operations
* @return Password status response
*/
int QcdmSerial::sendPassword(const char* password)
{
    if (!isOpen()) {
        return kQcdmIOError;
    }

    long data = std::stoul(password, nullptr, 16);

    #ifdef _WIN32
        data = _byteswap_uint64(data);
    #else
        data = __builtin_bswap64(data);
    #endif

    QcdmPasswordRequest packet;
    memcpy(&packet.password, &data, sizeof(data));

    lastTxSize = write((uint8_t*)&packet, sizeof(packet));

    if (!lastTxSize) {
        printf("Attempted to write to device but 0 bytes were written\n");
        return DIAG_CMD_TX_FAIL;
    }

    lastRxSize = read(buffer, DIAG_MAX_PACKET_SIZE);

    if (!lastRxSize) {
        printf("Device did not respond\n");
        return DIAG_CMD_RX_FAIL;
    }

    QcdmPasswordResponse* rxPacket = (QcdmPasswordResponse*)buffer;

    return rxPacket->status;
}

/**
* @brief sendQcdmPhoneMode
* @param mode - DIAG_PHONE_MODE
* @return
*/
int QcdmSerial::sendPhoneMode(uint8_t mode)
{
    if (!isOpen()) {
        return DIAG_CMD_PORT_CLOSED;
    }

    QcdmPhoneModeRequest packet;
    packet.mode = mode;

    lastTxSize = write((uint8_t*)&packet, sizeof(packet));

    if (!lastTxSize) {
        printf("Attempted to write to device but 0 bytes were written\n");
        return DIAG_CMD_TX_FAIL;
    }

    lastRxSize = read(buffer, DIAG_MAX_PACKET_SIZE);

    if (!lastRxSize) {
        printf("Device did not respond\n");
        return DIAG_CMD_RX_FAIL;
    }

    QcdmPhoneModeResponse* rxPacket = (QcdmPhoneModeResponse*)buffer;

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

    QcdmNvRequest packet;
    packet.cmd = DIAG_NV_READ_F;
    packet.nvItem = itemId;

    lastTxSize = write((uint8_t*)&packet, sizeof(packet));

    if (!lastTxSize) {
        printf("Attempted to write to device but 0 bytes were written\n");
        return DIAG_CMD_TX_FAIL;
    }

        lastRxSize = read(buffer, DIAG_MAX_PACKET_SIZE);

    if (!lastRxSize) {
        printf("Device did not respond\n");
        return DIAG_CMD_RX_FAIL;
    }

    *response = buffer;

    QcdmNvResponse* rxPacket = (QcdmNvResponse*)buffer;

    return rxPacket->cmd;
}

/**
* @brief setNvItem
* @param itemId = NV Item ID
* @param response = Full QCDM response
* @return QCDM command response status
*/
int QcdmSerial::setNvItem(int itemId, const char *data, int length) {
    uint8_t* resp = nullptr;
	
	//TODO: fix this
    setNvItem(itemId, data, length, &resp);
	return 0;
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

    QcdmNvRequest packet;
    packet.cmd = DIAG_NV_WRITE_F;
    packet.nvItem = itemId;
    memcpy(&packet.data, data, length);

    lastTxSize = write((uint8_t*)&packet, sizeof(packet));

    if (!lastTxSize) {
        printf("Attempted to write to device but 0 bytes were written\n");
        return DIAG_CMD_TX_FAIL;
    }

    lastRxSize = read(buffer, DIAG_MAX_PACKET_SIZE);

    if (!lastRxSize) {
        printf("Device did not respond\n");
        return DIAG_CMD_RX_FAIL;
    }

    *response = buffer;

    QcdmNvResponse* rxPacket = (QcdmNvResponse*)buffer;

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

    lastRxSize = read(buffer, DIAG_MAX_PACKET_SIZE);

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

    lastRxSize = read(buffer, DIAG_MAX_PACKET_SIZE);

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

    unsigned char data[] = { 0x11, 0x17, 0x0, 0x08 };

    lastTxSize = write((uint8_t*)data, sizeof(data));

    if (!lastTxSize) {
        printf("Attempted to write to device but 0 bytes were written\n");
        return DIAG_CMD_TX_FAIL;
    }

    lastRxSize = read(buffer, DIAG_MAX_PACKET_SIZE);

    if (!lastRxSize) {
        printf("Device did not respond\n");
        return DIAG_CMD_RX_FAIL;
    }

    *response = buffer;

    return buffer[0] != DIAG_BAD_PARM_F ? 1 : 0;
}

int QcdmSerial::sendCommand(uint8_t command)
{
	QcdmGenericRequest packet = { command };

	return sendCommand(command, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
}

int QcdmSerial::sendCommand(uint8_t command, uint8_t* data, size_t size)
{
	if (!isOpen() || !write(data, size)) {
		return kQcdmIOError;
	}

	size_t rxSize = read(buffer, DIAG_MAX_PACKET_SIZE);

	if (!rxSize) {
		return kQcdmIOError;
	}

	if (!isValidResponse(command, buffer, rxSize)) {
		return kQcdmError;
	}

	return kQcdmSuccess;
}

bool QcdmSerial::isValidResponse(uint8_t command, uint8_t* response, size_t size)
{
	
	if (command != response[0]) {
		switch (response[0]) {
			case DIAG_BAD_CMD_F:
				LOGE("Bad Command Response for Command %d", command);
				break;
			case DIAG_BAD_PARM_F:
				LOGE("Bad Parameter Response for Command %d", command);
				break;
			case DIAG_BAD_LEN_F:
				LOGE("Bad Length Response for Command %d", command);
				break;
			case DIAG_BAD_MODE_F:
				LOGE("Bad Mode Response for Command %d", command);
				break;
			default:
				LOGE("Unexpected Response for Command %d Received %d", command, response[0]);
				break;
		}
		return kQcdmError;
	}

	return true;
}

int QcdmSerial::getErrorLog()
{
	/*
	PACKED(typedef struct QcdmLogResponse{
		uint8_t command;
		uint8_t entries;
		uint16_t  length;
		uint8_t  logs[0];
	}) QcdmLogResponse;*/

	//DIAG_FEATURE_QUERY_F
	//DIAG_LOG_F
	QcdmGenericRequest packet = { DIAG_LOG_F };

	write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	size_t rx = read(buffer, DIAG_MAX_PACKET_SIZE);
	hexdump(buffer, rx);


	QcdmLogResponse* response = (QcdmLogResponse*)buffer;

	LOGI("Cmd: %d\n", response->command);
	LOGI("Entries: %d\n", response->entries);
	LOGI("Size: %lu\n", response->length);

	//hexdump(&response->logs, response->length);

	return kQcdmSuccess;
}


