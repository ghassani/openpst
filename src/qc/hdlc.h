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

int hdlc_request(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize);
int hdlc_response(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize);
int hdlc_escape(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize);
int hdlc_unescape(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize);

#endif // _UTIL_HDLC_H
