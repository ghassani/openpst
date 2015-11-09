/**
* LICENSE PLACEHOLDER
*
* @file meid.h
* @package OpenPST
* @brief meid helper
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#ifndef _UTIL_MEID_H
#define _UTIL_MEID_H

#include "include/definitions.h"
#include "util/hexdump.h"
#include <iostream>
#include <stdio.h>

namespace OpenPST {
	class Meid {
		static uint8_t calculateChecksum(uint8_t* data, uint8_t size);
		static bool verifyChecksum(uint8_t* data, uint8_t size);
	};
}

#endif // _UTIL_MEID_H
