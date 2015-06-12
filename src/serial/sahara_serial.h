/**
* LICENSE PLACEHOLDER
*/

#ifndef _SERIAL_SAHARA_SERIAL_H_
#define _SERIAL_SAHARA_SERIAL_H_

#include "definitions.h"
#include "serial/serial.h"
#include "qc/sahara.h"
#include "qc/mbn.h"
#include "util/hexdump.h"

namespace openpst {
class SaharaSerial : public serial::Serial {

    public:
        SaharaSerial(std::string port, int baudrate = 115200);

        ~SaharaSerial();

        /**
         * @brief sendPacket
         * @param sahara_packet_t packet
         * @return size_t - The amount of bytes written
         */
        size_t sendPacket(sahara_packet_t* packet);

        /**
         * @brief receiveHello
         * @return
         */
        int receiveHello();

        /**
         * @brief sendHello
         * @param uint32_t mode - @see enum SAHARA_MODE - if null defaults to current mode
         * @param uint32_t status - our status, other than 0 results in error - defaults to 0
         * @return
         */
        int sendHello(uint32_t mode = NULL, uint32_t status = 0x00);

        /**
         * @brief sendHello
         * @param uint32_t mode - @see enum SAHARA_MODE
         * @return
         */
        int switchMode(uint32_t mode);

        /**
         * @brief sendClientCommand
         * @param uint32_t command - @see enum SAHARA_CLIENT_COMMAND
         * @param responseData - A pointer to the command response data, if available
         * @param responseDataSize - The size of the response data
         * @return
         */
        int sendClientCommand(uint32_t command, uint8_t* responseData, size_t &responseDataSize);
        /**
         * @brief sendImage
         * @param file
         * @return
         */
        int sendImage(std::string file);
        /**
         * @brief sendReset
         * @return
         */
        int sendReset();

        /**
         * @brief sendDone
         * @return
         */
        int sendDone();

        /**
         * @brief sendDone
         * @return
         */
        void close();

        /**
         * @brief getNamedMode
         * @param mode
         * @return
         */
        const char* getNamedMode(uint32_t mode);
        /**
         * @brief getNamedClientCommand
         * @param command
         * @return
         */
        const char* getNamedClientCommand(uint32_t command);
        /**
         * @brief getNamedErrorStatus
         * @param status
         * @return
         */
        const char* getNamedErrorStatus(uint32_t status);
        /**
         * @brief getNamedRequestedImage
         * @param imageId
         * @return
         */
        const char* getNamedRequestedImage(uint32_t imageId);
        /**
         * @brief getNamedRequestedImageAlt
         * @param imageId
         * @return
         */
        const char* getNamedRequestedImageAlt(uint32_t imageId);

        sahara_hello_rx_t deviceState;
        sahara_read_data_rx_t readState;
        sahara_transfer_response_rx_t lastError;

        uint8_t* buffer;
        size_t bufferSize;
        size_t lastRxSize,
               lastTxSize;
};
}

#endif /* _SERIAL_SAHARA_SERIAL_H_ */
