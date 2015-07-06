/**
* LICENSE PLACEHOLDER
*
* @file hdlc.h
* @package OpenPST
* @brief HDLC helper functions definitions
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#ifndef _UTIL_HDLC_H
#define _UTIL_HDLC_H

#include "include/definitions.h"
#include "crc.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

/* QCDM protocol frames are pseudo Async HDLC frames which end with a 3-byte
*  trailer. This trailer consists of the 16-bit CRC of the frame plus an ending
* "async control character" whose value is 0x7E.  The frame *and* the CRC are
* escaped before adding the trailing control character so that the control
* character (0x7E) and the escape marker (0x7D) are never seen in the frame.
*/
#define HDLC_ESC_CHAR			0x7D  /* Escape sequence 1st character value */
#define HDLC_ESC_MASK			0x20  /* Escape sequence complement value */
#define HDLC_CONTROL_CHAR		0x7E
#define HDLC_OVERHEAD_LENGTH	4
#define HDLC_TRAILER_LENGTH		3
#define HDLC_LEADING_LENGTH		1


int hdlc_request(uint8_t* in, size_t inSize, uint8_t** out, size_t &outSize);
int hdlc_response(uint8_t* in, size_t inSize, uint8_t** out, size_t &outSize);
int hdlc_request(std::vector<uint8_t> &data);
int hdlc_response(std::vector<uint8_t> &data);

#endif // _UTIL_HDLC_H
