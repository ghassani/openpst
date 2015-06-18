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
	buffer = new uint8_t[2048]; // this will get resized after hello if needed
	bufferSize = 2048;
	memset(&deviceState, 0x00, sizeof(deviceState));
	memset(&lastError,	 0x00, sizeof(lastError));
	memset(&lastLog,	 0x00, sizeof(lastLog));
}

/**
 * @brief StreamingDloadSerial::~StreamingDloadSerial
 */
StreamingDloadSerial::~StreamingDloadSerial()
{
	delete buffer;
}



int StreamingDloadSerial::sendHello(std::string magic, uint8_t version, uint8_t compatibleVersion, uint8_t featureBits)
{
	streaming_dload_hello_tx_t hello;

	hello.command = STREAMING_DLOAD_HELLO;
	memcpy(hello.magic, magic.c_str(), magic.size());
	hello.version = version;
	hello.compatibleVersion = compatibleVersion;
	hello.featureBits = featureBits;

	lastTxSize = write((uint8_t*)&hello, sizeof(hello));

	if (!lastTxSize) {
		printf("Wrote 0 bytes\n"); 
		return -1;
	}

	lastRxSize = read(buffer, bufferSize);
	
	if (!lastRxSize) {
		printf("Device did not response\n");
		return -1;
	}

	if (!isValidResponse(STREAMING_DLOAD_HELLO_RESPONSE, buffer, lastRxSize)) {
		return 0;
	}

	int packetStartIndex = (buffer[0] == HDLC_CONTROL_CHAR ? 1 : 0);

	streaming_dload_hello_rx_header_t* resp = (streaming_dload_hello_rx_header_t*)buffer;
	memcpy((uint8_t*)&deviceState, &buffer[packetStartIndex], sizeof(streaming_dload_hello_rx_header_t));
	
	if (bufferSize < deviceState.maxPreferredBlockSize && deviceState.maxPreferredBlockSize > 0) {
		buffer = (uint8_t*) realloc(buffer, deviceState.maxPreferredBlockSize);
		bufferSize = deviceState.maxPreferredBlockSize;
	}

	int dataStartIndex = (packetStartIndex + sizeof(streaming_dload_hello_rx_header_t));

	// get the flashIdenfier, windowSize, numberOfSectors, sectorSizes, featureBits
	memcpy(&deviceState.flashIdenfier, &buffer[dataStartIndex], deviceState.flashIdLength);	
	memcpy(&deviceState.windowSize, &buffer[dataStartIndex + deviceState.flashIdLength], sizeof(deviceState.windowSize));
	memcpy(&deviceState.numberOfSectors, &buffer[dataStartIndex + deviceState.flashIdLength + sizeof(deviceState.windowSize)], sizeof(deviceState.numberOfSectors));

	int sectorSize = 4 * deviceState.numberOfSectors;
	memcpy(&deviceState.sectorSizes, &buffer[dataStartIndex + deviceState.flashIdLength + sizeof(deviceState.windowSize) + sizeof(deviceState.numberOfSectors)], sectorSize-1);
	memcpy(&deviceState.featureBits, &buffer[dataStartIndex + deviceState.flashIdLength + sizeof(deviceState.windowSize) + sizeof(deviceState.numberOfSectors) + sectorSize-1], sizeof(deviceState.featureBits));
	deviceState.featureBits = flip_endian16(deviceState.featureBits);
	return 1;
}

int StreamingDloadSerial::sendUnlock(std::string code)
{

	streaming_dload_unlock_tx_t packet;
	packet.command = STREAMING_DLOAD_UNLOCK;
	packet.code = std::stoul(code.c_str(), nullptr, 16);
	
	lastTxSize = write((uint8_t*)&packet, sizeof(packet));

	if (!lastTxSize) {
		printf("Wrote 0 bytes\n");
		return -1;
	}

	lastRxSize = read(buffer, bufferSize);

	if (!lastRxSize) {
		printf("Device did not response\n");
		return -1;
	}

	return 1;
}

