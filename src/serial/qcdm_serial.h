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
#include "qc/dm.h"
#include "qc/dm_nv.h"
#include "util/hexdump.h"

namespace OpenPST {
    class QcdmSerial : public HdlcSerial {
        public:
			/**
			* @brief QcdmSerial - Constructor
			*
			* @param std::string port
			* @param int baudrate
			* @param serial::Timeout - Timeout, defaults to 1000ms
			*/
			QcdmSerial(std::string port, int baudrate, serial::Timeout timeout = serial::Timeout::simpleTimeout(1000));

			/**
			* @brief ~QcdmSerial - Destructor
			*/
            ~QcdmSerial();

            /**
             * @brief sendSpc
             * @param spc - a 6 digit SPC code to unlock service programming
             * @return
             */
            int sendSpc(const char* spc);

            /**
            * @brief sendPassword
            * @param password - a 16 digit password to unlock secure operations
            * @return
            */
            int sendPassword(const char* password);

            /**
            * @brief sendPhoneMode
            * @param mode - DIAG_PHONE_MODE
            * @return
            */
            int sendPhoneMode(uint8_t mode);

            /**
            * @brief getNvItem
            * @param itemId - NV Item ID
            * @param response -
            * @return
            */
            int getNvItem(int itemId, uint8_t** response);

            /**
            * @brief setNvItem
            * @param itemId - NV Item ID
            * @param data - NV Item Data
            * @return
            */
            int setNvItem(int itemId, const char* data, int length);

            /**
            * @brief setNvItem
            * @param itemId - NV Item ID
            * @param data - NV Item Data
            * @param response -
            * @return
            */
            int setNvItem(int itemId, const char* data, int length, uint8_t** response);

            /**
            * @brief sendHtcNvUnlock
            * @param response -
            * @return
            */
            int sendHtcNvUnlock(uint8_t** response);

            /**
            * @brief sendLgNvUnlock
            * @param response -
            * @return
            */
            int sendLgNvUnlock(uint8_t** response);  //FIXME

            /**
            * @brief getLgSpc
            * @param response -
            * @return
            */
            int getLgSpc(uint8_t** response);

            uint8_t buffer[DIAG_MAX_PACKET_SIZE];
            size_t lastRxSize,
                   lastTxSize;
    };
}

#endif /* _SERIAL_QCDM_SERIAL_H */
