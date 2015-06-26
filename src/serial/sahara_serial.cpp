/**
* LICENSE PLACEHOLDER
*
* @file sahara_serial.cpp
* @class SaharaSerial
* @package OpenPST
* @brief Sahara protocol serial port implementation
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "sahara_serial.h"

using namespace openpst;

SaharaSerial::SaharaSerial(std::string port, int baudrate ) :
    serial::Serial (port, baudrate, serial::Timeout::simpleTimeout(1000))
{
	bufferSize = SAHARA_MAX_PACKET_SIZE;
	buffer = new uint8_t[SAHARA_MAX_PACKET_SIZE];
    memset(&deviceState, 0x00, sizeof(deviceState));
	memset(&readState, 0x00, sizeof(readState));
	memset(&memoryState, 0x00, sizeof(memoryState));
	
}

SaharaSerial::~SaharaSerial()
{
    delete buffer;
}


/**
* @brief SaharaSerial::sendPacket Sends a generic packet
* @param sahara_packet_t packet
* @return size_t - The amount of bytes written
*/
size_t SaharaSerial::sendPacket(sahara_packet_t* packet)
{
    if (!isOpen()) {
        return 0;
    }

    lastTxSize = write((uint8_t*)packet, packet->header.size);

    if (!lastTxSize) {
        printf("Attempted to write to port but 0 bytes were written\n");
        return 0;
    }

    hexdump_tx((uint8_t*)packet, lastTxSize);

    return lastTxSize;
}

/**
 * @brief SaharaSerial::readHello
 * @return
 */
int SaharaSerial::readHello()
{
    if (!isOpen()) {
        return 0;
    }

    lastRxSize = read(buffer, bufferSize);

	if (!lastRxSize) {
        printf("Did not receive hello. Not in sahara mode or requires restart\n");
        return 0;
    }

	hexdump_rx(buffer, lastRxSize);

	if (!isValidResponse(SAHARA_HELLO, buffer, lastRxSize)) {
		printf("Did Not Receive Hello Request From Device\n");
		return 0;
	}

    memcpy(&deviceState, buffer, sizeof(deviceState));

    // resize the read buffer if it is not already the same size
    // the device provides us with the maximum sized packet that it
    // will return to us
    if (deviceState.maxCommandPacketSize != bufferSize) {
        uint8_t* resizedBuffer = (uint8_t*) realloc(buffer, deviceState.maxCommandPacketSize);
        if (resizedBuffer != NULL) {
            buffer = resizedBuffer;
            bufferSize = deviceState.maxCommandPacketSize;
        }
    }

    return 1;
}

/**
* @brief sendHello
* @param uint32_t mode - @see enum SAHARA_MODE - if null defaults to current mode
* @param uint32_t version -The version of sahara protocol to request, defaults to 2
* @param uint32_t minVersion - The minimum version we can support, defaults to 1
* @param uint32_t status - indicate to device our status, if set to anything other than 0x00, sahara protocol will abort
* @return
*/
int SaharaSerial::sendHello(uint32_t mode, uint32_t version, uint32_t minVersion, uint32_t status)
{
    if (!isOpen()) {
		return SAHARA_OPERATION_ERROR;
    }

    sahara_hello_tx_t packet;
    memcpy(&packet, (uint8_t*)&deviceState, sizeof(packet));
    packet.header.command = SAHARA_HELLO_RESPONSE;
    packet.status = status;
    packet.mode = mode == NULL ? deviceState.mode : mode;
	packet.version = version;
	packet.minVersion = minVersion;

    lastTxSize = write((uint8_t*)&packet, sizeof(packet));

    if (!lastTxSize) {
        printf("Attempted to write to port but 0 bytes were written\n");
		return SAHARA_OPERATION_ERROR;
    }
	
	hexdump_tx((uint8_t*)&packet, lastTxSize);
	
	try {
		lastRxSize = read(buffer, bufferSize);
	} catch (serial::IOException e) {
		// sometimes (at least in memory debug mode) the device
		// will reset itself and the hello needs to be re-read and
		// sent
		close();
		
		sleep(5000); // give it some time to come back
		
		open();
		
		if (!readHello()) {
			printf("Error on re-handshake\n");
			return SAHARA_OPERATION_ERROR;
		}

		lastTxSize = write((uint8_t*)&packet, sizeof(packet)); // resend the hello response

		if (!lastTxSize) {
			printf("Attempted to write to port but 0 bytes were written\n");
			return SAHARA_OPERATION_ERROR;
		}

		hexdump_tx((uint8_t*)&packet, lastTxSize);

		lastRxSize = read(buffer, bufferSize);
	}
	
    if (!lastRxSize) {
        printf("Expected response but 0 bytes received from device\n");
		return SAHARA_OPERATION_ERROR;
    }

	hexdump_rx(buffer, lastRxSize);

	if (packet.mode == SAHARA_MODE_COMMAND && isValidResponse(SAHARA_COMMAND_READY, buffer, lastRxSize)) {        
        deviceState.mode = packet.mode;
	} else if (packet.mode == SAHARA_MODE_IMAGE_TX_PENDING && isValidResponse(SAHARA_READ_DATA, buffer, lastRxSize)) {        
        memcpy(&readState, buffer, sizeof(readState));
        deviceState.mode = packet.mode;
	} else if (packet.mode == SAHARA_MODE_MEMORY_DEBUG && isValidResponse(SAHARA_MEMORY_DEBUG, buffer, lastRxSize)) {    
		memcpy(&memoryState, buffer, sizeof(memoryState));
		deviceState.mode = packet.mode;
	} else {
		return SAHARA_OPERATION_ERROR;
    }

	return SAHARA_OPERATION_SUCCESS;
}

