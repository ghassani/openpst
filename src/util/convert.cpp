/**
* LICENSE PLACEHOLDER
*
* @file convert.h
* @package OpenPST
* @brief QCDM response conversion helper functions
*
* @author Gassan Idriss <ghassani@gmail.com>
* @author Matteson Raab <mraabhimself@gmail.com>
*/

#include "convert.h"

/**
* hexToString
* @param input
* @param length
*/
std::string hexToString(char *input, int length) {
    char outputBuffer[128];
    std::string result;

    for (int i = 0; i <= length; i++) {
    #ifdef _WIN32
        sprintf_s(outputBuffer, "%c", hex_trans_display[input[i]]);
    #else
        sprintf(outputBuffer, "%c", hex_trans_display[input[i]]);
    #endif

    result.append(outputBuffer);
    }

    return result;
}

/**
* bytesToHex
* @param input
* @param length
*/
std::string bytesToHex(unsigned char* input, int size, bool byteswap) {
  std::string result;

  if (byteswap) {
    for (int i = size; i >= 0; i--) {
        char buffer[2];
        sprintf(buffer, "%02x", input[i]);
        result.append(buffer);
    }
  } else {
    for (int i = 0; i < size; i++) {
        char buffer[2];
        sprintf(buffer, "%02x", input[i]);
        result.append(buffer);
    }
  }

  return result;
}
