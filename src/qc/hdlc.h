/**
* LICENSE PLACEHOLDER
*/

#ifndef _UTIL_HDLC_H
#define _UTIL_HDLC_H

#include "include/definitions.h"
#include "crc.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

/* QCDM protocol frames are pseudo Async HDLC frames which end with a 3-byte
*  trailer. This trailer consists of the 16-bit CRC of the frame plus an ending
* "async control character" whose value is 0x7E.  The frame *and* the CRC are
* escaped before adding the trailing control character so that the control
* character (0x7E) and the escape marker (0x7D) are never seen in the frame.
*/
#define HDLC_ESC_CHAR			0x7D  /* Escape sequence 1st character value */
#define HDLC_ESC_MASK			0x20  /* Escape sequence complement value */
#define HDLC_CONTROL_CHAR		0x7E
#define HDLC_TRAILER_LENGTH		0x03

int hdlc_request(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize);
int hdlc_response(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize);
int hdlc_escape(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize);
int hdlc_unescape(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize);

#endif // _UTIL_HDLC_H
