/**
* LICENSE PLACEHOLDER
*/

/*
 * @see DCN 80-V5348-1 J
 */

#ifndef _QC_STREAMING_DLOAD_H
#define _QC_STREAMING_DLOAD_H

#include "include/definitions.h"

#define STREAMING_DLOAD_FEATURE_BIT_UNCOMPRESSED_DOWNLOAD	0x00000001
#define STREAMING_DLOAD_FEATURE_BIT_NAND_BOOTABLE_IMAGE		0x00000002
#define STREAMING_DLOAD_FEATURE_BIT_NAND_BOOT_LOADER		0x00000004
#define STREAMING_DLOAD_FEATURE_BIT_MULTI_IMAGE				0x00000008
#define STREAMING_DLOAD_FEATURE_BIT_SECTOR_ADDRESSES		0x00000010

#define STREAMING_DLOAD_SECURITY_MODE_UNTRUSTED				0x00
#define STREAMING_DLOAD_SECURITY_MODE_TRUSTED				0x01

#define STREAMING_DLOAD_PARTITION_TABLE_ACCEPTED					0x00
#define STREAMING_DLOAD_PARTITION_TABLE_DIFFERS_OVERRIDE_ACCEPTED	0x01
#define STREAMING_DLOAD_PARTITION_TABLE_FORMAT_NOT_RECOGNIZED		0x02
#define STREAMING_DLOAD_PARTITION_TABLE_ERASE_FAILED				0x03

#define STREAMING_DLOAD_OPEN_MULTI_OPEN_SUCCESS						0x00
#define STREAMING_DLOAD_OPEN_MULTI_PAYLOAD_LENGTH_EXCEEDED			0x01
#define STREAMING_DLOAD_OPEN_MULTI_NO_PAYLOAD_EXPECTED				0x02
#define STREAMING_DLOAD_OPEN_MULTI_PAYLOAD_REQUIRED_FAIL			0x03
#define STREAMING_DLOAD_OPEN_MULTI_BLOCK_WRITE_PROTECTED			0x04

enum STREAMING_DLOAD_COMMAND : uint8_t {
    // 0x00 - Illegal
    STREAMING_DLOAD_HELLO                        = 0x01,
    STREAMING_DLOAD_HELLO_RESPONSE               = 0x02,
    STREAMING_DLOAD_READ                         = 0x03,
    STREAMING_DLOAD_READ_DATA                    = 0x04,
    STREAMING_DLOAD_SIMPLE_WRITE                 = 0x05,
    STREAMING_DLOAD_WRITE_OK                     = 0x06,
    STREAMING_DLOAD_STREAM_WRITE                 = 0x07,
    STREAMING_DLOAD_BLOCK_WRITTEN                = 0x08,
    STREAMING_DLOAD_NOP                          = 0x09,
    STREAMING_DLOAD_NOP_RESPONSE                 = 0x0A,
    STREAMING_DLOAD_RESET                        = 0x0B,
    STREAMING_DLOAD_RESET_ACK                    = 0x0C,
    STREAMING_DLOAD_ERROR                        = 0x0D,
    STREAMING_DLOAD_LOG                          = 0x0E,
    STREAMING_DLOAD_UNLOCK                       = 0x0F,
    STREAMING_DLOAD_UNLOCKED                     = 0x10,
    STREAMING_DLOAD_POWER_OFF                    = 0x11,
    STREAMING_DLOAD_POWERING_DOWN                = 0x12,
    STREAMING_DLOAD_OPEN                         = 0x13,
    STREAMING_DLOAD_OPENED                       = 0x14,
    STREAMING_DLOAD_CLOSE                        = 0x15,
    STREAMING_DLOAD_CLOSED                       = 0x16,
    STREAMING_DLOAD_SECURITY_MODE                = 0x17,
    STREAMING_DLOAD_SECUIRTY_MODE_RECEIVED       = 0x18,
    STREAMING_DLOAD_PARTITION_TABLE              = 0x19,
    STREAMING_DLOAD_PARTITION_TABLE_RECEIVED     = 0x1A,
    STREAMING_DLOAD_OPEN_MULTI_IMAGE             = 0x1B,
    STREAMING_DLOAD_OPENED_MULTI_IMAGE           = 0x1C,
    STREAMING_DLOAD_ERASE_FLASH                  = 0x1D,
    STREAMING_DLOAD_FLASH_ERASED                 = 0x1E,
    STREAMING_DLOAD_GET_ECC_STATE                = 0x1F,
    STREAMING_DLOAD_CURRENT_ECC_STATE            = 0x20,
    STREAMING_DLOAD_SET_ECC                      = 0x21,
    STREAMING_DLOAD_SET_ECC_RESPONSE             = 0x22,
    STREAMING_DLOAD_CALCULATE_SHA1_HASH          = 0x23,
    STREAMING_DLOAD_CALCULATE_SHA1_HASH_RESPONSE = 0x24,