/**
 * @brief SaharaSerial::switchMode
 * @param mode
 * @return
 */
int SaharaSerial::switchMode(uint32_t mode)
{
    if (!isOpen()) {
        return 0;
    }

	if (deviceState.mode == mode) {
		printf("Device already in mode 0x%02X - %s\n",
			mode,
			getNamedMode(mode)
		);
		return 1;
	}

    printf("Requesting Mode Switch From 0x%02x - %s  to 0x%02x - %s\n",
        deviceState.mode,
        getNamedMode(deviceState.mode),
        mode,
        getNamedMode(mode)
    );

    sahara_command_switch_mode_tx_t packet;
    packet.header.command = SAHARA_COMMAND_SWITCH_MODE;
    packet.header.size = sizeof(packet);
    packet.mode = mode;

    lastTxSize = write((uint8_t*)&packet, packet.header.size);

	if (!lastTxSize) {
        printf("Attempted to write to port but 0 bytes were written\n");
    }

	hexdump_tx((uint8_t*)&packet, lastTxSize);

    lastRxSize = read(buffer, bufferSize);

    if (!lastRxSize) {
        printf("Device Did Not Respond\n");
        return 0;
    }

	hexdump_rx(buffer, lastRxSize);

	if (packet.mode == SAHARA_MODE_COMMAND && isValidResponse(SAHARA_COMMAND_READY, buffer, lastRxSize)) {

	} else if (packet.mode == SAHARA_MODE_MEMORY_DEBUG && isValidResponse(SAHARA_MEMORY_DEBUG, buffer, lastRxSize)) {

	} else if (packet.mode == SAHARA_MODE_IMAGE_TX_PENDING && isValidResponse(SAHARA_READ_DATA, buffer, lastRxSize)) {
		if (!readHello() || !sendHello()) {
			return 0;
		}
	} else {
		return 0;
	}
	
    deviceState.mode = packet.mode;

    return 1;
}
/**
* @brief SaharaSerial::sendClientCommand
* @param uint32_t command - @see enum SAHARA_CLIENT_COMMAND
* @param responseData - A pointer to the memory allocated block with the command 
						response data if the client command returns some sort of 
						data like debug data, sn, chip info. 
						You will need to free this.
* @param responseDataSize - The size of the response data
* @return int
*/
int SaharaSerial::sendClientCommand(uint32_t command, uint8_t** responseData, size_t &responseDataSize)
{
    if (!isOpen()) {
        return 0;
    }

    if (deviceState.mode != SAHARA_MODE_COMMAND) {
        // try to switch to client command mode
        printf("Not In Client Command Mode. Attempting To Switch.\n");
        if (!switchMode(SAHARA_MODE_COMMAND)) {
            return 0;
        }
    }

    printf("Sending Client Command: 0x%02x - %s\n",
        command, getNamedClientCommand(command)
    );

    sahara_command_execute_tx_t packet;
    packet.header.command = SAHARA_COMMAND_EXECUTE;
    packet.header.size    = sizeof(packet);
    packet.command        = command;

    lastTxSize = write((uint8_t*)&packet, sizeof(packet));

    if (!lastTxSize) {
        printf("Attempted to write to port but 0 bytes were written\n");
        return 0;
    }

    hexdump_tx((uint8_t*)&packet, lastTxSize);

    lastRxSize = read(buffer, bufferSize);

	if (!lastRxSize) {
        printf("Expected response but 0 bytes received from device\n");
        return 0;
    }

	hexdump_rx(buffer, lastRxSize);

	if (!isValidResponse(SAHARA_COMMAND_EXECUTE_RESPONSE, buffer, lastRxSize)) {
		return 0;
	}
	
    sahara_command_execute_response_rx_t* execResponse = (sahara_command_execute_response_rx_t*) buffer;

    sahara_command_execute_data_tx_t execData;
    execData.header.command = SAHARA_COMMAND_EXECUTE_DATA;
    execData.header.size = sizeof(sahara_command_execute_data_tx_t);
    execData.command = command;

	uint8_t* cmdResponseData = new uint8_t[execResponse->size];

	size_t totalReadSize = 0,
		cmdResponseDataLength = execResponse->size;

	do {
		if (totalReadSize + execResponse->size > cmdResponseDataLength) {
			size_t newSize = totalReadSize + execResponse->size;
			cmdResponseData = (uint8_t*)realloc(cmdResponseData, newSize);

			if (cmdResponseData == NULL) {
				printf("Could Not Allocate %u More Bytes For Data\n", execResponse->size);
				free(cmdResponseData);
				return 0;
			}

			cmdResponseDataLength = newSize;
		}


		lastTxSize = write((uint8_t*)&execData, sizeof(execData));

		if (!lastTxSize) {
			printf("Attempted to write to port but 0 bytes were written\n");
			free(cmdResponseData);
			return 0;
		}

		hexdump_tx((uint8_t*)&execData, lastTxSize);

		lastRxSize = read(buffer, bufferSize);

		if (!lastRxSize) {
			printf("Expected response but 0 bytes received from device\n");
			free(cmdResponseData);
			return 0;
		}

		hexdump_rx(buffer, lastRxSize);
		
		if (!isValidResponse(NULL, buffer, lastRxSize)) {
			printf("Error Encountered\n");
			free(cmdResponseData);
			return 0;
		}

		memmove(&cmdResponseData[totalReadSize], buffer, lastRxSize);

		//TODO check for error
		totalReadSize += lastRxSize;

		if (lastRxSize < bufferSize) {
			// read the end of it
			break;
		}

		lastTxSize = write((uint8_t*)&execData, sizeof(execData));

		if (!lastTxSize) {
			printf("Attempted to write to port but 0 bytes were written\n");
			free(cmdResponseData);
			return 0;
		}

	} while (1);

	printf("========\nDumping Data For Command: 0x%02x - %s - %lu Bytes\n========\n\n",
		command, getNamedClientCommand(command), totalReadSize
	);

	hexdump(cmdResponseData, totalReadSize);

	*responseData = cmdResponseData;
	responseDataSize = totalReadSize;

    return 1;
}

