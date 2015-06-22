/**
* LICENSE PLACEHOLDER
*
* @file hexdump.h
* @package OpenPST
* @brief hexdump helper functions
*
* @author Gassan Idriss <ghassani@gmail.com>
* @author https://github.com/posixninja/DLOADTool
*/

#ifndef _UTIL_HEXDUMP_H
#define _UTIL_HEXDUMP_H

#include "include/definitions.h"
#include <iostream>
#include <stdio.h>
#include <QVariant>

const char hex_trans[] =
    "................................ !\"#$%&'()*+,-./0123456789"
    ":;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklm"
    "nopqrstuvwxyz{|}~...................................."
    "....................................................."
    "........................................";

void hexdump(unsigned char *data, unsigned int amount);

void hexdump(unsigned char *data, unsigned int amount, QString& out);

#endif