    //25-2F commands described in [Q2]

    STREAMING_DLOAD_UNFRAMED_STREAM_WRITE           = 0x30,
    STREAMING_DLOAD_UNFRAMED_STREAM_WRITE_RESPONSE  = 0x31,
    STREAMING_DLOAD_QFPROM_WRITE                    = 0x32,
    STREAMING_DLOAD_QFPROM_WRITE_RESPONSE           = 0x33,
    STREAMING_DLOAD_QFPROM_READ                     = 0x34,
    STREAMING_DLOAD_QFPROM_READ_RESPONSE            = 0x35

    //36-FE Reserved
};



enum STREAMING_DLOAD_ERROR_CODE : uint32_t {
    STREAMING_DLOAD_ERROR_ERROR_INVALID_DESTINATION_ADDRESS     = 0x02,
    STREAMING_DLOAD_ERROR_INVALID_LENGTH                        = 0x03,
    STREAMING_DLOAD_ERROR_UNEXPECTED_END_OF_PACKET              = 0x04,
    STREAMING_DLOAD_ERROR_INVALID_COMMAND                       = 0x05,
    STREAMING_DLOAD_ERROR_WRONG_FLASH_INTELLIGENT_ID            = 0x08,
    STREAMING_DLOAD_ERROR_BAD_PROGRAMMING_VOLTAGE               = 0x09,
    STREAMING_DLOAD_ERROR_WRITE_VERIFY_FAILED                   = 0x0A,
    STREAMING_DLOAD_ERROR_INCORRECT_SECURITY_CODE               = 0x0C,
    STREAMING_DLOAD_ERROR_CANNOT_POWER_DOWN                     = 0x0D,
    STREAMING_DLOAD_ERROR_NAND_FLASH_PROGRAMMING_NOT_SUPPORTED  = 0x0E,
    STREAMING_DLOAD_ERROR_COMMAND_OUT_OF_SQUENCE                = 0x0F,
    STREAMING_DLOAD_ERROR_CLOSE_DID_NOT_SUCCEED                 = 0x10,
    STREAMING_DLOAD_ERROR_INCOMPATIBLE_FEATURES_BITS            = 0x11,
    STREAMING_DLOAD_ERROR_OUT_OF_SPACE                          = 0x12,
    STREAMING_DLOAD_ERROR_INVALID_SECURITY_MODE                 = 0x13,
    STREAMING_DLOAD_ERROR_MULTI_IMAGE_NAND_NOT_SUPPORTED        = 0x14,
    STREAMING_DLOAD_ERROR_POWER_OFF_COMMAND_NOT_SUPPORTED       = 0x15
    // 0x16 - 0x1A RESERVED FOR ERROR REASONS IN [Q2]
};


PACKED(typedef struct streaming_dload_hello_tx_t { // 0x01
    uint8_t command;
    uint8_t magic[32];
    uint8_t version;
    uint8_t compatibleVersion;
    uint8_t featureBits;
} streaming_dload_hello_tx_t);

PACKED(typedef struct streaming_dload_hello_rx_t { // 0x02
    uint8_t  command;
    uint8_t  magic[32];
    uint8_t  version;
    uint8_t  compatibleVersion;
    uint32_t maxPreferredBlockSize;
    uint32_t baseFlashAddress;
    uint8_t  flashIdLength;
    uint8_t  flashIdenfier;
    uint16_t windowSize;
    uint16_t numberOfSectors;
    uint16_t secrorSizes;
    uint8_t  featureBits;
} streaming_dload_hello_rx_t);

PACKED(typedef struct streaming_dload_read_tx_t { // 0x03
    uint8_t command;
    uint32_t address;
    uint16_t length;
} streaming_dload_read_tx_t);

PACKED(typedef struct streaming_dload_read_rx_t { // 0x04
    uint8_t command;
    uint32_t address;
    uint8_t* data;
} streaming_dload_read_rx_t);

PACKED(typedef struct streaming_dload_simple_write_tx_t {// 0x05
    uint8_t command;
    uint32_t address;
    uint8_t* data;
} streaming_dload_simple_write_tx_t);

PACKED(typedef struct streaming_dload_simple_write_rx_t { // 0x06
    uint8_t command;
    uint32_t address;
} streaming_dload_simple_write_rx_t);

PACKED(typedef struct streaming_dload_stream_write_tx_t {// 0x07
    uint8_t command;
    uint32_t address;
    uint8_t* data;
} streaming_dload_stream_write_tx_t);

PACKED(typedef struct streaming_dload_stream_write_rx_t { // 0x08
    uint8_t command;
    uint32_t address;
} streaming_dload_stream_write_rx_t);

