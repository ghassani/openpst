/**
* LICENSE PLACEHOLDER
*
* @file sahara.h
* @package OpenPST
* @brief Sahara protocol definitions and structures
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#ifndef _QC_SAHARA_H
#define _QC_SAHARA_H

#ifndef SAHARA_MAX_PACKET_SIZE
#define SAHARA_MAX_PACKET_SIZE 0x400
#endif

#ifndef SAHARA_LOG_LENGTH
#define SAHARA_LOG_LENGTH 0x64
#endif

#ifndef SAHARA_MAX_MEMORY_DATA_SIZE
#define SAHARA_MAX_MEMORY_DATA_SIZE 0x1000
#endif

#ifndef SAHARA_RAM_ZI_SIZE
#define SAHARA_RAM_ZI_SIZE 0x20000
#endif

#ifndef SAHARA_MAX_MEMORY_REQUEST_SIZE
#define SAHARA_MAX_MEMORY_REQUEST_SIZE 0x50000
#endif

/**
 * These are all known commands, both rx and tx.
 */
enum SAHARA_COMMAND {
    SAHARA_HELLO                    = 0x01, // Initialize connection and protocol
    SAHARA_HELLO_RESPONSE           = 0x02, // Acknowledge connection/protocol, mode of operation
    SAHARA_READ_DATA                = 0x03, // Read specified number of bytes from host
    SAHARA_END_OF_IMAGE_TRANSFER    = 0x04, // image transfer end / target transfer failure
    SAHARA_DONE                     = 0x05, // Acknowledgement: image transfer is complete
    SAHARA_DONE_RESPONSE            = 0x06, // Target is exiting protocol
    SAHARA_RESET                    = 0x07, // Instruct target to perform a reset
    SAHARA_RESET_RESPONSE           = 0x08, // Indicate to host that target is about to reset
    SAHARA_MEMORY_DEBUG             = 0x09, // Indicate to host: target debug mode & ready to transfer memory content
    SAHARA_MEMORY_READ              = 0x0A, // Read number of bytes, starting from a specified address
    SAHARA_COMMAND_READY            = 0x0B, // Indicate to host: target ready to receive client commands,
    SAHARA_COMMAND_SWITCH_MODE      = 0x0C, // Switch to a mode defined in enum SAHARA_MODE
    SAHARA_COMMAND_EXECUTE          = 0x0D, // Indicate to host: to execute a given client command
    SAHARA_COMMAND_EXECUTE_RESPONSE = 0x0E, // Indicate to host: target command execution status
    SAHARA_COMMAND_EXECUTE_DATA     = 0x0F, // Indicate to target that host is ready to receive (more) data
    SAHARA_MEMORY_DEBUG_64          = 0x10,
    SAHARA_MEMORY_READ_64           = 0x11,
};

/**
 * SAHARA_MODE
 *
 * These are all known modes available
 * The initial hello lets you know which
 * mode the device is currently in. Your
 */
enum SAHARA_MODE {
    SAHARA_MODE_IMAGE_TX_PENDING  = 0x00,
    SAHARA_MODE_IMAGE_TX_COMPLETE = 0x01,
    SAHARA_MODE_MEMORY_DEBUG      = 0x02,
    SAHARA_MODE_COMMAND           = 0x03
};

/**
 * SAHARA_CLIENT_COMMAND
 *
 * When in or switched to command mode, these are
 * all commands that are currently known
 */
enum SAHARA_CLIENT_COMMAND {
      SAHARA_EXEC_CMD_NOP                      = 0x00,
      SAHARA_EXEC_CMD_SERIAL_NUM_READ          = 0x01,
      SAHARA_EXEC_CMD_MSM_HW_ID_READ           = 0x02,
      SAHARA_EXEC_CMD_OEM_PK_HASH_READ         = 0x03,
      SAHARA_EXEC_CMD_SWITCH_TO_DMSS_DLOAD     = 0x04,
      SAHARA_EXEC_CMD_SWITCH_TO_STREAM_DLOAD   = 0x05,
      SAHARA_EXEC_CMD_READ_DEBUG_DATA          = 0x06,
      SAHARA_EXEC_CMD_GET_SOFTWARE_VERSION_SBL = 0x07,
};

