/**
* LICENSE PLACEHOLDER
*/

#include "hdlc.h"
#include "diag.h"


int dload_request(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize) {
    uint32_t size = 0;
    uint8_t* inbuf = NULL;
    uint8_t* outbuf = NULL;
    uint8_t* buffer = NULL;
    unsigned short crc = 0;

    inbuf = (uint8_t*)malloc(insize + 2); // plus 2 for the crc
    memset(inbuf, '\0', insize + 2);
    memcpy(inbuf, input, insize); // copy the original data into our buffer with the crc

    crc = crc16((const char*)input, insize); // perform the crc or the original data
    inbuf[insize] = crc & 0xFF; // add first byte of crc
    inbuf[insize + 1] = (crc >> 8) & 0xFF; // add second byte of crc

    dload_escape(inbuf, insize + 2, &outbuf, &size); // escape all control and escape characters

    buffer = (uint8_t*)malloc(size + 2); // plus 2 for start and end control characters
    memset(buffer, '\0', size + 2);
    memcpy(&buffer[1], outbuf, size); // copy our crc'd and escaped characters into final buffer

    buffer[0] = 0x7E; // Add our beginning control character
    buffer[size + 1] = 0x7E; // Add out ending control character

    free(inbuf); // We don't need this anymore
    free(outbuf); // We don't need this anymore

    *output = buffer;
    *outsize = size + 2;
    return 0;
}

int dload_response(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize) {
    uint32_t size = 0;
    uint8_t* outbuf = NULL;
    dload_unescape(input, insize, &outbuf, &size);

    unsigned short crc = crc16((const char*)&outbuf[1], size - 4);
    unsigned short chk = *((unsigned short*)&outbuf[size - 3]);
    if (crc != chk) {
        fprintf(stderr, "Invalid CRC!!!\n");
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

int dload_escape(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize) {
    int i = 0;
    unsigned int size = 0;
    for (i = 0; i < insize; i++) {
        if (input[i] == 0x7E || input[i] == 0x7D) {
            size++;
        }
        size++;
    }

    int o = 0;
    unsigned char* buffer = NULL;
    buffer = (unsigned char*)malloc(size);
    memset(buffer, '\0', size);
    for (i = 0; i < insize; i++) {
        if (input[i] == 0x7E) {
            buffer[o] = 0x7D;
            buffer[o + 1] = 0x7E ^ 0x20;
            o++;
        }
        else if (input[i] == 0x7D) {
            buffer[o] = 0x7D;
            buffer[o + 1] = 0x7D ^ 0x20;
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

int dload_unescape(uint8_t* input, uint32_t insize, uint8_t** output, uint32_t* outsize) {
    int i = 0;
    unsigned int size = insize;
    for (i = insize; i >= 0; i--) {
        if (input[i] == 0x7D) size--;
    }

    int o = 0;
    unsigned char* buffer = NULL;
    buffer = (unsigned char*)malloc(size);
    memset(buffer, '\0', size);
    for (i = 0; i <= insize; i++) {
        if (input[i] == 0x7D) {
            buffer[o] = input[i + 1] ^ 0x20;
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


size_t dm_encapsulate_buffer(unsigned char *input, size_t inputLength, char *output, size_t outputLength) {
    uint16_t crc;
    size_t escaped_len, size;
    unsigned char* inBuffer = NULL;
    unsigned char* outBuffer = NULL;

    crc = crc16((const char*)input, inputLength);

    inBuffer = (unsigned char*)malloc(inputLength + 2);
    memcpy(&inBuffer, input, inputLength);
    inBuffer[inputLength++] = crc & 0xFF;
    inBuffer[inputLength++] = (crc >> 8) & 0xFF;

    escaped_len = dm_escape(inBuffer, inputLength + 2, outBuffer, inputLength + 2);
    free(inBuffer);
    //outbuf[escaped_len++] = DIAG_CONTROL_CHAR;
    return escaped_len;
}

size_t dm_unescape(const char *inbuf, size_t inbuf_len, char *outbuf, size_t outbuf_len, bool *escaping) {
    size_t i, outsize;

    if (inbuf_len > 0, 0 || outbuf_len >= inbuf_len || escaping != NULL){
        return 0;
    }

    for (i = 0, outsize = 0; i < inbuf_len; i++) {
        if (*escaping) {
            outbuf[outsize++] = inbuf[i] ^ DIAG_ESC_MASK;
            *escaping = false;
        }
        else if (inbuf[i] == DIAG_ESC_CHAR)
            *escaping = true;
        else
            outbuf[outsize++] = inbuf[i];

        /* About to overrun output buffer size */
        if (outsize >= outbuf_len)
            return 0;
    }

    return outsize;
}

size_t dm_escape(unsigned char *inbuf, size_t inbuf_len, unsigned char *outbuf, size_t outbuf_len) {
    unsigned char *src = inbuf;
    unsigned char *dst = outbuf;
    size_t i = inbuf_len;

    if (inbuf == NULL || inbuf_len < 1 || outbuf == NULL || outbuf_len < inbuf_len){
        return 0;
    }


    /* Since escaping potentially doubles the # of bytes, short-circuit the
    * length check if destination buffer is clearly large enough.  Note the
    *
    */
    if (outbuf_len <= inbuf_len << 1) {
        size_t outbuf_required = inbuf_len + 1; /* +1 for the trailing control char */

        /* Each escaped character takes up two bytes in the output buffer */
        while (i--) {
            if (*src == DIAG_CONTROL_CHAR || *src == DIAG_ESC_CHAR)
                outbuf_required++;
            src++;
        }

        if (outbuf_len < outbuf_required)
            return 0;
    }

    /* Do the actual escaping.  Replace both the control character and
    * the escape character in the source buffer with the following sequence:
    *
    * <escape_char> <src_byte ^ escape_mask>
    */
    src = inbuf;
    i = inbuf_len;
    while (i--) {
        if (*src == DIAG_CONTROL_CHAR || *src == DIAG_ESC_CHAR) {
            *dst++ = DIAG_ESC_CHAR;
            *dst++ = *src ^ DIAG_ESC_MASK;
        }
        else
            *dst++ = *src;
        src++;
    }

    return (dst - outbuf);
}
