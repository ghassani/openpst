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
* @brief StreamingDloadSerial() - Constructor
*
* @param std::string port - The device to connect to
* @param int baudrate - Defaults to 115200
*/
StreamingDloadSerial::StreamingDloadSerial(std::string port, int baudrate) :
    HdlcSerial (port, baudrate)
{
	state = {};
	state.hello.maxPreferredBlockSize = STREAMING_DLOAD_MAX_DATA_SIZE;
}

/**
* @brief ~StreamingDloadSerial() - Deconstructor
*/
StreamingDloadSerial::~StreamingDloadSerial()
{

}


/**
* @brief sendHello - sends the initial handshake for the session
*
* @param std::string magic - The magic handhsake word. Should be QCOM FAST DOWNLOAD HOST
* @param uint8_t version - The max version to be compatible with
* @param uint8_t compatibleVersion - The lowest version to be compatible with
* @param uint8_t featureBits - a set of feature bits of what features to enable.
*	@see qc/streaming_dload.h
*
* @return int
*/
int StreamingDloadSerial::sendHello(std::string magic, uint8_t version, uint8_t compatibleVersion, uint8_t featureBits)
{
	if (!isOpen()) {
		LOGD("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}
	
	size_t rxSize, txSize; 
	std::vector<uint8_t> buffer; 
	streaming_dload_hello_tx_t hello = {};
	
	hello.command = STREAMING_DLOAD_HELLO;
	memcpy(hello.magic, magic.c_str(), magic.size());
	hello.version = version;
	hello.compatibleVersion = compatibleVersion;
	hello.featureBits = featureBits;

	txSize = write((uint8_t*)&hello, sizeof(hello));

	if (!txSize) {
		LOGD("Wrote 0 bytes\n"); 
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE);
	
	if (!rxSize) {
		LOGD("Device did not respond\n");
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

/**
* @brief sendUnlock - send the unlock command for implementations that require it
*
* @param std::string code - The unlock code
*
* @return int
*/
int StreamingDloadSerial::sendUnlock(std::string code)
{
	if (!isOpen()) {
		LOGD("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	size_t txSize, rxSize;
	std::vector<uint8_t> buffer; 
	streaming_dload_unlock_tx_t packet;
	
	packet.command = STREAMING_DLOAD_UNLOCK;
	packet.code = std::stoul(code.c_str(), nullptr, 16);
	
	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		LOGD("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE);

	if (!rxSize) {
		LOGD("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_UNLOCKED, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

/**
* @brief setSecurityMode - set the session security mode
*
* @param uint8_t mode - The security mode to set to
*
* @return int
*/
int StreamingDloadSerial::setSecurityMode(uint8_t mode)
{
	if (!isOpen()) {
		LOGD("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}
	
	size_t txSize, rxSize;
	std::vector<uint8_t> buffer;

	streaming_dload_security_mode_tx_t packet;
	packet.command = STREAMING_DLOAD_SECURITY_MODE;
	packet.mode = mode;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		LOGD("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE);

	if (!rxSize) {
		LOGD("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_SECUIRTY_MODE_RECEIVED, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}


/**
* @brief sendNop - send a NOP
*
* @return int
*/
int StreamingDloadSerial::sendNop()
{
	if (!isOpen()) {
		LOGD("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	size_t txSize, rxSize;
	std::vector<uint8_t> buffer;
	streaming_dload_nop_tx_t packet;
	packet.command = STREAMING_DLOAD_NOP;
	packet.identifier = std::rand();

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		LOGD("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE);

	if (!rxSize) {
		LOGD("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_NOP_RESPONSE, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	streaming_dload_nop_rx_t* resp = (streaming_dload_nop_rx_t*)&buffer[0];

	if (resp->identifier != packet.identifier) {
		LOGD("Received NOP response but identifier did not match\n");
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

/**
* @brief sendReset - send a reset command
*
* @return int
*/
int StreamingDloadSerial::sendReset()
{
	if (!isOpen()) {
		LOGD("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}
	
	size_t txSize, rxSize;
	std::vector<uint8_t> buffer;
	
	streaming_dload_reset_tx_t packet;
	packet.command = STREAMING_DLOAD_RESET;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		LOGD("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE);

	if (!rxSize) {
		LOGD("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_RESET_ACK, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

/**
* @brief sendNop - send a power down command
*
* @return int
*/
int StreamingDloadSerial::sendPowerOff()
{
	if (!isOpen()) {
		LOGD("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	size_t txSize, rxSize;
	std::vector<uint8_t> buffer;
	streaming_dload_reset_tx_t packet;
	packet.command = STREAMING_DLOAD_POWER_OFF;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		LOGD("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE);

	if (!rxSize) {
		LOGD("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_POWERING_DOWN, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

/**
* @brief readEcc - read the current ECC state
*
* @param uint8_t& status - Will be set with the current status
* @return int
*/
int StreamingDloadSerial::readEcc(uint8_t& statusOut)
{
	if (!isOpen()) {
		LOGD("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}
	
	size_t txSize, rxSize;
	std::vector<uint8_t> buffer; 
	streaming_dload_get_ecc_state_tx_t packet;
	packet.command = STREAMING_DLOAD_GET_ECC_STATE;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		LOGD("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE);

	if (!rxSize) {
		LOGD("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_CURRENT_ECC_STATE, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	streaming_dload_get_ecc_state_rx_t* resp = (streaming_dload_get_ecc_state_rx_t*)&buffer[0];
	
	statusOut = resp->status;

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

/**
* @brief setEcc - set the current ECC state
*
* @param uint8_t status - The status to set it to
*
* @return int
*/
int StreamingDloadSerial::setEcc(uint8_t status)
{
	if (!isOpen()) {
		LOGD("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}
	
	size_t txSize, rxSize;
	std::vector<uint8_t> buffer; 
	streaming_dload_set_ecc_state_tx_t packet;
	packet.command = STREAMING_DLOAD_SET_ECC;
	packet.status = status;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		LOGD("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE);

	if (!rxSize) {
		LOGD("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_SET_ECC_RESPONSE, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

/**
* @brief openMode - Open mode
*
* @param uint8_t mode - The mode to open
*	@see qc/streaming_dload.h
* @return int
*/
int StreamingDloadSerial::openMode(uint8_t mode)
{
	if (!isOpen()) {
		LOGD("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}
	
	size_t txSize, rxSize;
	std::vector<uint8_t> buffer; 	
	streaming_dload_open_tx_t packet;
	packet.command = STREAMING_DLOAD_OPEN;
	packet.mode = mode;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		LOGD("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE);

	if (!rxSize) {
		LOGD("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_OPENED, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

/**
* @brief closeMode - Close the current opened mode
*
* @return int
*/
int StreamingDloadSerial::closeMode()
{
	if (!isOpen()) {
		LOGD("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	size_t txSize, rxSize;
	std::vector<uint8_t> buffer; 
	streaming_dload_close_tx_t packet;
	packet.command = STREAMING_DLOAD_CLOSE;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		LOGD("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE);

	if (!rxSize) {
		LOGD("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_CLOSED, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

/**
* @brief openMultiImage - open mode for multi image devices
*
* @param uint8_t imageType - The image type to open
*	@see qc/streaming_dload.h
*
* @return int
*/
int StreamingDloadSerial::openMultiImage(uint8_t imageType)
{
	if (!isOpen()) {
		LOGD("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	size_t txSize, rxSize;
	std::vector<uint8_t> buffer;
	streaming_dload_open_multi_image_tx_t packet;
	packet.command = STREAMING_DLOAD_OPEN_MULTI_IMAGE;
	packet.type = imageType;

	txSize = write((uint8_t*)&packet, sizeof(packet));

	if (!txSize) {
		LOGD("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE);

	if (!rxSize) {
		LOGD("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_OPENED_MULTI_IMAGE, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

/**
* @brief readAddress - Read x bytes from starting address
*					   into a memory allocated array
*
* @param uint32_t address - The starting address
* @param size_t length - The length to read from address
* @param uint8_t** - The memory allocated array containing the read data until success or error encountered.
* @param size_t& - The size of the memory allocated data
* @param size_t chunkSize - The amount to request per read operation. The max size is 1024.
*
* @return int
*/
int StreamingDloadSerial::readAddress(uint32_t address, size_t length, uint8_t** data, size_t& dataSize, size_t chunkSize)
{
	if (!isOpen()) {
		LOGD("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	size_t txSize, rxSize;

	uint8_t* out = new uint8_t[length];
	size_t outSize = length;

	uint8_t buffer[STREAMING_DLOAD_MAX_RX_SIZE];

	dataSize = 0;

	streaming_dload_read_tx_t packet;
	packet.command = STREAMING_DLOAD_READ;

	streaming_dload_read_rx_t* readRx;

	if (chunkSize > state.hello.maxPreferredBlockSize) {
		chunkSize = state.hello.maxPreferredBlockSize;
	}

	do {
		packet.address = address + dataSize;
		packet.length = length <= chunkSize ? length : chunkSize;

		LOGD("Requesting %lu bytes from %08X\n", packet.length, packet.address);

		txSize = write((uint8_t*)&packet, sizeof(packet));

		if (!txSize) {
			LOGD("Wrote 0 bytes requesting to read %lu bytes from 0x%08X\n", packet.length, packet.address);
			return STREAMING_DLOAD_OPERATION_IO_ERROR;
		}

		rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE);

		if (!rxSize) {
			LOGD("Device did not respond\n");
			free(out);
			return STREAMING_DLOAD_OPERATION_IO_ERROR;
		}

		if (!isValidResponse(STREAMING_DLOAD_READ_DATA, buffer, rxSize)) {
			LOGD("Invalid Response\n");
			free(out);
			return STREAMING_DLOAD_OPERATION_ERROR;
		}
		
		size_t newSize = dataSize + rxSize;
		if (newSize > outSize) {			 
			out = (uint8_t*)realloc(out, newSize);
			outSize = newSize;
		}

		readRx = (streaming_dload_read_rx_t*)&buffer[0];

		if (readRx->address != packet.address) {
			LOGD("Packet address and response address differ\n");
			return STREAMING_DLOAD_OPERATION_ERROR;
		}
		
		memcpy(&out[dataSize], readRx->data, rxSize - sizeof(packet));
		dataSize += (rxSize - sizeof(packet));

		if (length <= dataSize) {
			break; //done
		}
			
	} while (true);

	*data = out;
	
	LOGI("\n\n\nFinal Data Size: %lu bytes\n", dataSize);
	hexdump(out, dataSize);

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

/**
* @brief readAddress - Read x bytes from starting address
*					   into a std::vector<uint8_t> container
*
* @param uint32_t address - The starting address
* @param size_t length - The length to read from address
* @param std::vector<uint8_t> &out - The populated vector containing the read data until success or error encountered.
* @param size_t chunkSize - The amount to request per read operation. The max size is 1024.
*
* @return int
*/
int StreamingDloadSerial::readAddress(uint32_t address, size_t length, std::vector<uint8_t> &out, size_t chunkSize)
{
	if (!isOpen()) {
		LOGD("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	size_t txSize, rxSize;

	streaming_dload_read_tx_t packet;
	packet.command = STREAMING_DLOAD_READ;
	packet.address = address;

	streaming_dload_read_rx_t* readRx;

	if (out.size()) {
		out.clear();
	}

	out.reserve(length);

	std::vector<uint8_t> tmp;
	tmp.reserve(packet.length + sizeof(packet));

	if (chunkSize > state.hello.maxPreferredBlockSize) {
		chunkSize = state.hello.maxPreferredBlockSize;
	}

	do {
		packet.address = packet.address + out.size();		
		packet.length = length <= chunkSize ? length : chunkSize;
		
		LOGD("Requesting %lu bytes from %08X\n", packet.length, packet.address);

		txSize = write((uint8_t*)&packet, sizeof(packet));
		
		if (!txSize) {
			LOGD("Wrote 0 bytes requesting to read %lu bytes from 0x%08X\n", packet.length, packet.address);
			return STREAMING_DLOAD_OPERATION_IO_ERROR;
		}

		// read accounting for additional room for the data to be read
		// and the extra packet data in the header of the response
		// and possibly escaped content
		rxSize = read(tmp, STREAMING_DLOAD_MAX_RX_SIZE);

		if (!rxSize) {
			LOGD("Device did not respond to request to read %lu bytes from 0x%08X\n", packet.length, packet.address);
			return STREAMING_DLOAD_OPERATION_IO_ERROR;
		}
		
		if (!isValidResponse(STREAMING_DLOAD_READ_DATA, tmp)) {
			LOGD("Invalid response in request to read %lu bytes from 0x%08X. Data read so far is %lu bytes.\n", packet.length, packet.address, out.size());
			return STREAMING_DLOAD_OPERATION_ERROR;
		}

		readRx = (streaming_dload_read_rx_t*)&tmp[0];

		if (readRx->address != packet.address) {
			LOGD("Packet address and response address differ\n");
			return STREAMING_DLOAD_OPERATION_ERROR;
		}

		// remove the command code, address, and length to only
		// keep the real data
		tmp.erase(tmp.end() - rxSize, (tmp.end() - rxSize) + sizeof(packet));

		out.reserve(out.size() + tmp.size());
		std::copy(tmp.begin(), tmp.end(), std::back_inserter(out));

		if (length <= out.size()) {
			LOGD("Final read size is %lu bytes\n", out.size());
			break; //done
		}

		// clear out the tmp buffer
		// for a possible next read
		tmp.clear();

	} while (true);

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

/**
* @brief readAddress - Read x bytes from starting address
*					   into a file pointer
*
* @param uint32_t address - The starting address
* @param size_t length - The length to read from address
* @param FILE* out - The file pointer to write the data to
* @param size_t& outSize - The amount of bytes written to the file until success or error encountered.
* @param size_t chunkSize - The amount to request per read operation. The max size is 1024.
*
* @return int
*/
int StreamingDloadSerial::readAddress(uint32_t address, size_t length, FILE* out, size_t &outSize, size_t chunkSize)
{
	if (!isOpen()) {
		LOGD("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	size_t txSize, rxSize;

	streaming_dload_read_tx_t packet;
	packet.command = STREAMING_DLOAD_READ;
	packet.address = address;

	streaming_dload_read_rx_t* readRx;

	outSize = 0;

	std::vector<uint8_t> tmp;
	tmp.reserve(packet.length + sizeof(packet));

	if (chunkSize > state.hello.maxPreferredBlockSize) {
		chunkSize = state.hello.maxPreferredBlockSize;
	}

	do {
		packet.address = packet.address + outSize;
		packet.length = length <= chunkSize ? length : chunkSize;

		LOGD("Requesting %lu bytes from %08X\n", packet.length, packet.address);

		txSize = write((uint8_t*)&packet, sizeof(packet));

		if (!txSize) {
			LOGD("Wrote 0 bytes requesting to read %lu bytes from 0x%08X\n", packet.length, packet.address);
			return STREAMING_DLOAD_OPERATION_IO_ERROR;
		}

		rxSize = read(tmp, STREAMING_DLOAD_MAX_RX_SIZE);

		if (!rxSize) {
			LOGD("Device did not respond to request to read %lu bytes from 0x%08X\n", packet.length, packet.address);
			return STREAMING_DLOAD_OPERATION_IO_ERROR;
		}

		if (!isValidResponse(STREAMING_DLOAD_READ_DATA, tmp)) {
			LOGD("Invalid response in request to read %lu bytes from 0x%08X. Data read so far is %lu bytes.\n", packet.length, packet.address, outSize);
			return STREAMING_DLOAD_OPERATION_ERROR;
		}

		readRx = (streaming_dload_read_rx_t*)&tmp[0];

		if (readRx->address != packet.address) {
			LOGD("Packet address and response address differ\n");
			return STREAMING_DLOAD_OPERATION_ERROR;
		}

		// remove the command code, address, and length to only
		// keep the real data
		tmp.erase(tmp.end() - rxSize, (tmp.end() - rxSize) + sizeof(packet));

		size_t bytesWritten = fwrite(&tmp[0], sizeof(uint8_t), tmp.size(), out);

		if (bytesWritten != tmp.size()) {
			LOGD("Error writting to file. Attempted to write %lu bytes but only wrote %lu. Check for sufficient disk space.\n", tmp.size(), bytesWritten);
			return STREAMING_DLOAD_OPERATION_IO_ERROR;
		}

		outSize += bytesWritten;

		if (length <= outSize) {
			LOGD("Final read size is %lu bytes\n", outSize);
			break; //done
		}

		// clear out the tmp buffer
		// for a possible next read
		tmp.clear();

	} while (true);

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

/**
* @brief writePartitionTable - Writes partition table for sessions that require it.
*
* You should send this request with overwrite set to false first and check the outStatus
* If it is ok the write will take place. If it is not ok then to write the file you will
* need to force it with overwrite set to true.
*
* @param std::string filePath - The path to the partition table (max 512 bytes)
* @param uint8_t& - The status reported back by the device.
*	@see qc/streaming_dload.h
* @param bool overwritte - Defaults to false. If true will overwrite even if error is reported.
*/
int StreamingDloadSerial::writePartitionTable(std::string fileName, uint8_t& outStatus, bool overwrite)
{
	if (!isOpen()) {
		LOGD("Port Not Open\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}
	
	std::ifstream file(fileName.c_str(), std::ios::binary);

	if (!file.is_open()) {
		LOGD("Could Not Open File %s\n", fileName.c_str());
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	file.seekg(0, file.end);

	size_t fileSize = file.tellg();
	
	file.seekg(0, file.beg);
	
	if (fileSize > 512) {
		LOGD("File can\'t exceed 512 bytes");
		file.close();
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	size_t txSize, rxSize;
	std::vector<uint8_t> buffer;
	streaming_partition_table_tx_t packet = {};

	packet.command = STREAMING_DLOAD_PARTITION_TABLE;
	packet.overrideExisting = overwrite;

	file.read((char *)&packet.data, fileSize);
	file.close();

	txSize = write((uint8_t*)&packet, sizeof(packet));
	
	if (!txSize) {
		LOGD("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE);

	if (!rxSize) {
		LOGD("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_PARTITION_TABLE_RECEIVED, &buffer[0], rxSize)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	streaming_dload_partition_table_rx_t* resp = (streaming_dload_partition_table_rx_t*)&buffer[0];

	outStatus = resp->status;

	if (resp->status > 1) {
		LOGD("Received Error Response %02X\n", resp->status);
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
	
}

/**
* @brief readQfprom - Havent found a device or mode to use this in
*/
int StreamingDloadSerial::readQfprom(uint32_t rowAddress, uint32_t addressType)
{
	if (!isOpen()) {
		LOGD("Port Not Open\n");
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
		LOGD("Wrote 0 bytes\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	rxSize = read(buffer, STREAMING_DLOAD_MAX_RX_SIZE);

	if (!rxSize) {
		LOGD("Device did not respond\n");
		return STREAMING_DLOAD_OPERATION_IO_ERROR;
	}

	if (!isValidResponse(STREAMING_DLOAD_QFPROM_READ_RESPONSE, buffer)) {
		return STREAMING_DLOAD_OPERATION_ERROR;
	}

	return STREAMING_DLOAD_OPERATION_SUCCESS;
}

/**
* @brief isValidResponse
*
* @param uint8_t expectedCommand,
* @param uint8_t* response
* @param size_t responseSize
*/
bool StreamingDloadSerial::isValidResponse(uint8_t expectedCommand, uint8_t* response, size_t responseSize)
{
	if (response[0] != expectedCommand) {
		if (response[0] == STREAMING_DLOAD_LOG) {
			streaming_dload_log_rx_t* packet = (streaming_dload_log_rx_t*)response;
			LOGD("Received Log Response\n");
			memcpy((uint8_t*)&state.lastLog, packet, responseSize);
		} else if (response[0] == STREAMING_DLOAD_ERROR) {
			streaming_dload_error_rx_t* packet = (streaming_dload_error_rx_t*)response;
			LOGD("Received Error Response %02X - %s\n", packet->code, getNamedError(packet->code));
			memcpy((uint8_t*)&state.lastError, packet, responseSize);
		} else {
			LOGD("Unexpected Response\n");
		}
		return false;
	}

	return true;
}

/**
* @brief isValidResponse
*
* @param uint8_t expectedCommand,
* @param std::vector<uint8_t> response
*/
bool StreamingDloadSerial::isValidResponse(uint8_t expectedCommand, std::vector<uint8_t> &data)
{
	return isValidResponse(expectedCommand, &data[0], data.size());
}

/**
* @brief getNamedError - Get a named error from an error code
*
* @param uint8_t code - The error code
*
* @return const char* msg
*/
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

/**
* @brief getNamedOpenMode - Get a named mode from an open mode integer
*
* @param uint8_t mode - The mode
*
* @return const char* mode
*/
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

/**
* @brief getNamedMultiImage - Get a named image from an open multi image type
*
* @param uint8_t imageType - The image type id
*
* @return const char* name
*/
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