/**
 * @brief SaharaSerial::sendImage
 * @param file
 * @return
 */
int SaharaSerial::sendImage(std::string file)
{
    if (!isOpen()) {
        return 0;
    }

    if (readState.imageId == MBN_IMAGE_NONE) {
        printf("Device has not requested an image\n");
        return 0;
    }

    FILE* fp = fopen(file.c_str(), "rb");

    if (!fp) {
        printf("Could Not Open File %s\n", file.c_str());
        return 0;
    }


    size_t fileSize,
           fileReadResult,
           totalSent = 0,
           totalRequestLength = 0;

    unsigned int chunkSize;

    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    rewind(fp);

    printf("Loaded File %s With Size %lu\n", file.c_str(), fileSize);

    uint8_t* fileBuffer = new uint8_t[readState.length];
    size_t fileBufferSize = readState.length;

    while (totalSent != fileSize) {
        if (readState.length > fileBufferSize) {
            fileBuffer = (uint8_t*) realloc(fileBuffer, readState.length);
            if (fileBuffer == NULL) {
                printf("Could Not Allocate %lu Bytes For File Buffer\n", readState.length);
                free(fileBuffer);
                return 0;
            }
        }

        chunkSize = readState.length;

        totalRequestLength += readState.length;

        if (fileSize < (totalSent + chunkSize)) {
            chunkSize = fileSize - totalSent;
        }

        fileReadResult = fread(fileBuffer, 1, chunkSize, fp);

        lastTxSize = write(fileBuffer, chunkSize);

        hexdump_tx(fileBuffer, lastTxSize);

        totalSent += lastTxSize;

        // get next segment if any
        lastRxSize = read(buffer, bufferSize);

        if (!lastRxSize) {
            printf("Expected response but 0 bytes received from device\n");
            free(fileBuffer);
            fclose(fp);
            return 0;
        }

        hexdump_rx(buffer, lastRxSize);

        if (buffer[0] != SAHARA_READ_DATA) {
            printf("Unexpected Response. Expected 0x%02x But Received 0x%02x\n", SAHARA_READ_DATA, buffer[0]);
            if (buffer[0] == SAHARA_END_OF_IMAGE_TRANSFER) {
                sahara_transfer_response_rx_t* endTransfer = (sahara_transfer_response_rx_t*) buffer;
                if (endTransfer->status != SAHARA_STATUS_SUCCESS) {
                    printf("Device Responded With Error: %s\n", getNamedErrorStatus(endTransfer->status));
                    free(fileBuffer);
                    fclose(fp);
                    return 0;
                }
            }
            break;
        }

        memcpy(&readState, buffer, sizeof(readState));
    }

    free(fileBuffer);
    fclose(fp);

    return 1;
}


