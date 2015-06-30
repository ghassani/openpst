/**
* LICENSE PLACEHOLDER
*
* @file streaming_dload_serial.cpp
* @class StreamingDloadSerial
* @package OpenPST
* @brief Streaming DLOAD protocol serial port implementation
*
* @author Gassan Idriss <ghassani@gmail.com>
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
	buffer = new uint8_t[1024]; // this will get resized after hello if needed
	bufferSize = 1024;
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


/**
* @brief StreamingDloadSerial::~StreamingDloadSerial
*/
int StreamingDloadSerial::sendHello(std::string magic, uint8_t version, uint8_t compatibleVersion, uint8_t featureBits)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return -1;
	}
	
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
		printf("Device did not respond\n");
		return -1;
	}

	if (!isValidResponse(STREAMING_DLOAD_HELLO_RESPONSE, buffer, lastRxSize)) {
		return 0;
	}

	int idx = (buffer[0] == HDLC_CONTROL_CHAR ? 1 : 0);

	streaming_dload_hello_rx_header_t* resp = (streaming_dload_hello_rx_header_t*)buffer[idx];
	memcpy((uint8_t*)&deviceState, resp, sizeof(streaming_dload_hello_rx_header_t));
	
	if (bufferSize < deviceState.maxPreferredBlockSize && deviceState.maxPreferredBlockSize > 0) {
		buffer = (uint8_t*) realloc(buffer, deviceState.maxPreferredBlockSize);
		bufferSize = deviceState.maxPreferredBlockSize;
	}

	int dataStartIndex = (idx + sizeof(streaming_dload_hello_rx_header_t));

	// parse the packet and get the things that are not obvious without calculation
	// flashIdenfier, windowSize, numberOfSectors, sectorSizes, featureBits
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
	if (!isOpen()) {
		printf("Port Not Open\n");
		return -1;
	}

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
		printf("Device did not respond\n");
		return -1;
	}

	if (!isValidResponse(STREAMING_DLOAD_UNLOCKED, buffer, lastRxSize)) {
		return 0;
	}

	return 1;
}

int StreamingDloadSerial::setSecurityMode(uint8_t mode)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return -1;
	}
	
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
		printf("Device did not respond\n");
		return -1;
	}

	if (!isValidResponse(STREAMING_DLOAD_SECUIRTY_MODE_RECEIVED, buffer, lastRxSize)) {
		return 0;
	}

	return 1;
}

int StreamingDloadSerial::sendReset()
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return -1;
	}
	
	streaming_dload_reset_tx_t packet;
	packet.command = STREAMING_DLOAD_RESET;

	lastTxSize = write((uint8_t*)&packet, sizeof(packet));

	if (!lastTxSize) {
		printf("Wrote 0 bytes\n");
		return -1;
	}

	lastRxSize = read(buffer, bufferSize);

	if (!lastRxSize) {
		printf("Device did not respond\n");
		return -1;
	}

	if (!isValidResponse(STREAMING_DLOAD_RESET_ACK, buffer, lastRxSize)) {
		return 0;
	}

	return 1;
}

int StreamingDloadSerial::sendPowerOff()
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return -1;
	}
	
	streaming_dload_reset_tx_t packet;
	packet.command = STREAMING_DLOAD_POWER_OFF;

	lastTxSize = write((uint8_t*)&packet, sizeof(packet));

	if (!lastTxSize) {
		printf("Wrote 0 bytes\n");
		return -1;
	}

	lastRxSize = read(buffer, bufferSize);

	if (!lastRxSize) {
		printf("Device did not respond\n");
		return -1;
	}

	if (!isValidResponse(STREAMING_DLOAD_POWERING_DOWN, buffer, lastRxSize)) {
		return 0;
	}

	return 1;
}