enum SAHARA_STATUS_CODE {
  SAHARA_STATUS_SUCCESS                     = 0x00,
  SAHARA_NAK_INVALID_CMD                    = 0x01,
  SAHARA_NAK_PROTOCOL_MISMATCH              = 0x02,
  SAHARA_NAK_INVALID_TARGET_PROTOCOL        = 0x03,
  SAHARA_NAK_INVALID_HOST_PROTOCOL          = 0x04,
  SAHARA_NAK_INVALID_PACKET_SIZE            = 0x05,
  SAHARA_NAK_UNEXPECTED_IMAGE_ID            = 0x06,
  SAHARA_NAK_INVALID_HEADER_SIZE            = 0x07,
  SAHARA_NAK_INVALID_DATA_SIZE              = 0x08,
  SAHARA_NAK_INVALID_IMAGE_TYPE             = 0x09,
  SAHARA_NAK_INVALID_TX_LENGTH              = 0x0A,
  SAHARA_NAK_INVALID_RX_LENGTH              = 0x0B,
  SAHARA_NAK_GENERAL_TX_RX_ERROR            = 0x0C,
  SAHARA_NAK_READ_DATA_ERROR                = 0x0D,
  SAHARA_NAK_UNSUPPORTED_NUM_PHDRS          = 0x0E,
  SAHARA_NAK_INVALID_PDHR_SIZE              = 0x0F,
  SAHARA_NAK_MULTIPLE_SHARED_SEG            = 0x10,
  SAHARA_NAK_UNINIT_PHDR_LOC                = 0x11,
  SAHARA_NAK_INVALID_DEST_ADDR              = 0x12,
  SAHARA_NAK_INVALID_IMG_HDR_DATA_SIZE      = 0x13,
  SAHARA_NAK_INVALID_ELF_HDR                = 0x14,
  SAHARA_NAK_UNKNOWN_HOST_ERROR             = 0x15,
  SAHARA_NAK_TIMEOUT_RX                     = 0x16,
  SAHARA_NAK_TIMEOUT_TX                     = 0x17,
  SAHARA_NAK_INVALID_HOST_MODE              = 0x18,
  SAHARA_NAK_INVALID_MEMORY_READ            = 0x19,
  SAHARA_NAK_INVALID_DATA_SIZE_REQUEST      = 0x1A,
  SAHARA_NAK_MEMORY_DEBUG_NOT_SUPPORTED     = 0x1B,
  SAHARA_NAK_INVALID_MODE_SWITCH            = 0x1C,
  SAHARA_NAK_CMD_EXEC_FAILURE               = 0x1D,
  SAHARA_NAK_EXEC_CMD_INVALID_PARAM         = 0x1E,
  SAHARA_NAK_EXEC_CMD_UNSUPPORTED           = 0x1F,
  SAHARA_NAK_EXEC_DATA_INVALID_CLIENT_CMD   = 0x20,
  SAHARA_NAK_HASH_TABLE_AUTH_FAILURE        = 0x21,
  SAHARA_NAK_HASH_VERIFICATION_FAILURE      = 0x22,
  SAHARA_NAK_HASH_TABLE_NOT_FOUND           = 0x23,
  SAHARA_NAK_TARGET_INIT_FAILURE            = 0x24,
  SAHARA_NAK_IMAGE_AUTH_FAILURE             = 0x25,
  SAHARA_NAK_INVALID_IMG_HASH_TABLE_SIZE    = 0x26
};

/**
* sahara_header_t
*
* The header all transmissions use
* with the exception of a few
*/
typedef struct sahara_header_t {
    uint32_t command;
    uint32_t size;
} sahara_header_t;

typedef struct sahara_packet_t {
    sahara_header_t header;
    uint32_t parameters[2];
} sahara_packet_t;

/**
* sahara_hello_response_t
*
* The initial packet received from the device
* when first opening the COM port. If no data
* is available for read after the COM port is opened
* then the device may require a restart or it is not
* using sahara protocol
*/
typedef struct{ // 0x01
    sahara_header_t header;
    uint32_t version;
    uint32_t minVersion;
    uint32_t maxCommandPacketSize;
    uint32_t mode;
    uint32_t reserved[6];
} sahara_hello_rx_t;

