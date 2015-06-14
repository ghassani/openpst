/**
* LICENSE PLACEHOLDER
*/

#ifndef _SERIAL_QCDM_SERIAL_H
#define _SERIAL_QCDM_SERIAL_H

#include "include/definitions.h"
#include "serial/serial.h"
#include "qc/diag.h"
#include "qc/hdlc.h"
#include "util/hexdump.h"

namespace openpst {
    class QcdmSerial : public serial::Serial {
        public:
            QcdmSerial(std::string port, int baudrate = 115200);
            ~QcdmSerial();

            /**
             * @brief sendSpc
             * @param spc - a 6 digit SPC code to unlock
             * @return
             */
            int sendSpc(const char* spc);

            /**
             * Escapes the data and creates a CRC'ed HDLC packet
             * then writes the data
             *
             * @super Serial::write (uint8_t *data, size_t size);
             *
             * @brief write
             * @param data
             * @param size
             * @return
             */
            size_t write (uint8_t *data, size_t size);

            /**
             * Reads and unescpaes theCRC'ed HDLC packet
             * read from the device
             *
             * @super Serial::read (uint8_t *buffer, size_t size);
             *
             * @brief write
             * @param data
             * @param size
             * @return
             */
            size_t read (uint8_t *buf, size_t size);

            uint8_t buffer[DIAG_MAX_RX_PKT_SIZ];
            size_t lastRxSize,
                   lastTxSize;


    };
}

#endif /* _SERIAL_QCDM_SERIAL_H */
