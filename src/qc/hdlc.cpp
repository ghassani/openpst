/**
* LICENSE PLACEHOLDER
*
* @file hdlc.cpp
* @package OpenPST
* @brief HDLC helper functions
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#include "hdlc.h"


int hdlc_request(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize) {
    uint32_t size = 0;
    uint8_t* inbuf = NULL;
    uint8_t* outbuf = NULL;
    uint8_t* buffer = NULL;
    uint16_t crc = 0;

    inbuf = (uint8_t*)malloc(insize + 2); // plus 2 for the crc
    memset(inbuf, '\0', insize + 2);
    memcpy(inbuf, input, insize); // copy the original data into our buffer with the crc

    crc = crc16((const char*)input, insize); // perform the crc or the original data
    inbuf[insize] = crc & 0xFF; // add first byte of crc
    inbuf[insize + 1] = (crc >> 8) & 0xFF; // add second byte of crc

    hdlc_escape(inbuf, insize + 2, &outbuf, &size); // escape all control and escape characters

    buffer = (uint8_t*)malloc(size + 2); // plus 2 for start and end control characters
    memset(buffer, '\0', size + 2);
    memcpy(&buffer[1], outbuf, size); // copy our crc'd and escaped characters into final buffer

    buffer[0] = HDLC_CONTROL_CHAR; // Add our beginning control character
    buffer[size + 1] = HDLC_CONTROL_CHAR; // Add out ending control character

    free(inbuf); // We don't need this anymore
    free(outbuf); // We don't need this anymore

    *output = buffer;
    *outsize = size + 2;
    return 0;
}

int hdlc_response(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize) {
    uint32_t size = 0;
    uint8_t* outbuf = NULL;
    hdlc_unescape(input, insize, &outbuf, &size);

    uint16_t crc = crc16((const char*)&outbuf[1], size - 4);
    uint16_t chk = *((uint16_t*)&outbuf[size - 3]);
    if (crc != chk) {
        printf("Invalid Response CRC\n");
        size += 2;
    }
    uint8_t* buffer = (uint8_t*)malloc(size - 4);
    memset(buffer, '\0', size - 4);
    memcpy(buffer, &outbuf[1], size - 4);
    free(outbuf);

    *output = buffer;
    *outsize = size - 4;
    return 0;
}

int hdlc_escape(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize) {
    int i = 0;
    uint32_t size = 0;
    for (i = 0; i < insize; i++) {
        if (input[i] == HDLC_CONTROL_CHAR || input[i] == HDLC_ESC_CHAR) {
            size++;
        }
        size++;
    }

    int o = 0;
    uint8_t* buffer = NULL;
    buffer = (uint8_t*)malloc(size);
    memset(buffer, '\0', size);
    for (i = 0; i < insize; i++) {
        if (input[i] == HDLC_CONTROL_CHAR) {
            buffer[o] = HDLC_ESC_CHAR;
            buffer[o + 1] = HDLC_CONTROL_CHAR ^ HDLC_ESC_MASK;
            o++;
        }
        else if (input[i] == HDLC_ESC_CHAR) {
            buffer[o] = HDLC_ESC_CHAR;
            buffer[o + 1] = HDLC_ESC_CHAR ^ HDLC_ESC_MASK;
            o++;
        }
        else {
            buffer[o] = input[i];
        }
        o++;
    }

    *outsize = size;
    *output = buffer;
    return 0;
}

int hdlc_unescape(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize) {
    int i = 0;
    uint32_t size = insize;
    for (i = insize; i >= 0; i--) {
        if (input[i] == HDLC_ESC_CHAR) size--;
    }

    int o = 0;
    uint8_t* buffer = NULL;
    buffer = (uint8_t*)malloc(size);
    memset(buffer, '\0', size);
    for (i = 0; i <= insize; i++) {
        if (input[i] == HDLC_ESC_CHAR) {
            buffer[o] = input[i + 1] ^ HDLC_ESC_MASK;
            i++;
        }
        else {
            buffer[o] = input[i];
        }
        o++;
    }

    *outsize = size;
    *output = buffer;
    return 0;
}