int StreamingDloadSerial::sendNop()
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return -1;
	}
	
	streaming_dload_nop_tx_t packet;
	packet.command = STREAMING_DLOAD_NOP;
	packet.identifier = std::rand();

	lastTxSize = write((uint8_t*)&packet, sizeof(packet));

	if (!lastTxSize) {
		printf("Wrote 0 bytes\n");
		return -1;
	}

	lastRxSize = read(buffer, bufferSize);

	if (!lastRxSize) {
		printf("Device did not respond\n");
		return -1;
	}

	if (!isValidResponse(STREAMING_DLOAD_NOP_RESPONSE, buffer, lastRxSize)) {
		return 0;
	}

	streaming_dload_nop_rx_t* resp = (streaming_dload_nop_rx_t*)&buffer[buffer[0] == HDLC_CONTROL_CHAR ? 1 : 0];

	if (resp->identifier  != packet.identifier) {
		printf("Received NOP response but identifier did not match\n");
	}

	return 1;
}

int StreamingDloadSerial::readEcc(uint8_t& statusOut)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return -1;
	}
	
	streaming_dload_get_ecc_state_tx_t packet;
	packet.command = STREAMING_DLOAD_GET_ECC_STATE;

	lastTxSize = write((uint8_t*)&packet, sizeof(packet));

	if (!lastTxSize) {
		printf("Wrote 0 bytes\n");
		return -1;
	}

	lastRxSize = read(buffer, bufferSize);

	if (!lastRxSize) {
		printf("Device did not respond\n");
		return -1;
	}

	if (!isValidResponse(STREAMING_DLOAD_CURRENT_ECC_STATE, buffer, lastRxSize)) {
		return 0;
	}

	streaming_dload_get_ecc_state_rx_t* resp = (streaming_dload_get_ecc_state_rx_t*)&buffer[buffer[0] == HDLC_CONTROL_CHAR ? 1 : 0];
	
	statusOut = resp->status;

	return 1;
}

int StreamingDloadSerial::setEcc(uint8_t status)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return -1;
	}
	
	streaming_dload_set_ecc_state_tx_t packet;
	packet.command = STREAMING_DLOAD_SET_ECC;
	packet.status = status;

	lastTxSize = write((uint8_t*)&packet, sizeof(packet));

	if (!lastTxSize) {
		printf("Wrote 0 bytes\n");
		return -1;
	}

	lastRxSize = read(buffer, bufferSize);

	if (!lastRxSize) {
		printf("Device did not respond\n");
		return -1;
	}

	if (!isValidResponse(STREAMING_DLOAD_SET_ECC_RESPONSE, buffer, lastRxSize)) {
		return 0;
	}

	return 1;
}

int StreamingDloadSerial::openMode(uint8_t mode)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return -1;
	}
	
	streaming_dload_open_tx_t packet;
	packet.command = STREAMING_DLOAD_OPEN;
	packet.mode = mode;

	lastTxSize = write((uint8_t*)&packet, sizeof(packet));

	if (!lastTxSize) {
		printf("Wrote 0 bytes\n");
		return -1;
	}

	lastRxSize = read(buffer, bufferSize);

	if (!lastRxSize) {
		printf("Device did not respond\n");
		return -1;
	}

	if (!isValidResponse(STREAMING_DLOAD_OPENED, buffer, lastRxSize)) {
		return 0;
	}

	return 1;
}

int StreamingDloadSerial::closeMode()
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return -1;
	}

	streaming_dload_close_tx_t packet;
	packet.command = STREAMING_DLOAD_CLOSE;

	lastTxSize = write((uint8_t*)&packet, sizeof(packet));

	if (!lastTxSize) {
		printf("Wrote 0 bytes\n");
		return -1;
	}

	lastRxSize = read(buffer, bufferSize);

	if (!lastRxSize) {
		printf("Device did not respond\n");
		return -1;
	}

	if (!isValidResponse(STREAMING_DLOAD_CLOSED, buffer, lastRxSize)) {
		return 0;
	}

	return 1;
}



int StreamingDloadSerial::openMultiImage(uint8_t imageType)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return -1;
	}

	streaming_dload_open_multi_image_tx_t packet;
	packet.command = STREAMING_DLOAD_OPEN_MULTI_IMAGE;
	packet.type = imageType;

	lastTxSize = write((uint8_t*)&packet, sizeof(packet));

	if (!lastTxSize) {
		printf("Wrote 0 bytes\n");
		return -1;
	}

	lastRxSize = read(buffer, bufferSize);

	if (!lastRxSize) {
		printf("Device did not respond\n");
		return -1;
	}

	if (!isValidResponse(STREAMING_DLOAD_OPENED_MULTI_IMAGE, buffer, lastRxSize)) {
		return 0;
	}

	return 1;
}