int SaharaSerial::readMemory(uint32_t address, uint32_t size, uint8_t** responseData, size_t& responseSize)
{
	if (!isOpen()) {
		return 0;
	}

	if (deviceState.mode != SAHARA_MODE_MEMORY_DEBUG) {		
		printf("Not In Memory Debug Mode. Attempting To Switch.\n");
		if (!switchMode(SAHARA_MODE_MEMORY_DEBUG)) {
			return 0;
		}
	}

	sahara_memory_read_tx_t packet;
	packet.header.command = SAHARA_MEMORY_READ;
	packet.header.size = sizeof(packet);

	uint32_t lastAddress = NULL;
	uint8_t* out = new uint8_t[size];
	size_t outSize = size;
	responseSize = 0;

	do {
		packet.address = responseSize > 0 ? lastAddress + bufferSize : address;
		packet.length = size > bufferSize ? bufferSize : size;


		lastTxSize = write((uint8_t*)&packet, sizeof(packet));

		if (!lastTxSize) {
			printf("Attempted to write to port but 0 bytes were written\n");
			return 0;
		}

		hexdump_tx((uint8_t*)&packet, lastTxSize);

		lastRxSize = read(buffer, bufferSize);

		if (!lastRxSize) {
			printf("Expected response but 0 bytes received from device\n");
			return 0;
		}

		hexdump_rx(buffer, lastRxSize);

		lastAddress = packet.address;

		size_t newSize = responseSize + lastRxSize;
		if (newSize > outSize) {
			out = (uint8_t*)realloc(out, newSize);
			outSize = newSize;
		}

		memcpy(&out[responseSize], buffer, lastRxSize);

		responseSize += lastRxSize;

		if (size <= responseSize) {
			break; //done
		}

	} while (true);

	*responseData = out;

	//printf("\n\n\nFinal Data Size: %lu bytes\n", responseSize);
	//hexdump(out, responseSize);

	return 1;
}