PACKED(typedef struct streaming_dload_nop_tx_t {// 0x09
    uint8_t command;
    uint8_t* data;
} streaming_dload_nop_tx_t);

PACKED(typedef struct streaming_dload_nop_rx_t { // 0x0A
    uint8_t command;
    uint8_t* data;
} streaming_dload_nop_rx_t);

PACKED(typedef struct streaming_dload_reset_tx_t {// 0x0B
    uint8_t command;
} streaming_dload_reset_tx_t);

PACKED(typedef struct streaming_dload_reset_rx_t { // 0x0C
    uint8_t command;
} streaming_dload_reset_rx_t);

PACKED(typedef struct streaming_dload_error_rx_t { // 0x0D
    uint8_t command;
    uint32_t code;
    uint8_t* text;
} streaming_dload_error_rx_t);

PACKED(typedef struct streaming_dload_log_rx_t { // 0x0E
    uint8_t command;
    uint8_t* text;
} streaming_dload_log_rx_t);

PACKED(typedef struct streaming_dload_unlock_tx_t { // 0x0F
    uint8_t command;
    uint64_t code;
} streaming_dload_unlock_tx_t);

PACKED(typedef struct streaming_dload_unlock_rx_t { // 0x10
    uint8_t command;
} streaming_dload_unlock_rx_t);

PACKED(typedef struct streaming_dload_power_off_tx_t { // 0x11
    uint8_t command;
} streaming_dload_power_off_tx_t);

PACKED(typedef struct streaming_dload_power_off_rx_t { // 0x12
    uint8_t command;
} streaming_dload_power_off_rx_t);

PACKED(typedef struct streaming_dload_open_tx_t { // 0x13
    uint8_t command;
    uint8_t mode;
} streaming_dload_open_tx_t);

PACKED(typedef struct streaming_dload_open_rx_t { // 0x14
    uint8_t command;
} streaming_dload_open_rx_t);

PACKED(typedef struct streaming_dload_close_tx_t { // 0x15
    uint8_t command;
} streaming_dload_close_tx_t);

PACKED(typedef struct streaming_dload_close_rx_t { // 0x16
    uint8_t command;
} streaming_dload_close_rx_t);

PACKED(typedef struct streaming_dload_security_mode_tx_t { // 0x17
    uint8_t command;
    uint8_t mode; // 0x00 - Untrusted, 0x01 - Trusted
} streaming_dload_security_mode_tx_t);

PACKED(typedef struct streaming_dload_security_mode_rx_t { // 0x18
    uint8_t command;
} streaming_dload_security_mode_rx_t);

PACKED(typedef struct streaming_partition_table_tx_t { // 0x19
    uint8_t command;
    uint8_t override; // 0x00 no override, 0x01 override existing table
    uint8_t* partitionTable; // max 512 bytes
} streaming_partition_table_tx_t);

PACKED(typedef struct streaming_dload_partition_table_rx_t { // 0x1A
    uint8_t command;
    uint8_t status; // 0x0 – Partition table accepted
                    // 0x1 – Partition table differs, override is accepted
                    // 0x2 – Partition table format not recognized, does not accept override
                    // 0x3 – Erase operation failed
} streaming_dload_partition_table_rx_t);

PACKED(typedef struct streaming_dload_open_multi_image_tx_t { // 0x1B
    uint8_t command;
    uint8_t type;
    uint8_t* data;
} streaming_dload_open_multi_image_tx_t);

PACKED(typedef struct streaming_dload_open_multi_image_rx_t { // 0x16
    uint8_t command;
} streaming_dload_open_multi_image_rx_t);

PACKED(typedef struct streaming_dload_qfprom_write_tx_t { // 0x32
    uint8_t command;
    uint32_t rowAddress;
    uint32_t lsb;
    uint32_t msb;
} streaming_dload_qfprom_write_tx_t);

PACKED(typedef struct streaming_dload_qfprom_write_rx_t { // 0x33
    uint8_t command;
    uint32_t rowAddress;
} streaming_dload_qfprom_write_rx_t);

/*
PACKED(typedef struct streaming_dload_qfprom_read_tx_t { // 0x34
    uint8_t command;
    uint3_qc_streaming_dload_h2_t rowAddress;
    uint32_t addressType; // 0x00 - Corrected Reads, 0x01 - Raw Reads
    uint32_t msb;
} streaming_dload_qfprom_read_tx_t);
*/

PACKED(typedef struct streaming_dload_qfprom_read_rx_t { // 0x35
    uint8_t command;
    uint32_t rowAddress;
    uint32_t lsb;
    uint32_t msb;
} streaming_dload_qfprom_read_rx_t);


#endif // _QC_STREAMING_DLOAD_H