/**
* sahara_hello_response_t
*
* The initial packet sent to the device
* in response to the initial hello packet
*/
typedef struct { // 0x02
    sahara_header_t header;
    uint32_t version;
    uint32_t minVersion;
    uint32_t status; // ok or error
    uint32_t mode;
    uint32_t reserved[6];
} sahara_hello_tx_t;

/**
* sahara_read_data_rx_t
*
* When the device sends this packet
* it is requesting a file and an initial
* chunk of the file for validation
*/
typedef struct { // 0x03
    sahara_header_t header;
    uint32_t imageId;
    uint32_t offset;
    uint32_t size;
} sahara_read_data_rx_t;

/**
* sahara_transfer_response_rx_t
*
* When an error is encountered or an image transfer has
* ended.
*
* Not sure, but if response if 0 then everything went ok; proceed to next command
*/
typedef struct { // 0x04
    sahara_header_t header;
    uint32_t file;
    uint32_t status;
} sahara_transfer_response_rx_t;

/*
* sahara_done_tx_t
*
* Sent to the device in response to a successful sahara_transfer_response_rx_t
*/
typedef struct { // 0x05
    sahara_header_t header;
} sahara_done_tx_t;

/*
* sahara_done_rx_t
*
* Received from the device after sending sahara_done_tx_t
*/
typedef struct { // 0x06
    sahara_header_t header;
    uint32_t imageTxStatus; // 0 pending, 1 complete
} sahara_done_rx_t;

/*
* sahara_reset_tx_t
*/
typedef struct { // 0x07
    sahara_header_t header;
} sahara_reset_tx_t;

/*
* sahara_reset_rx_t
*/
typedef struct { // 0x08
    sahara_header_t header;
} sahara_reset_rx_t;


typedef struct { // 0x09
    sahara_header_t header;
    uint32_t memoryTableAddress;
	uint32_t memoryTableLength;
} sahara_memory_debug_rx_t;


typedef struct { // 0x0a
  sahara_header_t header;
  uint32_t address;
  uint32_t size;
} sahara_memory_read_tx_t;

/**
* sahara_command_ready_rx_t
*
* Received from the device when in or switching to client
* command mode. ready to take a command (sahara_command_execute_tx_t)
*
*/
typedef struct { // 0x0b
    sahara_header_t header;
    uint32_t imageTxStatus; // 0 pending, 1 complete
} sahara_command_ready_rx_t;

/**
* sahara_command_switch_mode_tx_t
*
* Sent to the device to switch modes when in client command mode.
*
* Device should respond with sahara_hello_rx_t
*/
typedef struct { // 0x0c
    sahara_header_t header;
    uint32_t mode;
} sahara_command_switch_mode_tx_t;

/**
* sahara_command_execute_tx_t
*
* Execute a client command
*
* If the command is invalid you will receive
* sahara_transfer_response_rx_t
*/
typedef struct { // 0x0d
    sahara_header_t header;
    uint32_t command;
} sahara_command_execute_tx_t;


/**
* sahara_command_execute_response_rx_t
*
* Received from the device in response to sahara_command_execute_tx_t
* with the size of the data expected back from the command execution
*
*/
typedef struct { // 0x0e
    sahara_header_t header;
    uint32_t command;
    uint32_t size;
} sahara_command_execute_response_rx_t;

/**
* sahara_command_execute_data_tx_t
*
* Sent in response to sahara_command_execute_response_rx_t
* indicating we are ready to receive the data of n size
* defined in sahara_command_execute_response_rx_t
*
* After sent, read the size of data from the device
*/
typedef struct { // 0x0f
    sahara_header_t header;
    uint32_t command;
} sahara_command_execute_data_tx_t;

/**
 *
 * sahara_memory_debug_64_tx_t
 */
typedef struct { // 0x10
    sahara_header_t header;
    uint64_t address;
    uint64_t size;
} sahara_memory_debug_64_tx_t;

/**
 * sahara_memory_read_64_tx_t
 */
typedef struct { // 0x11
    sahara_header_t header;
    uint64_t address;
    uint64_t size;
} sahara_memory_read_64_tx_t;


PACKED(typedef struct {
	uint32_t unknown1;
	uint32_t address;
	uint32_t size;
	uint8_t name[20];
	uint8_t filename[20];
}) sahara_memory_table_entry_t;


#endif // _QC_SAHARA_H
