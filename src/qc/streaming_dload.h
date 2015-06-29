/**
* LICENSE PLACEHOLDER
*
* @file streaming_dload.h
* @package OpenPST
* @brief Streaming DLOAD definitions and structures
* @see DCN 80-V5348-1 J
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

/*
 * 
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

#define STREAMING_DLOAD_MAGIC_SIZE 32
#define STREAMING_DLOAD_FLASH_ID_MAX_SIZE    32
#define STREAMING_DLOAD_MESSAGE_SIZE  64
#define STREAMING_DLOAD_MAX_SECTORS 32
#define STREAMING_DLOAD_MAX_PACKET_SIZE 2048

enum STREAMING_DLOAD_COMMAND {
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
    STREAMING_DLOAD_CALCULATE_SHA1_HASH          = 0x23, // depreciated
    STREAMING_DLOAD_CALCULATE_SHA1_HASH_RESPONSE = 0x24, // depreciated

    //25-2F commands described in [Q2]

    STREAMING_DLOAD_UNFRAMED_STREAM_WRITE           = 0x30,
    STREAMING_DLOAD_UNFRAMED_STREAM_WRITE_RESPONSE  = 0x31,
    STREAMING_DLOAD_QFPROM_WRITE                    = 0x32,
    STREAMING_DLOAD_QFPROM_WRITE_RESPONSE           = 0x33,
    STREAMING_DLOAD_QFPROM_READ                     = 0x34,
    STREAMING_DLOAD_QFPROM_READ_RESPONSE            = 0x35

    //36-FE Reserved
};

enum STREAMING_DLOAD_ERROR_CODE {
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

enum STREAMING_DLOAD_OPEN_MODE {
	STREAMING_DLOAD_OPEN_MODE_BOOTLOADER_DOWNLOAD		= 0x01,
	STREAMING_DLOAD_OPEN_MODE_BOOTABLE_IMAGE_DOWNLOAD	= 0x02,
	STREAMING_DLOAD_OPEN_MODE_CEFS_IMAGE_DOWNLOAD		= 0x03
};

enum STREAMING_DLOAD_OPEN_MULTI_MODE {
	STREAMING_DLOAD_OPEN_MULTI_MODE_NONE		= 0x00,  
	STREAMING_DLOAD_OPEN_MULTI_MODE_PBL			= 0x01,  // Primary boot loader
	STREAMING_DLOAD_OPEN_MULTI_MODE_QCSBLHDCFG	= 0x02,  // Qualcomm secondary boot loader header and config data 
	STREAMING_DLOAD_OPEN_MULTI_MODE_QCSBL		= 0x03,	 // Qualcomm secondary boot loader
	STREAMING_DLOAD_OPEN_MULTI_MODE_OEMSBL		= 0x04,	 // OEM secondary boot loader (max payload: 512 octets)
	STREAMING_DLOAD_OPEN_MULTI_MODE_AMSS		= 0x05,	 // AMSS modem executable (max payload: 512 octets)
	STREAMING_DLOAD_OPEN_MULTI_MODE_APPS		= 0x06,	 // AMSS applications executable (max payload: 512 octets)
	STREAMING_DLOAD_OPEN_MULTI_MODE_OBL			= 0x07,	 // MSM6250 OTP boot loader 
	STREAMING_DLOAD_OPEN_MULTI_MODE_FOTAUI		= 0x08,	 // FOTA UI binary 
	STREAMING_DLOAD_OPEN_MULTI_MODE_CEFS		= 0x09,	 // Compact EFS2 image
	STREAMING_DLOAD_OPEN_MULTI_MODE_APPSBL		= 0x0A,	 // AMSS applications boot loader (max payload: 512 octets)
	STREAMING_DLOAD_OPEN_MULTI_MODE_APPS_CEFS	= 0x0B,  // Apps CEFS image
	STREAMING_DLOAD_OPEN_MULTI_MODE_FLASH_BIN	= 0x0C,  // Flash.bin for Windows Mobile
	STREAMING_DLOAD_OPEN_MULTI_MODE_DSP1		= 0x0D,	 // DSP1 runtime image
	STREAMING_DLOAD_OPEN_MULTI_MODE_CUSTOM		= 0x0E,	 // Image for user-defined partition User (max payload: 512 octets)
	STREAMING_DLOAD_OPEN_MULTI_MODE_DBL			= 0x0F,	 // DBL image for Secure  Boot 2.0 architecture 
	STREAMING_DLOAD_OPEN_MULTI_MODE_OSBL		= 0x10,	 // OSBL image for Secure  Boot 2.0 architecture 
	STREAMING_DLOAD_OPEN_MULTI_MODE_FSBL		= 0x11,	 // FSBL image for Secure  Boot 2.0 architecture 
	STREAMING_DLOAD_OPEN_MULTI_MODE_DSP2		= 0x12,	 // DSP2 executable 
	STREAMING_DLOAD_OPEN_MULTI_MODE_RAW			= 0x13,	 // Apps EFS2 raw image 
	STREAMING_DLOAD_OPEN_MULTI_MODE_ROFS1		= 0x14,  // Symbian
	STREAMING_DLOAD_OPEN_MULTI_MODE_ROFS2		= 0x15,  // Symbian
	STREAMING_DLOAD_OPEN_MULTI_MODE_ROFS3		= 0x16,  // Symbian
														 // 0x17-0x1F - RESERVED
	STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_USER	= 0x21,  // EMMC card USER partition image 
	STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_BOOT0	= 0x22,  // EMMC card BOOT0 partition image 
	STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_BOOT1	= 0x23,  // EMMC card BOOT1 partition image 
	STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_RPMB	= 0x24,  // partition1
	STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_GPP1	= 0x25,  // partition1-4
	STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_GPP2	= 0x26,   
	STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_GPP3	= 0x27,   
	STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_GPP4	= 0x28    
};

PACKED(typedef struct { // 0x01
    uint8_t command;
	uint8_t magic[STREAMING_DLOAD_MAGIC_SIZE];
    uint8_t version;
    uint8_t compatibleVersion;
    uint8_t featureBits;
}) streaming_dload_hello_tx_t;

PACKED(typedef struct { // 0x02
	uint8_t  command;
	uint8_t  magic[STREAMING_DLOAD_MAGIC_SIZE];
	uint8_t  version;
	uint8_t  compatibleVersion;
	uint32_t maxPreferredBlockSize;
	uint32_t baseFlashAddress;
	uint8_t  flashIdLength;
}) streaming_dload_hello_rx_header_t;

PACKED(typedef struct { // 0x02
    uint8_t  command;
	uint8_t  magic[STREAMING_DLOAD_MAGIC_SIZE];
    uint8_t  version;
    uint8_t  compatibleVersion;
    uint32_t maxPreferredBlockSize;
    uint32_t baseFlashAddress;
    uint8_t  flashIdLength;
	uint8_t  flashIdenfier[STREAMING_DLOAD_FLASH_ID_MAX_SIZE];
    uint16_t windowSize;
    uint16_t numberOfSectors;
    uint16_t sectorSizes[STREAMING_DLOAD_MAX_SECTORS * 4];
	uint16_t featureBits;
}) streaming_dload_hello_rx_t;

PACKED(typedef struct { // 0x03
    uint8_t command;
    uint32_t address;
    uint16_t length;
}) streaming_dload_read_tx_t;

PACKED(typedef struct { // 0x04
    uint8_t command;
    uint32_t address;
    uint8_t data[0];
}) streaming_dload_read_rx_t;

PACKED(typedef struct {// 0x05
    uint8_t command;
    uint32_t address;
    uint8_t* data;
}) streaming_dload_simple_write_tx_t;

PACKED(typedef struct { // 0x06
    uint8_t command;
    uint32_t address;
}) streaming_dload_simple_write_rx_t;

PACKED(typedef struct {// 0x07
    uint8_t command;
    uint32_t address;
    uint8_t* data;
}) streaming_dload_stream_write_tx_t;

PACKED(typedef struct { // 0x08
    uint8_t command;
    uint32_t address;
}) streaming_dload_stream_write_rx_t;

PACKED(typedef struct {// 0x09
    uint8_t command;
    uint32_t identifier; // this can be any amount of data. its just sent back to us in the response to validate the NOP call
}) streaming_dload_nop_tx_t;

PACKED(typedef struct { // 0x0A
    uint8_t command;
	uint32_t identifier; // response should have same as in transmitted packet
}) streaming_dload_nop_rx_t;

PACKED(typedef struct {// 0x0B
    uint8_t command;
}) streaming_dload_reset_tx_t;

PACKED(typedef struct { // 0x0C
    uint8_t command;
}) streaming_dload_reset_rx_t;

PACKED(typedef struct { // 0x0D
    uint8_t command;
    uint32_t code;
	uint8_t text[STREAMING_DLOAD_MESSAGE_SIZE];
}) streaming_dload_error_rx_t;

PACKED(typedef struct { // 0x0E
    uint8_t command;
	uint8_t text[STREAMING_DLOAD_MESSAGE_SIZE];
}) streaming_dload_log_rx_t;

PACKED(typedef struct { // 0x0F
    uint8_t command;
    uint64_t code;
}) streaming_dload_unlock_tx_t;

PACKED(typedef struct { // 0x10
    uint8_t command;
}) streaming_dload_unlock_rx_t;

PACKED(typedef struct { // 0x11
    uint8_t command;
}) streaming_dload_power_off_tx_t;

PACKED(typedef struct { // 0x12
    uint8_t command;
}) streaming_dload_power_off_rx_t;

PACKED(typedef struct { // 0x13
    uint8_t command;
    uint8_t mode;
}) streaming_dload_open_tx_t;

PACKED(typedef struct { // 0x14
    uint8_t command;
}) streaming_dload_open_rx_t;

PACKED(typedef struct { // 0x15
    uint8_t command;
}) streaming_dload_close_tx_t;

PACKED(typedef struct { // 0x16
    uint8_t command;
}) streaming_dload_close_rx_t;

PACKED(typedef struct { // 0x17
    uint8_t command;
    uint8_t mode; // 0x00 - Untrusted, 0x01 - Trusted
}) streaming_dload_security_mode_tx_t;

PACKED(typedef struct { // 0x18
    uint8_t command;
}) streaming_dload_security_mode_rx_t;

PACKED(typedef struct { // 0x19
    uint8_t command;
	uint8_t overrideExisting; // 0x00 no override, 0x01 override existing table
    uint8_t data[512]; // max 512 bytes
}) streaming_partition_table_tx_t;


PACKED(typedef struct { // 0x1A
    uint8_t command;
    uint8_t status; // 0x0 – Partition table accepted
                    // 0x1 – Partition table differs, override is accepted
                    // 0x2 – Partition table format not recognized, does not accept override
                    // 0x3 – Erase operation failed
}) streaming_dload_partition_table_rx_t;

PACKED(typedef struct  { // 0x1B
    uint8_t command;
    uint8_t type;
}) streaming_dload_open_multi_image_tx_t;

PACKED(typedef struct { // 0x1B
	uint8_t command;
	uint8_t type;
}) streaming_dload_open_multi_image_payload_tx_t;

PACKED(typedef struct { // 0x1C
	uint8_t command;
	uint8_t status; // 0x00 Open successful
					// 0x01 Payload length exceeded, fail
					// 0x02 No payload expected, fail
					// 0x03 Payload required, fail
					// 0x04 Block 0 write protected, fail
}) streaming_dload_open_multi_image_rx_t;

/*
* This packet will erase the whole flash. If the operation fails, you may
* have to restore the device via jtag. Use with caution
*/
PACKED(typedef struct { // 0x1D
	uint8_t command;
}) streaming_dload_erase_flash_tx_t;