int SaharaSerial::readMemory(uint32_t address, uint32_t size, const char* outFile, size_t& outFileSize)
{
	if (!isOpen()) {
		return 0;
	}

	if (deviceState.mode != SAHARA_MODE_MEMORY_DEBUG) {
		printf("Not In Memory Debug Mode. Attempting To Switch.\n");
		if (!switchMode(SAHARA_MODE_MEMORY_DEBUG)) {
			return 0;
		}
	}

	FILE* fp = fopen(outFile, "a+b");

	if (!fp) {
		printf("Error opening file %s for writing\n", outFile);
		return 0;
	}
	
	sahara_memory_read_tx_t packet;
	packet.header.command = SAHARA_MEMORY_READ;
	packet.header.size = sizeof(packet);
	packet.address = address;
	packet.length = size;

	lastTxSize = write((uint8_t*)&packet, sizeof(packet));

	if (!lastTxSize) {
		printf("Attempted to write to port but 0 bytes were written\n");
		return 0;
	}

	hexdump_tx((uint8_t*)&packet, lastTxSize);

	uint8_t memBuffer[SAHARA_MAX_MEMORY_DATA_SIZE];

	outFileSize = 0;

	do {
		lastRxSize = read(memBuffer, SAHARA_MAX_MEMORY_DATA_SIZE);

		if (lastRxSize == 0) {
			break;
		}

		hexdump_rx(memBuffer, lastRxSize);

		fwrite(memBuffer, sizeof(uint8_t), lastRxSize, fp);

		outFileSize += lastRxSize;
	} while (true);

	fclose(fp);

	return 1;
}


/**
 * @brief SaharaSerial::sendDone
 * @return
 */
int SaharaSerial::sendDone()
{
    if (!isOpen()) {
        return 0;
    }

    sahara_done_tx_t packet;
    packet.header.command = SAHARA_DONE;
    packet.header.size = sizeof(packet);

    lastTxSize = write((uint8_t*)&packet, sizeof(packet));

    if (!lastTxSize) {
        printf("Attempted to write to port but 0 bytes were written\n");
        return 0;
    }

    hexdump_tx((uint8_t*)&packet, lastTxSize);

    lastRxSize = read(buffer, bufferSize);

    if (!lastRxSize) {
        printf("Expected response but 0 bytes received from device\n");
        return 0;
    }

    hexdump_rx(buffer, lastRxSize);

	if (!isValidResponse(SAHARA_DONE_RESPONSE, buffer, lastRxSize)) {
		return 0;
	}

    //sahara_done_rx_t* doneResponse = (sahara_done_rx_t*) buffer;

    return 1;
}

/**
 * @brief SaharaSerial::sendReset
 * @return
 */
int SaharaSerial::sendReset()
{
    if (!isOpen()) {
        return 0;
    }

    sahara_reset_tx_t packet;
    packet.header.command = SAHARA_RESET;
    packet.header.size = sizeof(packet);

	try {
		lastTxSize = write((uint8_t*)&packet, sizeof(packet)); 
		hexdump_tx((uint8_t*)&packet, lastTxSize);

		lastRxSize = read(buffer, bufferSize);
		hexdump_rx(buffer, lastRxSize);

	} catch (std::exception e) {
		printf(e.what());
		try {
			close();
		} catch (std::exception e) {
			printf(e.what());
		}
		return 1;
	}

    if (!lastTxSize) {
        printf("Attempted to write to port but 0 bytes were written\n");
        return 0;
    }

    //hexdump_tx((uint8_t*)&packet, lastTxSize);

    if (!lastRxSize) {
        printf("Expected response but 0 bytes received from device\n");
        return 0;
    }

    //hexdump_rx(buffer, lastRxSize);

	if (!isValidResponse(SAHARA_RESET_RESPONSE, buffer, lastRxSize)) {
		return 0;
	}

	// close after reset
	close();

    return 1;
}

/**
 * @brief SaharaSerial::getNamedMode
 * @param mode
 * @return
 */
const char* SaharaSerial::getNamedMode(uint32_t mode)
{
    switch(mode) {
        case SAHARA_MODE_IMAGE_TX_PENDING:  return "Image Transfer Pending";
        case SAHARA_MODE_IMAGE_TX_COMPLETE: return "Image Transfer Complete";
        case SAHARA_MODE_MEMORY_DEBUG:      return "Memory Debug";
        case SAHARA_MODE_COMMAND:           return "Command Mode";
        default:
            return "Unknown";
    }
}

/**
 * @brief SaharaSerial::close
 */
void SaharaSerial::close()
{
   serial::Serial::close();
   memset(&deviceState, 0x00, sizeof(deviceState));
   memset(&readState,   0x00, sizeof(readState));
}


