/**
* LICENSE PLACEHOLDER
*
* @file crc.h
* @package OpenPST
* @brief crc helper functions
*
* @author Gassan Idriss <ghassani@gmail.com>
* @author https://github.com/posixninja/DLOADTool
*/

#include "crc.h"

uint16_t crc16(const char *buffer, size_t len) {
    uint16_t crc = 0xffff;

    while (len--)
        crc = crc_table[(crc ^ *buffer++) & 0xff] ^ (crc >> 8);
    return ~crc;
}
