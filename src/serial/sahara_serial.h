/**
* LICENSE PLACEHOLDER
*
* @file sahara_serial.h
* @class SaharaSerial
* @package OpenPST
* @brief Sahara protocol serial port implementation
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#ifndef _SERIAL_SAHARA_SERIAL_H_
#define _SERIAL_SAHARA_SERIAL_H_

#include "include/definitions.h"
#include "serial/serial.h"
#include "qc/sahara.h"
#include "qc/mbn.h"
#include "util/hexdump.h"
#include "util/sleep.h"
#include <iostream>
#include <fstream>

namespace openpst {

	enum SAHARA_OPERATION_RESULT {
		SAHARA_OPERATION_IO_ERROR	= -1,
		SAHARA_OPERATION_ERROR		= 0,
		SAHARA_OPERATION_SUCCESS	= 1
	};

	class SaharaSerial : public serial::Serial {

		public:
			SaharaSerial(std::string port, int baudrate = 115200);

			~SaharaSerial();

			/**
			 * @brief sendPacket Sends a generic packet
			 * @param sahara_packet_t packet
			 * @return size_t - The amount of bytes written
			 */
			size_t sendPacket(sahara_packet_t* packet);

			/**
			 * @brief readHello
			 * @return int
			 */
			int readHello();

			/**
			 * @brief sendHello
			 * @param uint32_t mode - @see enum SAHARA_MODE
			 * @param uint32_t version -The version of sahara protocol to request, defaults to 2
			 * @param uint32_t minVersion - The minimum version we can support, defaults to 1
			 * @param uint32_t status - indicate to device our status, if set to anything other than 0x00, sahara protocol will abort
			 * @return int
			 */
			int sendHello(uint32_t mode, uint32_t version = 0x02, uint32_t minVersion = 0x01, uint32_t status = 0x00);

			/**
			 * @brief sendHello
			 * @param uint32_t mode - @see enum SAHARA_MODE
			 * @return int
			 */ 
			int switchMode(uint32_t mode);

			/**
			 * @brief sendClientCommand
			 * @param uint32_t command - @see enum SAHARA_CLIENT_COMMAND
			 * @param responseData - A pointer to the memory allocated block with the command
									 response data if the client command returns some sort of
									 data like debug data, sn, chip info.
									 You will need to free this.
			 * @param responseDataSize - The size of the response data
			 * @return int
			 */
			int sendClientCommand(uint32_t command, uint8_t** responseData, size_t &responseDataSize);

			/**
			 * @brief sendImage - Send a whole image file to the device. 
			 * @param std::string file
			 * @return int
			 */
			int sendImage(std::string filePath);

			/**
			* @brief readNextImageChunkSize
			* @param uint32_t offset - Will hold the next offset to send from
			* @param size_t size - Will hold the next size to read from the file being
			*					  requested
			* @return int - if success returned and offset and size are 0 then transfer is complete
			*/
			int readNextImageOffset(uint32_t& offset, size_t& size);

			/**
			* @brief readMemory - Read size starting from address and 
			*					  store it in a memory allocated buffer
			*
			* @param uint32_t address - The starting address to read from
			* @param size_t size - If the size is over SAHARA_MAX_MEMORY_REQUEST_SIZE, it will read in chunks of SAHARA_MAX_MEMORY_REQUEST_SIZE
			* @param uint8_t** out - Pointer to the memory allocated buffer with the read data
			* @param size_t outSize - Total size of the read data
			* @return int
			*/
			int readMemory(uint32_t address, size_t size, uint8_t** out, size_t& outSize);
			
			/**
			* @brief readMemory - Read size starting from address and
			*					  save the result into the specified outFilePath
			*
			* @param uint32_t address - The starting address to read from
			* @param size_t size - If the size is over SAHARA_MAX_MEMORY_REQUEST_SIZE, it will read in chunks of SAHARA_MAX_MEMORY_REQUEST_SIZE
			* @param const char* outFilePath - Path to the file to create and store the read data
			* @param size_t outSize - Total size of the read data
			* @return int
			*/
			int readMemory(uint32_t address, size_t size, const char* outFilePath, size_t& outSize);
			
			/**
			* @brief readMemory - Read size starting from address and
			*					  save the result into an existing file pointer.
			*				
			* @note - Will not close the file pointer handle
			*
			* @param uint32_t address
			* @param size_t size - If the size is over SAHARA_MAX_MEMORY_REQUEST_SIZE, it will read in chunks of SAHARA_MAX_MEMORY_REQUEST_SIZE
			* @param FILE* out - File pointer to the file to store the read data
			* @param size_t outSize
			* @return int
			*/
			int readMemory(uint32_t address, size_t size, FILE* out, size_t& outSize);

			/**
			 * @brief sendReset
			 * @return int
			 */
			int sendReset();

			/**
			 * @brief sendDone - Sends the done command. In emergency mode this will 
			 *					 transition to the uploaded programmer 
			 * @return int
			 */
			int sendDone();

			/**
			 * @brief sendDone
			 * @return void
			 */
			void close();

			/**
			 * @brief getNamedMode
			 * @param mode
			 * @return const char*
			 */
			const char* getNamedMode(uint32_t mode);

			/**
			 * @brief getNamedClientCommand
			 * @param command
			 * @return const char*
			 */
			const char* getNamedClientCommand(uint32_t command);

			/**
			 * @brief getNamedErrorStatus
			 * @param status
			 * @return const char*
			 */
			const char* getNamedErrorStatus(uint32_t status);

			/**
			 * @brief getNamedRequestedImage
			 * @param imageId
			 * @return const char*
			 */
			const char* getNamedRequestedImage(uint32_t imageId);

			sahara_hello_rx_t deviceState;
			sahara_read_data_rx_t readState;
			sahara_memory_debug_rx_t memoryState;
			sahara_transfer_response_rx_t lastError;

			uint8_t* buffer;
			size_t bufferSize;
			size_t lastRxSize,
				   lastTxSize;
		private:
			bool isValidResponse(uint32_t expectedResponseCommand, uint8_t* data, size_t dataSize);
	};
}

#endif /* _SERIAL_SAHARA_SERIAL_H_ */