int StreamingDloadSerial::setSecurityMode(uint8_t mode)
{
	streaming_dload_security_mode_tx_t packet;
	packet.command = STREAMING_DLOAD_SECURITY_MODE;
	packet.mode = mode;

	lastTxSize = write((uint8_t*)&packet, sizeof(packet));

	if (!lastTxSize) {
		printf("Wrote 0 bytes\n");
		return -1;
	}

	lastRxSize = read(buffer, bufferSize);

	if (!lastRxSize) {
		printf("Device did not response\n");
		return -1;
	}

	return 1;

	return 1;
}

bool StreamingDloadSerial::isValidResponse(uint8_t expectedCommand, uint8_t* response, size_t& responseSize)
{
	int idx = response[0] == HDLC_CONTROL_CHAR ? 1 : 0;

	if (response[idx] != expectedCommand) {
		if (response[idx] == STREAMING_DLOAD_LOG) {
			streaming_dload_log_rx_t* packet = (streaming_dload_log_rx_t*)&response[1];
			memcpy((uint8_t*)&lastLog, &buffer[idx], (responseSize - (HDLC_TRAILER_LENGTH + idx)));
		} else if (response[idx] == STREAMING_DLOAD_ERROR) {
			streaming_dload_error_rx_t* packet = (streaming_dload_error_rx_t*)&response[idx];
			printf("Received Error Response %02X - %s\n", packet->code, getNamedError(packet->code));
			memcpy((uint8_t*)&lastError, &response[idx], (responseSize - (HDLC_TRAILER_LENGTH + idx)));
		} else {
			printf("Unexpected Response\n");
		}
		return false;
	}

	return true;
}


const char* StreamingDloadSerial::getNamedError(uint8_t code)
{
	switch (code) {
		case STREAMING_DLOAD_ERROR_ERROR_INVALID_DESTINATION_ADDRESS:		return "Invalid Destination Address";
		case STREAMING_DLOAD_ERROR_INVALID_LENGTH:							return "Invalid Length";
		case STREAMING_DLOAD_ERROR_UNEXPECTED_END_OF_PACKET:				return "Unexpected End of Packet";
		case STREAMING_DLOAD_ERROR_INVALID_COMMAND:							return "Invalid Command";
		case STREAMING_DLOAD_ERROR_WRONG_FLASH_INTELLIGENT_ID:				return "Wrong Flash Intelligent ID";
		case STREAMING_DLOAD_ERROR_BAD_PROGRAMMING_VOLTAGE:					return "Bad Programming Voltage";
		case STREAMING_DLOAD_ERROR_WRITE_VERIFY_FAILED:						return "Write Verify Failed";
		case STREAMING_DLOAD_ERROR_INCORRECT_SECURITY_CODE:					return "Incorrect Security Code";
		case STREAMING_DLOAD_ERROR_CANNOT_POWER_DOWN:						return "Cannot Power Down";
		case STREAMING_DLOAD_ERROR_NAND_FLASH_PROGRAMMING_NOT_SUPPORTED:	return "NAND Flash Programming Not Supported";
		case STREAMING_DLOAD_ERROR_COMMAND_OUT_OF_SQUENCE:					return "Command Out of Sequence";
		case STREAMING_DLOAD_ERROR_CLOSE_DID_NOT_SUCCEED:					return "Close Did Not Succeed";
		case STREAMING_DLOAD_ERROR_INCOMPATIBLE_FEATURES_BITS:				return "Incompatible Features Bits";
		case STREAMING_DLOAD_ERROR_OUT_OF_SPACE:							return "Out of Space";
		case STREAMING_DLOAD_ERROR_INVALID_SECURITY_MODE:					return "Invalid Security Mode";
		case STREAMING_DLOAD_ERROR_MULTI_IMAGE_NAND_NOT_SUPPORTED:			return "Multi Image NAND Not Supported";
		case STREAMING_DLOAD_ERROR_POWER_OFF_COMMAND_NOT_SUPPORTED:			return "Power Off Command Not Supported";
		default: 
			return "Unknown";
	}
}