int StreamingDloadSerial::readAddress(uint32_t address, size_t length, uint8_t** data, size_t& dataSize, size_t chunkSize)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return -1;
	}

	bool isComplete = false;
	
	uint32_t lastAddress = 0;

	uint8_t* out = new uint8_t[length];
	size_t outSize = length;
	dataSize = 0;

	streaming_dload_read_tx_t packet;
	packet.command = STREAMING_DLOAD_READ;

	streaming_dload_read_rx_t* readRx;
	int overhead;
	bool isFirstRead = true;

	do {
		packet.address = isFirstRead == true ? address : lastAddress + chunkSize;
		packet.length = length <= chunkSize ? length : chunkSize;
		isFirstRead = false;

		lastTxSize = write((uint8_t*)&packet, sizeof(packet));

		if (!lastTxSize) {
			printf("Wrote 0 bytes\n");
			free(out); 
			return -1;
		}

		lastRxSize = read(buffer, bufferSize);

		if (!lastRxSize) {
			printf("Device did not respond\n");
			free(out);
			return -1;
		}

		if (!isValidResponse(STREAMING_DLOAD_READ_DATA, buffer, lastRxSize)) {
			printf("Invalid Response\n");
			free(out);
			return 0;
		}
		
		lastAddress = packet.address;

		size_t newSize = dataSize + lastRxSize;
		if (newSize > outSize) {			 
			out = (uint8_t*)realloc(out, newSize);
			outSize = newSize;
		}

		readRx = (streaming_dload_read_rx_t*)&buffer[buffer[0] == HDLC_CONTROL_CHAR ? 1 : 0];

		overhead = (sizeof(readRx->address) + sizeof(readRx->command) + HDLC_OVERHEAD_LENGTH);

		memcpy(&out[dataSize], readRx->data, lastRxSize - overhead);
		dataSize += (lastRxSize - overhead);

		if (length <= dataSize) {
			isComplete = true;
			break; //done
		}
			
	} while (!isComplete);

	*data = out;
	
	printf("\n\n\nFinal Data Size: %lu bytes\n", dataSize);
	hexdump(out, dataSize);

	return 1;
}

int StreamingDloadSerial::writePartitionTable(std::string fileName, uint8_t& outStatus, bool overwrite)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return -1;
	}
	
#ifdef _WIN32
	FILE* fp;
	fopen_s(&fp, fileName.c_str(), "rb");
#else
	FILE* fp = fopen(fileName.c_str(), "rb");
#endif

	if (!fp) {
		printf("Could Not Open File %s\n", fileName.c_str());
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	size_t fileSize = ftell(fp);
	rewind(fp);

	if (fileSize > 512) {
		printf("File can\'t exceed 512 bytes");
		fclose(fp);
		return -1;
	}

	streaming_partition_table_tx_t packet;
	packet.command = STREAMING_DLOAD_PARTITION_TABLE;
	packet.overrideExisting = overwrite;
	fread(&packet.data, sizeof(uint8_t), fileSize, fp);
	fclose(fp);

	lastTxSize = write((uint8_t*)&packet, sizeof(packet));
	
	if (!lastTxSize) {
		printf("Wrote 0 bytes\n");
		return -1;
	}

	lastRxSize = read(buffer, bufferSize);

	if (!lastRxSize) {
		printf("Device did not respond\n");
		return -1;
	}

	if (!isValidResponse(STREAMING_DLOAD_PARTITION_TABLE_RECEIVED, buffer, lastRxSize)) {
		return 0;
	}

	streaming_dload_partition_table_rx_t* resp = (streaming_dload_partition_table_rx_t*)&buffer[buffer[0] == HDLC_CONTROL_CHAR ? 1 : 0];

	outStatus = resp->status;

	if (resp->status > 1) {
		printf("Received Error Response %02X\n", resp->status);
		return 0;
	}


	return 1;
	
}


