/**
* LICENSE PLACEHOLDER
*/

#ifndef _SERIAL_STREAMING_DLOAD_SERIAL_H
#define _SERIAL_STREAMING_DLOAD_SERIAL_H

#include "include/definitions.h"
#include "serial/serial.h"
#include "serial/hdlc_serial.h"
#include "qc/streaming_dload.h"
#include "qc/hdlc.h"
#include "util/hexdump.h"
#include "util/endian.h"

namespace openpst {
	class StreamingDloadSerial : public HdlcSerial {
		public:
			uint8_t* buffer;
			size_t bufferSize;
			size_t lastRxSize;
			size_t lastTxSize;

            StreamingDloadSerial(std::string port, int baudrate = 115200);
            ~StreamingDloadSerial();

			int sendHello(std::string magic, uint8_t version, uint8_t compatibleVersion, uint8_t featureBits);
			int sendUnlock(std::string code);
			int setSecurityMode(uint8_t mode);
			streaming_dload_hello_rx_t deviceState;
			streaming_dload_error_rx_t lastError;
			streaming_dload_log_rx_t lastLog;

			const char* getNamedError(uint8_t code);

	private:
		bool isValidResponse(uint8_t expectedCommand, uint8_t* response, size_t& responseSize);
    };
}

#endif // _SERIAL_STREAMING_DLOAD_SERIAL_H 
