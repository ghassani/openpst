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
using serial::IOException;

QcdmSerial::QcdmSerial(std::string port, int baudrate, serial::Timeout timeout) :
    HdlcSerial (port, baudrate, timeout)
{

}

QcdmSerial::~QcdmSerial()
{

}

QcdmVersionResponse QcdmSerial::getVersion()
{
	QcdmVersionResponse response;

	sendCommand(DIAG_VERNO_F);

    std::memcpy(&response, buffer, sizeof(response));

	return response;
} 

uint16_t QcdmSerial::getDiagVersion()
{
	sendCommand(DIAG_DIAG_VER_F);

	return ((QcdmDiagVersionResponse*)&buffer)->version;
}

QcdmStatusResponse QcdmSerial::getStatus()
{
	QcdmStatusResponse response = {};

	sendCommand(DIAG_STATUS_F);

	std::memcpy(&response, buffer, sizeof(response));

	return response;
}

QcdmGuidResponse QcdmSerial::getGuid()
{
	QcdmGuidResponse response = {};

	sendCommand(DIAG_GET_GUID_F);

	std::memcpy(&response, buffer, sizeof(response));

	return response;
}

bool QcdmSerial::sendSpc(std::string spc)
{
	QcdmSpcRequest packet = {};
	QcdmSpcResponse* response;
	size_t responseSize;

	if (spc.length() != 6) {
		throw QcdmInvalidArgument("SPC must be 6 digits");
    }

	packet.command = DIAG_SPC_F;
    std::memcpy(&packet.spc, spc.c_str(), 6);

	sendCommand(packet.command, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	response = (QcdmSpcResponse*)buffer;

	return response->status == 1;
}

bool QcdmSerial::sendPassword(std::string password)
{
	if (password.length() != 16) {
		throw QcdmInvalidArgument("Password must be 16 digits");
	}

	return false;
	/*
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
	*/
}

void QcdmSerial::switchToDload()
{
	sendCommand(DIAG_DLOAD_F, false);
}

bool QcdmSerial::setPhoneMode(QcdmPhoneMode mode)
{
	QcdmPhoneModeResponse* response;
	QcdmPhoneModeRequest packet;

	packet.command = DIAG_CONTROL_F;
    packet.mode	   = mode;

	sendCommand(packet.command, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	response = (QcdmPhoneModeResponse*)buffer;

	return response->status;
}

QcdmNvResponse QcdmSerial::readNV(uint16_t itemId)
{
	QcdmNvResponse response = {};
	QcdmNvRequest packet = {};

	packet.command	= DIAG_NV_READ_F;
    packet.nvItem	= itemId;

	sendCommand(packet.command, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	std::memcpy(&response, buffer, sizeof(QcdmNvResponse));

	return response;
}


bool QcdmSerial::writeNV(uint16_t itemId, uint8_t* data, size_t size)
{
	QcdmNvRequest packet;
	
	if (size > DIAG_NV_ITEM_SIZE) {
		throw QcdmInvalidArgument("Data is larger than DIAG_NV_ITEM_SIZE");
	}

    packet.command	= DIAG_NV_WRITE_F;
    packet.nvItem	= itemId;
	memcpy(&packet.data, data, size);

	sendCommand(packet.command, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	return true;
}

QcdmNvPeekResponse QcdmSerial::peekNV(uint32_t address, uint8_t size)
{
	QcdmNvPeekRequest packet = {};
	QcdmNvPeekResponse response = {};

	if (size > DIAG_NV_PEEK_MAX_SIZE) {
		throw QcdmInvalidArgument("NV peek request size is over the maximum");
	}

	packet.command = DIAG_NV_PEEK_F;
	packet.address = address;
	packet.size	   = size;
	
	sendCommand(packet.command, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
	
	memcpy(&response, buffer, sizeof(response));
		
	return response;
}

/**
* @brief sendHtcNvUnlock
* @param response -
* @return
*/
bool QcdmSerial::sendHtcNvUnlock()
{
	uint8_t packet[] = { DIAG_SPC_F, 0x74, 0x64, 0x77, 0x61, 0x6F, 0x70 };

	sendCommand(NULL, packet, sizeof(packet));
	
    return true;
}

bool QcdmSerial::sendLgNvUnlock()
{

	uint8_t packet[] = { 0x33, 0x7D, 0x5F };

	sendCommand(NULL, packet, sizeof(packet));

	return true;
}


bool QcdmSerial::getLgSpc()
{
	unsigned char packet[] = { 0x11, 0x17, 0x0, 0x08 };

	sendCommand(NULL, packet, sizeof(packet));

	return true;
}

void QcdmSerial::sendCommand(uint8_t command, bool validate)
{
    QcdmGenericRequest packet = { command };

    return sendCommand(command, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
}

void QcdmSerial::sendCommand(uint8_t command, uint8_t* data, size_t size, bool validate)
{
	QcdmResponseHeader* response = (QcdmResponseHeader*)&buffer;
	size_t responseSize;
	std::string error;

	write(data, size);

	if (!(responseSize = read(buffer, DIAG_MAX_PACKET_SIZE))) {
		throw QcdmResponseError("Device did not respond");
	}

	if (validate) {
		if (command && response->command != command) {
			error = getErrorString(response->command);
			if (error.length()) {
				throw QcdmResponseError(error);
			} else {
				throw QcdmResponseError("Received an unexpected response");
			}
		}
		else if (!command) {
			error = getErrorString(response->command);

			if (error.length()) {
				throw QcdmResponseError(error);
			}
		}
	}
}

std::string QcdmSerial::getErrorString(uint8_t responseCommand)
{
	std::string error;

	switch (responseCommand) {
		case DIAG_BAD_CMD_F:
			error = "Bad Command";
			break;
		case DIAG_BAD_PARM_F:
			error = "Bad Parameter";
			break;
		case DIAG_BAD_LEN_F:
			error = "Bad Length";
			break;
		case DIAG_BAD_MODE_F:
			error = "Bad Mode";
			break;
		case DIAG_BAD_SPC_MODE_F:
			error = "Bad SPC Mode";
		default:
			error = "";
			break;
	}

	return error;
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