int StreamingDloadSerial::readQfprom(uint32_t rowAddress, uint32_t addressType)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return -1;
	}

	streaming_dload_qfprom_read_tx_t packet;
	packet.command = STREAMING_DLOAD_QFPROM_READ;
	packet.addressType = addressType;
	packet.rowAddress = rowAddress;

	lastTxSize = write((uint8_t*)&packet, sizeof(packet));

	if (!lastTxSize) {
		printf("Wrote 0 bytes\n");
		return -1;
	}

	lastRxSize = read(buffer, bufferSize);

	if (!lastRxSize) {
		printf("Device did not respond\n");
		return -1;
	}

	if (!isValidResponse(STREAMING_DLOAD_QFPROM_READ_RESPONSE, buffer, lastRxSize)) {
		return 0;
	}

	return 1;
}

bool StreamingDloadSerial::isValidResponse(uint8_t expectedCommand, uint8_t* response, size_t& responseSize)
{
	int idx = response[0] == HDLC_CONTROL_CHAR ? 1 : 0;

	if (response[idx] != expectedCommand) {
		if (response[idx] == STREAMING_DLOAD_LOG) {
			streaming_dload_log_rx_t* packet = (streaming_dload_log_rx_t*)&response[1];
			printf("Received Log Response\n");
			memcpy((uint8_t*)&lastLog, packet, (responseSize - (HDLC_TRAILER_LENGTH)));
		} else if (response[idx] == STREAMING_DLOAD_ERROR) {
			streaming_dload_error_rx_t* packet = (streaming_dload_error_rx_t*)&response[idx];
			printf("Received Error Response %02X - %s\n", packet->code, getNamedError(packet->code));
			memcpy((uint8_t*)&lastError, packet, (responseSize - (HDLC_TRAILER_LENGTH)));
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

const char* StreamingDloadSerial::getNamedOpenMode(uint8_t mode)
{
	switch (mode) {
		case STREAMING_DLOAD_OPEN_MODE_BOOTLOADER_DOWNLOAD:		return "Bootloader Download";
		case STREAMING_DLOAD_OPEN_MODE_BOOTABLE_IMAGE_DOWNLOAD:	return "Bootable Image Download";
		case STREAMING_DLOAD_OPEN_MODE_CEFS_IMAGE_DOWNLOAD:		return "CEFS Image Download";
		default:
			return "Unknown";
	}
}


const char* StreamingDloadSerial::getNamedMultiImage(uint8_t imageType)
{
	switch (imageType) {
		case STREAMING_DLOAD_OPEN_MULTI_MODE_NONE:			return "None";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_PBL:			return "Primary boot loader";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_QCSBLHDCFG:	return "Qualcomm secondary boot loader header and config data";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_QCSBL:			return "Qualcomm secondary boot loader";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_OEMSBL:		return "OEM secondary boot loader";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_AMSS:			return "AMSS modem executable";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_APPS:			return "AMSS applications executable";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_OBL:			return "MSM6250 OTP boot loader";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_FOTAUI:		return "FOTA UI binary";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_CEFS:			return "Compact EFS2 image";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_APPSBL:		return "AMSS applications boot loader";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_APPS_CEFS:		return "Apps CEFS image";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_FLASH_BIN:		return "Flash.bin for Windows Mobile";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_DSP1:			return "DSP1 runtime image";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_CUSTOM:		return "User-defined partition";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_DBL:			return "DBL image for Secure  Boot 2.0";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_OSBL:			return "OSBL image for Secure  Boot 2.0";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_FSBL:			return "FSBL image for Secure  Boot 2.0";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_DSP2:			return "DSP2 executable";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_RAW:			return "Apps EFS2 raw image ";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_ROFS1:			return "ROFS1 - Symbian";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_ROFS2:			return "ROFS2 - Symbian";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_ROFS3:			return "ROFS3 - Symbian";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_USER:		return "EMMC USER partition";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_BOOT0:	return "EMMC BOOT0 partition";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_BOOT1:	return "EMMC BOOT1 partition";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_RPMB:		return "EMMC RPMB";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_GPP1:		return "EMMC GPP1";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_GPP2:		return "EMMC GPP2";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_GPP3:		return "EMMC GPP3";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_GPP4:		return "EMMC GPP4";
		default:
			return "Unknown";
	}
}