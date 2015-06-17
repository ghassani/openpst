/**
* LICENSE PLACEHOLDER
*/

#ifndef _SERIAL_STREAMING_DLOAD_SERIAL_H
#define _SERIAL_STREAMING_DLOAD_SERIAL_H

#include "include/definitions.h"
#include "serial/serial.h"
#include "serial/hdlc_serial.h"
#include "qc/hdlc.h"
#include "util/hexdump.h"

namespace openpst {
	class StreamingDloadSerial : public HdlcSerial {
        public:
            StreamingDloadSerial(std::string port, int baudrate = 115200);
            ~StreamingDloadSerial();
    };
}

#endif /* _SERIAL_STREAMING_DLOAD_SERIAL_H */
