/**
* LICENSE PLACEHOLDER
*
* @file qcdm_serial.cpp
* @class QcdmSerial
* @package OpenPST
* @brief QCDM serial port implementation
*
* @author Gassan Idriss <ghassani@gmail.com>
* @author Matteson Raab <mraabhimself@gmail.com>
*/


#ifndef _SERIAL_QCDM_SERIAL_H
#define _SERIAL_QCDM_SERIAL_H

#include "include/definitions.h"
#include "serial/serial.h"
#include "serial/hdlc_serial.h"
#include "qc/diag.h"
#include "qc/hdlc.h"
#include "util/hexdump.h"

namespace openpst {
    class QcdmSerial : public HdlcSerial {
        public:
            QcdmSerial(std::string port, int baudrate = 115200);
            ~QcdmSerial();

            /**
             * @brief sendSpc
             * @param spc - a 6 digit SPC code to unlock service programming
             * @return
             */
            int sendSpc(const char* spc);

			/**
			* @brief send16Password
			* @param password - a 16 digit password to unlock secure operations
			* @return
			*/
			int send16Password(const char* password);

			QString getNvItemString(int itemId);

            uint8_t buffer[DIAG_MAX_RX_PKT_SIZ];
            size_t lastRxSize,
                   lastTxSize;
    };
}

#endif /* _SERIAL_QCDM_SERIAL_H */