bool SaharaSerial::isValidResponse(uint32_t expectedResponseCommand, uint8_t* data, size_t dataSize)
{
	if (expectedResponseCommand == data[0]) {
		return true;
	}

	if (data[0] == SAHARA_END_OF_IMAGE_TRANSFER) {
		sahara_transfer_response_rx_t* error = (sahara_transfer_response_rx_t*)data;
		if (error->status != 0x00 && expectedResponseCommand != SAHARA_READ_DATA) {
			memcpy(&lastError, error, sizeof(sahara_transfer_response_rx_t));
			printf("Device Responded With Error: %s\n", getNamedErrorStatus(error->status));
			// we need to reset after an error and disconnect to prepare for another transfer if needed
			sendReset();
			return false;
		}
	} else if (NULL != expectedResponseCommand) {
		printf("Unexpected Response. Expected 0x%02x But Received 0x%02x", expectedResponseCommand, data[0]);
		return false;
	}

	return true;
}

/**
 * @brief SaharaSerial::getNamedClientCommand
 * @param command
 * @return
 */
const char* SaharaSerial::getNamedClientCommand(uint32_t command)
{
    switch(command) {
        case SAHARA_EXEC_CMD_NOP:                      return "NOP";
        case SAHARA_EXEC_CMD_SERIAL_NUM_READ:          return "Read Serial Number";
        case SAHARA_EXEC_CMD_MSM_HW_ID_READ:           return "Read MSM HW ID";
        case SAHARA_EXEC_CMD_OEM_PK_HASH_READ:         return "Read OEM PK Hash";
        case SAHARA_EXEC_CMD_SWITCH_TO_DMSS_DLOAD:     return "Switch To DMSS DLOAD";
        case SAHARA_EXEC_CMD_SWITCH_TO_STREAM_DLOAD:   return "Switch To Streaming DLOAD";
        case SAHARA_EXEC_CMD_READ_DEBUG_DATA:          return "Read Debug Data";
        case SAHARA_EXEC_CMD_GET_SOFTWARE_VERSION_SBL: return "Read SBL SW Version";
        default:
            return "Unknown";
    }
}

/**
 * @brief SaharaSerial::getNamedErrorStatus
 * @param status
 * @return
 */
const char* SaharaSerial::getNamedErrorStatus(uint32_t status)
{
    switch(status) {
        case SAHARA_STATUS_SUCCESS:                    return "Success";
        case SAHARA_NAK_INVALID_CMD:                   return "Invalid Command";
        case SAHARA_NAK_PROTOCOL_MISMATCH:             return "Protocol Mismatch";
        case SAHARA_NAK_INVALID_TARGET_PROTOCOL:       return "Invalid Target Protocol";
        case SAHARA_NAK_INVALID_HOST_PROTOCOL:         return "Invalid Host Protocol";
        case SAHARA_NAK_INVALID_PACKET_SIZE:           return "Invalid Packet Size";
        case SAHARA_NAK_UNEXPECTED_IMAGE_ID:           return "Unexpected Image ID";
        case SAHARA_NAK_INVALID_HEADER_SIZE:           return "Invalid Header Size";
        case SAHARA_NAK_INVALID_DATA_SIZE:             return "Invalid Data Size";
        case SAHARA_NAK_INVALID_IMAGE_TYPE:            return "Invalid Image Type";
        case SAHARA_NAK_INVALID_TX_LENGTH:             return "Invalid TX Length";
        case SAHARA_NAK_INVALID_RX_LENGTH:             return "Invalid RX Length";
        case SAHARA_NAK_GENERAL_TX_RX_ERROR:           return "General TX RX Error";
        case SAHARA_NAK_UNSUPPORTED_NUM_PHDRS:         return "Cannot receive specified number of program headers";
        case SAHARA_NAK_INVALID_PDHR_SIZE:             return "Invalid data length received for program headers";
        case SAHARA_NAK_MULTIPLE_SHARED_SEG:           return "Multiple shared segments found in ELF image";
        case SAHARA_NAK_UNINIT_PHDR_LOC:               return "Uninitialized program header location";
        case SAHARA_NAK_INVALID_DEST_ADDR:             return "Invalid destination address";
        case SAHARA_NAK_INVALID_IMG_HDR_DATA_SIZE:     return "Invalid data size receieved in image header";
        case SAHARA_NAK_INVALID_ELF_HDR:               return "Invalid ELF header received";
        case SAHARA_NAK_UNKNOWN_HOST_ERROR:            return "Unknown Host Error";
        case SAHARA_NAK_TIMEOUT_RX:                    return "RX Timeout";
        case SAHARA_NAK_TIMEOUT_TX:                    return "TX Timeout";
        case SAHARA_NAK_INVALID_HOST_MODE:             return "Invalid Host Mode";
        case SAHARA_NAK_INVALID_MEMORY_READ:           return "Invalid Memory Read";
        case SAHARA_NAK_INVALID_DATA_SIZE_REQUEST:     return "Invalid Data Size Request";
        case SAHARA_NAK_MEMORY_DEBUG_NOT_SUPPORTED:    return "Memory Debug Not Supported";
        case SAHARA_NAK_INVALID_MODE_SWITCH:           return "Invalid Mode Switch";
        case SAHARA_NAK_CMD_EXEC_FAILURE:              return "Command Execute Failure";
        case SAHARA_NAK_EXEC_CMD_INVALID_PARAM:        return "Invalid Command Parameter";
        case SAHARA_NAK_EXEC_CMD_UNSUPPORTED:          return "Command Unsupported";
        case SAHARA_NAK_EXEC_DATA_INVALID_CLIENT_CMD:  return "Invalid Client Command";
        case SAHARA_NAK_HASH_TABLE_AUTH_FAILURE:       return "Hash Table Auth Failure";
        case SAHARA_NAK_HASH_VERIFICATION_FAILURE:     return "Hash Table Verification Failure";
        case SAHARA_NAK_HASH_TABLE_NOT_FOUND:          return "Hash Table Not Found";
        case SAHARA_NAK_TARGET_INIT_FAILURE:           return "Target Init Failure";
        case SAHARA_NAK_IMAGE_AUTH_FAILURE:            return "Image Auth Failure";
        case SAHARA_NAK_INVALID_IMG_HASH_TABLE_SIZE:   return "Invalid ELF Hash Table Size";
        default:
            return "Unknown";
        }
}

