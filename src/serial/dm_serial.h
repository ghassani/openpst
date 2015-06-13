/**
* LICENSE PLACEHOLDER
*/

#ifndef _SERIAL_DM_SERIAL_H
#define _SERIAL_DM_SERIAL_H

#include "include/definitions.h"
#include "serial/serial.h"
#include "qc/diag.h"
#include "qc/hdlc.h"
#include "util/hexdump.h"

namespace openpst {
    class DmSerial : public serial::Serial {
        public:
            DmSerial(std::string port, int baudrate = 115200);
            ~DmSerial();

            uint8_t buffer[1024];
            size_t lastRxSize,
                   lastTxSize;
    };
}

#endif /* _SERIAL_DM_SERIAL_H */
