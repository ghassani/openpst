/**
* LICENSE PLACEHOLDER
*
* @file hdlc_serial.h
* @class HdlcSerial
* @package OpenPST
* @brief HDLC serial port implementation
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#ifndef _SERIAL_HDLC_SERIAL_H
#define _SERIAL_HDLC_SERIAL_H

#include "include/definitions.h"
#include "serial/serial.h"
#include "qc/hdlc.h"
#include "util/hexdump.h"

namespace openpst {
    class HdlcSerial : public serial::Serial {
        public:
            HdlcSerial(std::string port, int baudrate = 115200);
            ~HdlcSerial();

            /**
             * Escapes the data and creates a CRC'ed HDLC packet
             * then writes the data
             *
             * @super Serial::write (uint8_t *data, size_t size);
             *
             * @brief write
             * @param data
             * @param size
             * @param bool encapsulate
             * @return
             */
            size_t write (uint8_t *data, size_t size, bool encapsulate = true);

            /**
             * Reads and unescpaes theCRC'ed HDLC packet
             * read from the device
             *
             * @super Serial::read (uint8_t *buffer, size_t size);
             *
             * @brief read
             * @param uint8_t* buf
             * @param size_t size
             * @param bool unescape
             * @return
             */
            size_t read (uint8_t *buf, size_t size, bool unescape = true);
			
			size_t write(std::vector<uint8_t> &data, bool encapsulate = true);
			size_t read(std::vector<uint8_t> &buffer, size_t size, bool unescape = true);

            uint8_t buffer[2048 * 2];
            size_t lastRxSize,
                   lastTxSize;
    };
}

#endif /* _SERIAL_HDLC_SERIAL_H */