PACKED(typedef struct { // 0x1E
	uint8_t command;
}) streaming_dload_erase_flash_rx_t;


PACKED(typedef struct { // 0x1F
	uint8_t command;
}) streaming_dload_get_ecc_state_tx_t;

PACKED(typedef struct { // 0x20
	uint8_t command;
	uint8_t status; // 0x00 ECC generation/check disabled
					// 0x01 ECC generation/check enabled
}) streaming_dload_get_ecc_state_rx_t;

PACKED(typedef struct { // 0x21
	uint8_t command;
	uint8_t status; // 0x00 disable, 0x01 enabled
}) streaming_dload_set_ecc_state_tx_t;

PACKED(typedef struct { // 0x22
	uint8_t command;
}) streaming_dload_set_ecc_state_rx_t;


PACKED(typedef struct { // 0x30
	uint8_t command;
	uint8_t padding[2]; // should be set to 0x000000
	uint32_t address;
	uint32_t length;
	uint8_t* data;
}) streaming_dload_unframed_stream_write_tx_t;

PACKED(typedef struct { // 0x31
	uint8_t command;
	uint32_t address;
}) streaming_dload_unframed_stream_write_rx_t;

PACKED(typedef struct { // 0x32
    uint8_t command;
    uint32_t rowAddress;
    uint32_t lsb;
    uint32_t msb;
}) streaming_dload_qfprom_write_tx_t;

PACKED(typedef struct { // 0x33
    uint8_t command;
    uint32_t rowAddress;
}) streaming_dload_qfprom_write_rx_t;


PACKED(typedef struct { // 0x34
    uint8_t command;
    uint32_t rowAddress;
    uint32_t addressType; // 0x00 - Corrected Reads, 0x01 - Raw Reads
    uint32_t msb;
}) streaming_dload_qfprom_read_tx_t;


PACKED(typedef struct { // 0x35
    uint8_t command;
    uint32_t rowAddress;
    uint32_t lsb;
    uint32_t msb;
}) streaming_dload_qfprom_read_rx_t;



#endif // _QC_STREAMING_DLOAD_H