/**
 * @brief SaharaSerial::getNamedRequestedImage
 * @param imageId
 * @return
 */
const char* SaharaSerial::getNamedRequestedImage(uint32_t imageId)
{
    switch(imageId) {
        case MBN_IMAGE_NONE:          return "None";
        case MBN_IMAGE_OEM_SBL_IMG:   return "OEM SBL";
        case MBN_IMAGE_AMSS_IMG:      return "AMSS";
        case MBN_IMAGE_QCSBL_IMG:     return "QCSBL";
        case MBN_IMAGE_HASH_IMG:      return "Hash";
        case MBN_IMAGE_APPSBL_IMG:    return "APPSBL";
        case MBN_IMAGE_HOSTDL:        return "HOSTDL";
        case MBN_IMAGE_DSP1:          return "DSP1";
        case MBN_IMAGE_FSBL:          return "FSBL";
        case MBN_IMAGE_DBL:           return "DBL";
        case MBN_IMAGE_OSBL:          return "OSBL";
        case MBN_IMAGE_DSP2:          return "DSP2";
        case MBN_IMAGE_EHOSTDL:       return "EHOSTDL";
        case MBN_IMAGE_NANDPRG:       return "NANDPRG";
        case MBN_IMAGE_NORPRG:        return "NORPRG";
        case MBN_IMAGE_RAMFS1:        return "RAMFS1";
        case MBN_IMAGE_RAMFS2:        return "RAMFS2";
        case MBN_IMAGE_ADSP_Q5:       return "ADSP Q5";
        case MBN_IMAGE_APPS_KERNEL:   return "APPS Kernel";
        case MBN_IMAGE_BACKUP_RAMFS:  return "Backup RAMFS";
        case MBN_IMAGE_SBL1:          return "SBL1";
        case MBN_IMAGE_SBL2:          return "SBL2";
        case MBN_IMAGE_RPM:           return "RPM";
        case MBN_IMAGE_SBL3:          return "SBL3";
        case MBN_IMAGE_TZ:            return "TZ";
        case MBN_IMAGE_SSD_KEYS:      return "SSD Keys";
        case MBN_IMAGE_GEN:           return "GEN";
        case MBN_IMAGE_DSP3:          return "DSP3";
        case MBN_IMAGE_ACDB:          return "ACDB";
        case MBN_IMAGE_WDT:           return "WDT";
        case MBN_IMAGE_MBA:           return "MBA";
        default:
            return "Unknown";
    }
}
