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

        enum QcdmOpertationResult {
            kQcdmIOError            = -2,
            kQcdmRequiresSecurity   = -1,
            kQcdmError              = 0,
            kQcdmSuccess            = 1
        };

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
            * @brief getVersion
            * @return QcdmVersionResponse
			*
			* @throws QcdmResponseError
			* @throws serial::PortNotOpenedException
			* @throws serial::IOException
			*/
			QcdmVersionResponse getVersion();

			/**
			* @brief getDiagVersion
			* @return uint16_t
			*
			* @throws QcdmResponseError
			* @throws serial::PortNotOpenedException
			* @throws serial::IOException
			*/
			uint16_t getDiagVersion();

			/**
			* @brief getStatus
			* @return QcdmStatusResponse
			*
			* @throws QcdmResponseError
			* @throws serial::PortNotOpenedException
			* @throws serial::IOException
			*/
			QcdmStatusResponse getStatus();

			/**
			* @brief getGuid
			* @return QcdmGuidResponse
			*
			* @throws QcdmResponseError
			* @throws serial::PortNotOpenedException
			* @throws serial::IOException
			*/
			QcdmGuidResponse getGuid();
			
            /**
             * @brief sendSpc
             * @param spc - a 6 digit SPC code to unlock service programming
             * @return bool
			 *
			 * @throws QcdmInvalidArgument
			 * @throws QcdmResponseError
			 * @throws serial::PortNotOpenedException
			 * @throws serial::IOException
             */
			bool sendSpc(std::string spc);

            /**
            * @brief sendPassword
            * @param password - a 16 digit password to unlock secure operations
			* @return bool
			*
			* @throws QcdmInvalidArgument
			* @throws QcdmResponseError
			* @throws serial::PortNotOpenedException
			* @throws serial::IOException
			*/
			bool sendPassword(std::string password);

            /**
            * @brief sendPhoneMode
            * @param QcdmPhoneMode mode
            * @return bool
			*
			* @throws QcdmResponseError
			* @throws serial::PortNotOpenedException
			* @throws serial::IOException
            */
			bool setPhoneMode(QcdmPhoneMode mode);

            /**
            * @brief readNV - Read an item from non volatile memory
            * @param uint16_t itemId - NV Item ID to read
            * @param response - Response structure to populate the read data with
            * @return QcdmNvResponse
			*
			* @throws QcdmResponseError
			* @throws serial::PortNotOpenedException
			* @throws serial::IOException
            */
			QcdmNvResponse readNV(uint16_t itemId);

            /**
            * @brief writeNV - Write an item to non volatile memory
            * @param itemId - NV Item ID
            * @param uint8_t* data - NV Item Data
			* @param size_t size - Cannot exceed DIAG_NV_ITEM_SIZE
            * @return bool
			*
			* @throws QcdmInvalidArgument
			* @throws QcdmResponseError
			* @throws serial::PortNotOpenedException
			* @throws serial::IOException
            */
			bool writeNV(uint16_t itemId, uint8_t* data, size_t size);

			QcdmNvPeekResponse peekNV(uint32_t address, uint8_t size);

			void switchToDload();

			bool sendHtcNvUnlock();
			   
            bool sendLgNvUnlock();  

			bool getLgSpc();

            

            int getErrorLog();
            int clearErrorLog();


			std::string lastError;
            uint8_t buffer[DIAG_MAX_PACKET_SIZE];
            size_t lastRxSize,
                   lastTxSize;

            
			void sendCommand(uint8_t command, bool validate = true);
			void sendCommand(uint8_t command, uint8_t* data, size_t size, bool validate = true);
			std::string getErrorString(uint8_t responseCommand);
		private:

    };
}

class QcdmResponseError : public std::exception
{
	const QcdmResponseError& operator=(QcdmResponseError);
	std::string _what;
public:
	QcdmResponseError(std::string message) : _what(message)  {}
	QcdmResponseError(const QcdmResponseError& second) : _what(second._what) {}
	virtual ~QcdmResponseError() throw() {}
	virtual const char* what() const throw () {
		return _what.c_str();
	}
};

class QcdmInvalidArgument : public std::invalid_argument
{
	const QcdmInvalidArgument& operator=(QcdmInvalidArgument);
	std::string _what;
public:
	QcdmInvalidArgument(std::string message) : invalid_argument(message), _what(message)  {}
	QcdmInvalidArgument(const QcdmInvalidArgument& second) : invalid_argument(second), _what(second._what) {}
	virtual ~QcdmInvalidArgument() throw() {}
	virtual const char* what() const throw () {
		return _what.c_str();
	}
};


#endif /* _SERIAL_QCDM_SERIAL_H */
