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
	memset(&state, 0x00, sizeof(state));
	state.hello.maxPreferredBlockSize = STREAMING_DLOAD_MAX_PACKET_SIZE;
}

/**
 * @brief StreamingDloadSerial::~StreamingDloadSerial
 */
StreamingDloadSerial::~StreamingDloadSerial()
{

}


/**
* @brief StreamingDloadSerial::~StreamingDloadSerial
*/
int StreamingDloadSerial::sendHello(std::string magic, uint8_t version, uint8_t compatibleVersion, uint8_t featureBits)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}
	
	size_t rxSize, txSize; 
	std::vector<uint8_t> buffer; 
	streaming_dload_hello_tx_t hello;
	
	hello.command = STREAMING_DLOAD_HELLO;
	memcpy(hello.magic, magic.c_str(), magic.size());
	hello.version = version;
	hello.compatibleVersion = compatibleVersion;
	hello.featureBits = featureBits;

	txSize = write((uint8_t*)&hello, sizeof(hello));

	if (!txSize) {
		printf("Wrote 0 bytes\n"); 
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, state.hello.maxPreferredBlockSize);
	
	if (!rxSize) {
		printf("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_HELLO_RESPONSE, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}
		
	streaming_dload_hello_rx_header_t* resp = (streaming_dload_hello_rx_header_t*)&buffer[0];
	
	memcpy(&state.hello, &buffer[0], sizeof(streaming_dload_hello_rx_header_t));
	
	int dataStartIndex = sizeof(streaming_dload_hello_rx_header_t);

	// parse the packet and get the things that are not obvious without calculation
	// flashIdenfier, windowSize, numberOfSectors, sectorSizes, featureBits
	memcpy(&state.hello.flashIdenfier, &buffer[dataStartIndex], state.hello.flashIdLength);
	memcpy(&state.hello.windowSize, &buffer[dataStartIndex + state.hello.flashIdLength], sizeof(state.hello.windowSize));
	memcpy(&state.hello.numberOfSectors, &buffer[dataStartIndex + state.hello.flashIdLength + sizeof(state.hello.windowSize)], sizeof(state.hello.numberOfSectors));

	int sectorSize = 4 * state.hello.numberOfSectors;
	memcpy(&state.hello.sectorSizes, &buffer[dataStartIndex + state.hello.flashIdLength + sizeof(state.hello.windowSize) + sizeof(state.hello.numberOfSectors)], sectorSize-1);
	memcpy(&state.hello.featureBits, &buffer[dataStartIndex + state.hello.flashIdLength + sizeof(state.hello.windowSize) + sizeof(state.hello.numberOfSectors) + sectorSize-1], sizeof(state.hello.featureBits));
	state.hello.featureBits = flip_endian16(state.hello.featureBits);

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

int StreamingDloadSerial::sendUnlock(std::string code)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	size_t txSize, rxSize;
	std::vector<uint8_t> buffer; 
	streaming_dload_unlock_tx_t packet;
	
	packet.command = STREAMING_DLOAD_UNLOCK;
	packet.code = std::stoul(code.c_str(), nullptr, 16);
	
	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		printf("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, state.hello.maxPreferredBlockSize);

	if (!rxSize) {
		printf("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_UNLOCKED, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

int StreamingDloadSerial::setSecurityMode(uint8_t mode)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}
	
	size_t txSize, rxSize;
	std::vector<uint8_t> buffer;
	streaming_dload_security_mode_tx_t packet;
	packet.command = STREAMING_DLOAD_SECURITY_MODE;
	packet.mode = mode;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		printf("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, state.hello.maxPreferredBlockSize);

	if (!rxSize) {
		printf("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_SECUIRTY_MODE_RECEIVED, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

int StreamingDloadSerial::sendReset()
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}
	
	size_t txSize, rxSize;
	std::vector<uint8_t> buffer;
	
	streaming_dload_reset_tx_t packet;
	packet.command = STREAMING_DLOAD_RESET;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		printf("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, state.hello.maxPreferredBlockSize);

	if (!rxSize) {
		printf("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_RESET_ACK, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

int StreamingDloadSerial::sendPowerOff()
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	size_t txSize, rxSize;
	std::vector<uint8_t> buffer;
	streaming_dload_reset_tx_t packet;
	packet.command = STREAMING_DLOAD_POWER_OFF;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		printf("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, state.hello.maxPreferredBlockSize);

	if (!rxSize) {
		printf("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_POWERING_DOWN, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}


int StreamingDloadSerial::sendNop()
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	size_t txSize, rxSize;
	std::vector<uint8_t> buffer;
	streaming_dload_nop_tx_t packet;
	packet.command = STREAMING_DLOAD_NOP;
	packet.identifier = std::rand();

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		printf("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, state.hello.maxPreferredBlockSize);

	if (!rxSize) {
		printf("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_NOP_RESPONSE, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	streaming_dload_nop_rx_t* resp = (streaming_dload_nop_rx_t*)&buffer[0];

	if (resp->identifier  != packet.identifier) {
		printf("Received NOP response but identifier did not match\n");
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

int StreamingDloadSerial::readEcc(uint8_t& statusOut)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}
	
	size_t txSize, rxSize;
	std::vector<uint8_t> buffer; 
	streaming_dload_get_ecc_state_tx_t packet;
	packet.command = STREAMING_DLOAD_GET_ECC_STATE;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		printf("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, state.hello.maxPreferredBlockSize);

	if (!rxSize) {
		printf("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_CURRENT_ECC_STATE, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	streaming_dload_get_ecc_state_rx_t* resp = (streaming_dload_get_ecc_state_rx_t*)&buffer[0];
	
	statusOut = resp->status;

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

int StreamingDloadSerial::setEcc(uint8_t status)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}
	
	size_t txSize, rxSize;
	std::vector<uint8_t> buffer; 
	streaming_dload_set_ecc_state_tx_t packet;
	packet.command = STREAMING_DLOAD_SET_ECC;
	packet.status = status;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		printf("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, state.hello.maxPreferredBlockSize);

	if (!rxSize) {
		printf("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_SET_ECC_RESPONSE, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

int StreamingDloadSerial::openMode(uint8_t mode)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}
	
	size_t txSize, rxSize;
	std::vector<uint8_t> buffer; 	
	streaming_dload_open_tx_t packet;
	packet.command = STREAMING_DLOAD_OPEN;
	packet.mode = mode;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		printf("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, state.hello.maxPreferredBlockSize);

	if (!rxSize) {
		printf("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_OPENED, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

int StreamingDloadSerial::closeMode()
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	size_t txSize, rxSize;
	std::vector<uint8_t> buffer; 
	streaming_dload_close_tx_t packet;
	packet.command = STREAMING_DLOAD_CLOSE;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		printf("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, state.hello.maxPreferredBlockSize);

	if (!rxSize) {
		printf("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_CLOSED, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}



int StreamingDloadSerial::openMultiImage(uint8_t imageType)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	size_t txSize, rxSize;
	std::vector<uint8_t> buffer;
	streaming_dload_open_multi_image_tx_t packet;
	packet.command = STREAMING_DLOAD_OPEN_MULTI_IMAGE;
	packet.type = imageType;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		printf("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, state.hello.maxPreferredBlockSize);

	if (!rxSize) {
		printf("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_OPENED_MULTI_IMAGE, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

int StreamingDloadSerial::readAddress(uint32_t address, size_t length, uint8_t** data, size_t& dataSize, size_t chunkSize)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	bool isComplete = false;
	
	size_t txSize, rxSize;
	uint8_t buffer[STREAMING_DLOAD_MAX_PACKET_SIZE];

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

		txSize = write((uint8_t*)&packet, sizeof(packet));

		if (!txSize) {
			printf("Wrote 0 bytes\n");
			free(out); 
			return STREAMING_DLOAD_OPERATION_IO_ERROR;
		}

		rxSize = read(buffer, state.hello.maxPreferredBlockSize);

		if (!rxSize) {
			printf("Device did not respond\n");
			free(out);
			return STREAMING_DLOAD_OPERATION_IO_ERROR;
		}

		if (!isValidResponse(STREAMING_DLOAD_READ_DATA, buffer, rxSize)) {
			printf("Invalid Response\n");
			free(out);
			return STREAMING_DLOAD_OPERATION_ERROR;
		}
		
		lastAddress = packet.address;

		size_t newSize = dataSize + rxSize;
		if (newSize > outSize) {			 
			out = (uint8_t*)realloc(out, newSize);
			outSize = newSize;
		}

		readRx = (streaming_dload_read_rx_t*)&buffer[0];

		overhead = (sizeof(readRx->address) + sizeof(readRx->command));

		memcpy(&out[dataSize], readRx->data, rxSize - overhead);
		dataSize += (rxSize - overhead);

		if (length <= dataSize) {
			isComplete = true;
			break; //done
		}
			
	} while (!isComplete);

	*data = out;
	
	printf("\n\n\nFinal Data Size: %lu bytes\n", dataSize);
	hexdump(out, dataSize);

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

int StreamingDloadSerial::readAddress(uint32_t address, size_t length, std::vector<uint8_t> &out, size_t chunkSize)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	bool isComplete = false;

	size_t txSize, rxSize;
	size_t totalRead = 0;

	streaming_dload_read_tx_t packet;
	packet.command = STREAMING_DLOAD_READ;
	packet.address = address;
	streaming_dload_read_rx_t* readRx;

	do {
		packet.address = packet.address + totalRead;
		packet.length = length <= chunkSize ? length : chunkSize;

		txSize = write((uint8_t*)&packet, sizeof(packet));

		if (!txSize) {
			printf("Wrote 0 bytes\n");
			return STREAMING_DLOAD_OPERATION_IO_ERROR;
		}

		rxSize = read(out, state.hello.maxPreferredBlockSize);

		if (!rxSize) {
			printf("Device did not respond\n");
			return STREAMING_DLOAD_OPERATION_IO_ERROR;
		}
		totalRead += rxSize;

		if (!isValidResponse(STREAMING_DLOAD_READ_DATA, out)) {
			printf("Invalid Response\n");
			return STREAMING_DLOAD_OPERATION_ERROR;
		}
		


		if (length <= totalRead) {
			break; //done
		}

	} while (true);


	printf("\n\n\nFinal Data Size: %lu bytes\n", totalRead);
	hexdump(&out[0], totalRead);

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

int StreamingDloadSerial::writePartitionTable(std::string fileName, uint8_t& outStatus, bool overwrite)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}
	
#ifdef _WIN32
	FILE* fp;
	fopen_s(&fp, fileName.c_str(), "rb");
#else
	FILE* fp = fopen(fileName.c_str(), "rb");
#endif

	if (!fp) {
		printf("Could Not Open File %s\n", fileName.c_str());
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	fseek(fp, 0, SEEK_END);
	size_t fileSize = ftell(fp);
	rewind(fp);

	if (fileSize > 512) {
		printf("File can\'t exceed 512 bytes");
		fclose(fp);
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	size_t txSize, rxSize;
	std::vector<uint8_t> buffer;
	streaming_partition_table_tx_t packet;
	packet.command = STREAMING_DLOAD_PARTITION_TABLE;
	packet.overrideExisting = overwrite;
	fread(&packet.data, sizeof(uint8_t), fileSize, fp);
	fclose(fp);

	txSize = write((uint8_t*)&packet, sizeof(packet));
	
	if (!txSize) {
		printf("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, state.hello.maxPreferredBlockSize);

	if (!rxSize) {
		printf("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_PARTITION_TABLE_RECEIVED, &buffer[0], rxSize)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	streaming_dload_partition_table_rx_t* resp = (streaming_dload_partition_table_rx_t*)&buffer[0];

	outStatus = resp->status;

	if (resp->status > 1) {
		printf("Received Error Response %02X\n", resp->status);
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
	
}

int StreamingDloadSerial::readQfprom(uint32_t rowAddress, uint32_t addressType)
{
	if (!isOpen()) {
		printf("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	size_t txSize, rxSize;
	std::vector<uint8_t> buffer; 
	streaming_dload_qfprom_read_tx_t packet;
	packet.command = STREAMING_DLOAD_QFPROM_READ;
	packet.addressType = addressType;
	packet.rowAddress = rowAddress;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		printf("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, state.hello.maxPreferredBlockSize);

	if (!rxSize) {
		printf("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_QFPROM_READ_RESPONSE, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

bool StreamingDloadSerial::isValidResponse(uint8_t expectedCommand, uint8_t* response, size_t responseSize)
{
	if (response[0] != expectedCommand) {
		if (response[0] == STREAMING_DLOAD_LOG) {
			streaming_dload_log_rx_t* packet = (streaming_dload_log_rx_t*)response;
			printf("Received Log Response\n");
			memcpy((uint8_t*)&lastLog, packet, responseSize);
		} else if (response[0] == STREAMING_DLOAD_ERROR) {
			streaming_dload_error_rx_t* packet = (streaming_dload_error_rx_t*)response;
			printf("Received Error Response %02X - %s\n", packet->code, getNamedError(packet->code));
			memcpy((uint8_t*)&lastError, packet, responseSize);
		} else {
			printf("Unexpected Response\n");
		}
		return false;
	}

	return true;
}

bool StreamingDloadSerial::isValidResponse(uint8_t expectedCommand, std::vector<uint8_t> &data)
{
	return isValidResponse(expectedCommand, &data[0], data.size());
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
		case STREAMING_DLOAD_OPEN_MULTI_MODE_DBL:			return "DBL image for Secure Boot 2.0";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_OSBL:			return "OSBL image for Secure Boot 2.0";
		case STREAMING_DLOAD_OPEN_MULTI_MODE_FSBL:			return "FSBL image for Secure Boot 2.0";
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