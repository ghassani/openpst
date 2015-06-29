/**
* LICENSE PLACEHOLDER
*
* @file diag.h
* @package OpenPST
* @brief QCDM definitions and structures
*
* @author Gassan Idriss <ghassani@gmail.com>
* @author Matteson Raab <mraabhimself@gmail.com>
*/

#ifndef _QC_DIAG_H
#define _QC_DIAG_H

#include "include/definitions.h"

#ifndef DIAG_NV_ITEM_SIZE
#define	DIAG_NV_ITEM_SIZE	128
#endif

#ifndef DIAG_MAX_RX_PKT_SIZ
#define DIAG_MAX_RX_PKT_SIZ (2048 * 2)
#endif

#ifndef DIAG_MAX_TX_PKT_SIZ
#define DIAG_MAX_TX_PKT_SIZ (2048 * 2)
#endif

#ifndef DIAG_RX_TIMEOUT
#define DIAG_RX_TIMEOUT (15*1000)
#endif

enum DIAG_PASSWORD_STATUS {
	DIAG_PASSWORD_REJECT = 0x0,
	DIAG_PASSWORD_ACCEPT = 0x1
};

enum DIAG_SPC_STATUS {
	DIAG_SPC_REJECT = 0x0,
	DIAG_SPC_ACCEPT = 0x1
};

enum DIAG_CMD_STATUS {
	DIAG_CMD_GENERAL_FAIL = -1,
	DIAG_CMD_PORT_CLOSED = -2,
	DIAG_CMD_RX_FAIL = -3,
	DIAG_CMD_TX_FAIL = -4
};

enum DIAG_COMMAND {
	DIAG_BAD_CMD_F = 0x13,
	DIAG_BAD_LEN_F = 0x15,
	DIAG_BAD_MODE_F = 0x18,
	DIAG_BAD_PARM_F = 20,
	DIAG_CONTROL_F = 0x29,
	DIAG_DLOAD_F = 0x3a,
	DIAG_ERR_CLEAR_F = 0x2b,
	DIAG_ERR_READ_F = 0x2a,
	DIAG_ESN_F = 1,
	DIAG_EVENT_MASK_GET_F = 0x81,
	DIAG_EVENT_MASK_SET_F = 130,
	DIAG_EVENT_REPORT_F = 0x60,
	DIAG_EXT_BUILD_ID_F = 0x7c,
	DIAG_EXT_LOGMASK_F = 0x5d,
	DIAG_EXT_MSG_CONFIG_F = 0x7d,
	DIAG_EXT_MSG_F = 0x79,
	DIAG_GET_DIPSW_F = 0x2f,
	DIAG_GPS_CMD_F = 0x6c,
	DIAG_HS_KEY_F = 0x20,
	DIAG_LOG_CONFIG_F = 0x73,
	DIAG_LOG_F = 0x10,
	DIAG_MAX_F = 0x7e,
	DIAG_MEMORY_PEEK_BYTE_F = 2,
	DIAG_MEMORY_PEEK_DWORD_F = 4,
	DIAG_MEMORY_PEEK_WORD_F = 3,
	DIAG_MEMORY_POKE_BYTE_F = 5,
	DIAG_MEMORY_POKE_DWORD_F = 7,
	DIAG_MEMORY_POKE_WORD_F = 6,
	DIAG_MSG_F = 0x1f,
	DIAG_NV_READ_F = 0x26,
	DIAG_NV_WRITE_F = 0x27,
	DIAG_NV_WRITE_ONLINE_F = 0x4c,
	DIAG_PROTOCOL_LOOPBACK_F = 0x7b,
	DIAG_SECURITY_FREEZE_F = 0xff,
	DIAG_SERIAL_MODE_CHANGE = 0x44,
	DIAG_SET_DIPSW_F = 0x30,
	DIAG_SPC_F = 0x41,
	DIAG_PASSWORD_F = 0x46,
	DIAG_STATUS_F = 14,
	DIAG_STATUS_REQUEST_F = 12,
	DIAG_SUBSYS_CMD_F = 0x4b,
	DIAG_SUBSYS_CMD_VER_2_F = 0x80,
	DIAG_VERNO_F = 0,
	DIAG_VOC_PCM_LB_F = 0x31,
    DIAG_VOC_PKT_LB_F = 50
};

enum DIAG_PHONE_MODE {
    MODE_OFFLINE_A_F,
	MODE_OFFLINE_D_F,
	MODE_RESET_F,
	MODE_FTM_F,
	MODE_ONLINE_F,
	MODE_LPM_F,
	MODE_POWER_OFF_F,
	MODE_MAX_F
};

enum DIAG_RTRE_MODE {
	RTRE_MODE_RUIM_ONLY = 0x0,
	RTRE_MODE_NV_ONLY = 0x1,
	RTRE_MODE_RUIM_PREF = 0x2,
	RTRE_MODE_GSM_1X = 0x3
};



typedef struct qcdm_spc_tx_t{
    uint8_t command;
    uint8_t spc[6];
} qcdm_spc_tx_t;

typedef struct qcdm_spc_rx_t{
    uint8_t command;
    uint8_t status;
} qcdm_spc_rx_t;

typedef struct qcdm_16pw_tx_t{
	uint8_t command;
	uint8_t password[16];
} qcdm_16pw_tx_t;

typedef struct qcdm_16pw_rx_t{
	uint8_t command;
	uint8_t status;
} qcdm_16pw_rx_t;

PACKED(typedef struct {
	uint8_t cmd;
	uint16_t nvItem;
	uint8_t data[DIAG_NV_ITEM_SIZE];
} qcdm_nv_tx_t);

PACKED(typedef struct {
    uint8_t cmd;
    uint16_t nvItem;
    uint8_t data[DIAG_NV_ITEM_SIZE];
} qcdm_nv_rx_t);

#endif // _QC_DIAG_H
