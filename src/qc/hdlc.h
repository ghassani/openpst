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

size_t dm_unescape(const char *inbuf, size_t inbuf_len, char *outbuf, size_t outbuf_len, bool *escaping);
size_t dm_escape(unsigned char *inbuf, size_t inbuf_len, unsigned char *outbuf, size_t outbuf_len);
size_t dm_encapsulate_buffer(unsigned char *input, size_t inputLength, char *output, size_t outputLength);

int dload_request(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize);
int dload_response(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize);
int dload_escape(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize);
int dload_unescape(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize);

#endif // _UTIL_HDLC_H
