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
    DIAG_VERNO_F = 0x0,
    DIAG_ESN_F = 0x1,
    DIAG_MEMORY_PEEK_BYTE_F = 0x2,
    DIAG_MEMORY_PEEK_WORD_F = 0x3,
    DIAG_MEMORY_PEEK_DWORD_F = 0x4,
    DIAG_MEMORY_POKE_BYTE_F = 0x5,
    DIAG_MEMORY_POKE_WORD_F = 0x6,
    DIAG_MEMORY_POKE_DWORD_F = 0x7,
    DIAG_OUTPUT_BYTE_F = 0x8,
    DIAG_OUTPUT_WORD_F = 0x9,
    DIAG_INPUT_BYTE_F = 0xA,
    DIAG_INPUT_WORD_F = 0xB,
    DIAG_STATUS_REQUEST_F = 0xC,
    DIAG_STATUS_F = 0xE,
    DIAG_LOGMASK_F = 0xF,
    DIAG_LOG_F = 0x10,
    DIAG_NV_PEEK_F = 0x11,
    DIAG_NV_POKE_F = 0x12,
    DIAG_BAD_CMD_F = 0x13,
    DIAG_BAD_PARM_F = 0x14,
    DIAG_BAD_LEN_F = 0x15,
    DIAG_BAD_DEV_F = 0x16,
    DIAG_BAD_MODE_F = 0x18,
    DIAG_TAGRAPH_F = 0x19,
    DIAG_MARKOV_F = 0x1A,
    DIAG_MARKOV_RESET_F = 0x1B,
    DIAG_VER_F = 0x1C,
    DIAG_TS_F = 0x1D,
    DIAG_TA_PARM_F = 0x1E,
    DIAG_MSG_F = 0x1F,
    DIAG_HS_KEY_F = 0x20,
    DIAG_HS_LOCK_F = 0x21,
    DIAG_HS_SCREEN_F = 0x22,
    DIAG_PARM_SET_F = 0x24,
    DIAG_NV_READ_F = 0x26,
    DIAG_NV_WRITE_F = 0x27,
    DIAG_CONTROL_F = 0x29,
    DIAG_ERR_READ_F = 0x2A,
    DIAG_ERR_CLEAR_F = 0x2B,
    DIAG_SER_RESET_F = 0x2C,
    DIAG_SER_REPORT_F = 0x2D,
    DIAG_TEST_F = 0x2E,
    DIAG_GET_DIPSW_F = 0x2F,
    DIAG_SET_DIPSW_F = 0x30,
    DIAG_VOC_PCM_LB_F = 0x31,
    DIAG_VOC_PKT_LB_F = 0x32,
    DIAG_ORIG_CALL_F = 0x35,
    DIAG_END_CALL_F = 0x36,
    DIAG_SW_VER_F = 0x38,
    DIAG_DLOAD_F = 0x3A,
    DIAG_TMOB_F = 0x3B,
    DIAG_FTM_CMD_F = 0x3B,
    DIAG_TEST_STATE_F = 0x3D,
    DIAG_STATE_F = 0x3F,
    DIAG_PILOT_SETS_F = 0x40,
    DIAG_SPC_F = 0x41,
    DIAG_BAD_SPC_MODE_F = 0x42,
    DIAG_PARM_GET2_F = 0x43,
    DIAG_SERIAL_MODE_CHANGE_F = 0x44,
    DIAG_PASSWORD_F = 0x46,
    DIAG_BAD_SEC_MODE_F = 0x47,
    DIAG_PR_LIST_WR_F = 0x48,
    DIAG_PR_LIST_RD_F = 0x49,
    DIAG_SUBSYS_CMD_F = 0x4B,
    DIAG_NV_WRITE_ONLINE_F = 0x4C,
    DIAG_FEATURE_QUERY_F = 0x51,
    DIAG_SMS_READ_F = 0x53,
    DIAG_SMS_WRITE_F = 0x54,
    DIAG_SUP_FER_F = 0x55,
    DIAG_SUP_WALSH_CODES_F = 0x56,
    DIAG_SET_MAX_SUP_CH_F = 0x57,
    DIAG_PARM_GET_IS95B_F = 0x58,
    DIAG_FS_OP_F = 0x59,
    DIAG_AKEY_VERIFY_F = 0x5A,
    DIAG_BMP_HS_SCREEN_F = 0x5B,
    DIAG_CONFIG_COMM_F = 0x5C,
    DIAG_EXT_LOGMASK_F = 0x5D,
    DIAG_EVENT_REPORT_F = 0x60,
    DIAG_STREAMING_CONFIG_F = 0x61,
    DIAG_PARM_RETRIEVE_F = 0x62,
    DIAG_STATUS_SNAPSHOT_F = 0x63,
    DIAG_RPC_F = 0x64,
    DIAG_GET_PROPERTY_F = 0x65,
    DIAG_PUT_PROPERTY_F = 0x66,
    DIAG_GET_GUID_F = 0x67,
    DIAG_USER_CMD_F = 0x68,
    DIAG_GET_PERM_PROPERTY_F = 0x69,
    DIAG_PUT_PERM_PROPERTY_F = 0x6A,
    DIAG_PERM_USER_CMD_F = 0x6B,
    DIAG_GPS_SESS_CTRL_F = 0x6C,
    DIAG_GPS_GRID_F = 0x6D,
    DIAG_GPS_STATISTICS_F = 0x6E,
    DIAG_ROUTE_F = 0x6F,
    DIAG_IS2000_STATUS_F = 0x70,
    DIAG_RLP_STAT_RESET_F = 0x71,
    DIAG_TDSO_STAT_RESET_F = 0x72,
    DIAG_LOG_CONFIG_F = 0x73,
    DIAG_TRACE_EVENT_REPORT_F = 0x74,
    DIAG_SBI_READ_F = 0x75,
    DIAG_SBI_WRITE_F = 0x76,
    DIAG_SSD_VERIFY_F = 0x77,
    DIAG_LOG_ON_DEMAND_F = 0x78,
    DIAG_EXT_MSG_F = 0x79,
    DIAG_ONCRPC_F = 0x7A,
    DIAG_PROTOCOL_LOOPBACK_F = 0x7B,
    DIAG_EXT_BUILD_ID_F = 0x7C,
    DIAG_EXT_MSG_CONFIG_F = 0x7D,
    DIAG_EXT_MSG_TERSE_F = 0x7E,
    DIAG_SUBSYS_CMD_VER_2_F = 0x80,
    DIAG_EVENT_MASK_GET_F = 0x81,
    DIAG_EVENT_MASK_SET_F = 0x82,
    DIAG_CHANGE_PORT_SETTINGS = 0x8C,
    DIAG_CNTRY_INFO_F = 0x8D,
    DIAG_SUPS_REQ_F = 0x8E,
    DIAG_MMS_ORIG_SMS_REQUEST_F = 0x8F,
    DIAG_MEAS_MODE_F = 0x90,
    DIAG_MEAS_REQ_F = 0x91,
    DIAG_QSR_EXT_MSG_TERSE_F = 0x92,
    DIAG_DCI_CMD_REQ = 0x93,
    DIAG_DCI_DELAYED_RSP = 0x94,
    DIAG_BAD_TRANS_F = 0x95,
    DIAG_SSM_DISALLOWED_CMD_F = 0x96,
    DIAG_LOG_ON_DEMAND_EXT_F = 0x97,
    DIAG_CMD_EXT_F = 0x98,
    DIAG_QSR4_EXT_MSG_TERSE_F = 0x99,
    DIAG_SECURITY_FREEZE_F = 0xFF
};

enum DIAG_EFS_SUBSYS_CMD {
    EFS_HDR_F = 0x5, // From CDMAWSTOOL
    EFS_GPS_F = 0xD, // From CDMAWSTOOL
    EFS_SMS_F = 0xE, // From CDMAWSTOOL
    EFS_CM_F = 0xF, // From CDMAWSTOOL
    EFS_STORAGE_F = 0x13, // From CDMAWSTOOL
    EFS_OLD_CONTROL_F = 0x32, // From CDMAWSTOOL
    EFS_CONTROL_F = 0xFA // From CDMAWSTOOL
};

enum DIAG_EFS_STORAGE_CMD {
    EFS2_STORAGE_OPEN_F = 0x2, // From CDMAWSTOOL
    EFS2_STORAGE_CLOSE_F = 0x3, // From CDMAWSTOOL
    EFS2_STORAGE_READ_F = 0x4, // From CDMAWSTOOL
    EFS2_STORAGE_WRITE_F = 0x5, // From CDMAWSTOOL
    EFS2_STORAGE_UNLINK_F = 0x8, // From CDMAWSTOOL
    EFS2_STORAGE_OPENDIR_F = 0xB, // From CDMAWSTOOL
    EFS2_STORAGE_READDIR_F = 0xC, // From CDMAWSTOOL
    EFS2_STORAGE_CLOSEDIR_F = 0xD, // From CDMAWSTOOL
};

enum DIAG_PHONE_MODE {
    MODE_OFFLINE_A_F  = 0x0, // Guessing
    MODE_OFFLINE_D_F = 0x1, // From CDMAWSTOOL
    MODE_RESET_F = 0x2, // From CDMAWSTOOL
    MODE_FTM_F = 0x3, // Guessing
    MODE_ONLINE_F = 0x4, // From CDMAWSTOOL
    MODE_LPM_F = 0x5, // From CDMAWSTOOL
    MODE_POWER_OFF_F =0x6, // Guessing
    MODE_MAX_F = 0x7 // Guessing
};

enum DIAG_RTRE_MODE {
    RTRE_MODE_RUIM_ONLY = 0x0,
    RTRE_MODE_NV_ONLY = 0x1,
    RTRE_MODE_RUIM_PREF = 0x2,
    RTRE_MODE_GSM_1X = 0x3
};

enum DIAG_PREF_MODE {
    PREF_MODE_AMPS = 3,
    PREF_MODE_AMPS_CDMA = 2,
    PREF_MODE_AUTOMATIC = 4,
    PREF_MODE_CDMA_AMPS = 0,
    PREF_MODE_CDMA_AMPS_HDR = 20,
    PREF_MODE_CDMA_AMPS_HDR_WLAN = 0x1b,
    PREF_MODE_CDMA_AMPS_WLAN = 0x1c,
    PREF_MODE_CDMA_GSM_WCDMA = 0x33,
    PREF_MODE_CDMA_HDR = 0x13,
    PREF_MODE_CDMA_HDR_GSM_WCDMA = 0x34,
    PREF_MODE_CDMA_HDR_WLAN = 0x17,
    PREF_MODE_CDMA_IS2000 = 9,
    PREF_MODE_CDMA_IS95 = 1,
    PREF_MODE_CDMA_WLAN = 0x15,
    PREF_MODE_CDMAxAMPS = 11,
    PREF_MODE_DIGITAL = 0x12,
    PREF_MODE_EMERGENCY = 5,
    PREF_MODE_GPS = 12,
    PREF_MODE_GSM_GPRS_EDGE = 13,
    PREF_MODE_GSM_WCDMA = 0x11,
    PREF_MODE_GSM_WCDMA_WLAN = 0x1a,
    PREF_MODE_GSM_WLAN = 0x18,
    PREF_MODE_HDR = 10,
    PREF_MODE_HDR_OFF = 0x10,
    PREF_MODE_HDR_WLAN = 0x16,
    PREF_MODE_HDR_WLAN_OFF = 0x1d,
    PREF_MODE_HOME = 6,
    PREF_MODE_LTE = 30,
    PREF_MODE_LTE_CDMA = 0x20,
    PREF_MODE_LTE_CDMA_GSM = 0x25,
    PREF_MODE_LTE_CDMA_GSM_WLAN = 0x2e,
    PREF_MODE_LTE_CDMA_HDR = 0x24,
    PREF_MODE_LTE_CDMA_HDR_WLAN = 0x2d,
    PREF_MODE_LTE_CDMA_WCDMA = 0x26,
    PREF_MODE_LTE_CDMA_WCDMA_WLAN = 0x2f,
    PREF_MODE_LTE_CDMA_WLAN = 0x29,
    PREF_MODE_LTE_GSM = 0x22,
    PREF_MODE_LTE_GSM_WCDMA = 0x1f,
    PREF_MODE_LTE_GSM_WLAN = 0x2b,
    PREF_MODE_LTE_HDR = 0x21,
    PREF_MODE_LTE_HDR_GSM = 0x27,
    PREF_MODE_LTE_HDR_GSM_WLAN = 0x30,
    PREF_MODE_LTE_HDR_WCDMA = 40,
    PREF_MODE_LTE_HDR_WCDMA_WLAN = 0x31,
    PREF_MODE_LTE_HDR_WLAN = 0x2a,
    PREF_MODE_LTE_WCDMA = 0x23,
    PREF_MODE_LTE_WCDMA_WLAN = 0x2c,
    PREF_MODE_WCDMA_HSDPA = 14,
    PREF_MODE_WCDMA_WLAN = 0x19,
    PREF_MODE_WLAN = 15,
    PREF_MODE_WLAN_OFF = 50
};

enum DIAG_PREF_SERV {
    PREF_SERV_SYSTEM_A = 0x0,
    PREF_SERV_SYSTEM_AB = 0x1,
    PREF_SERV_SYSTEM_B = 0x2,
    PREF_SERV_SYSTEM_BA = 0x3,
    PREF_SERV_HOME_ONLY = 0x4,
    PREF_SERV_HOME_PREF = 0x5
};

enum DIAG_SYSTEM_PREF {
    SYSTEM_PREF_SYSTEM_A = 0x0,
    SYSTEM_PREF_SYSTEM_B = 0x1,
    SYSTEM_PREF_HOME_ONLY = 0x2,
    SYSTEM_PREF_HOME_PREF = 0x3
};

enum DIAG_ROAM_PREF {
    ROAM_PREF_HOME = 0x1,
    ROAM_PREF_AFFILIATED = 0x3,
    ROAM_PREF_AUTOMATIC = 0xff,
    ROAM_PREF_STATIC = 0x100,
};

enum DIAG_HYBRID_PREF {
    HYBRID_PREF_DISABLE = 0x0,
    HYBRID_PREF_ENABLE = 0x1,
    HYBRID_PREF_FACTORY = 0x2
};

enum DIAG_HDR_SCP_FORCE_AT_CONFIG {
    AT_CONFIG_REV_0_ONLY = 0x0,
    AT_CONFIG_REV_A_MFPA = 0x1,
    AT_CONFIG_REV_A_MFPA_EMPA = 0x2,
    AT_CONFIG_REV_B_MFPA = 0x3,
    AT_CONFIG_REV_A_EHRPD = 0x4,
    AT_CONFIG_REV_B_EHRPD = 0x5
};

enum DIAG_HDR_SCP_FORCE_REL0_CONFIG {
    REL0_CONFIG_REV_A = 0x0,
    REL0_CONFIG_REV_0 = 0x1
};

enum DIAG_DS_MIP_RETRIES {
    DS_MIP_RETRIES_1000MS = 0x0,
    DS_MIP_RETRIES_1250MS = 0x1,
    DS_MIP_RETRIES_1500MS = 0x2,
    DS_MIP_RETRIES_1750MS = 0x3,
    DS_MIP_RETRIES_2000MS = 0x4
};

enum DIAG_DS_QCMIP {
    DS_QCMIP_SIMPLE_IP = 0x0,
    DS_QCMIP_PREF_MOBILE_IP = 0x1,
    DS_QCMIP_MOBILE_IP = 0x2
};

PACKED(typedef struct qcdm_phone_mode_tx_t{
    uint8_t command;
    uint8_t mode;
    uint8_t padding;
}) qcdm_phone_mode_tx_t;

PACKED(typedef struct qcdm_phone_mode_rx_t{
    uint8_t command;
    uint8_t status;
}) qcdm_phone_mode_rx_t;

PACKED(typedef struct qcdm_spc_tx_t{
    uint8_t command;
    uint8_t spc[6];
}) qcdm_spc_tx_t;

PACKED(typedef struct qcdm_spc_rx_t{
    uint8_t command;
    uint8_t status;
}) qcdm_spc_rx_t;

PACKED(typedef struct{
    uint8_t command;
    uint8_t password[8];
}) qcdm_16pw_tx_t;

PACKED(typedef struct {
    uint8_t command;
    uint8_t status;
}) qcdm_16pw_rx_t;

PACKED(typedef struct {
    uint8_t cmd;
    uint16_t nvItem;
    uint8_t data[DIAG_NV_ITEM_SIZE];
}) qcdm_nv_tx_t;

PACKED(typedef struct {
    uint8_t cmd;
    uint16_t nvItem;
    uint8_t data[DIAG_NV_ITEM_SIZE];
}) qcdm_nv_rx_t;
PACKED(typedef struct {
    uint8_t cmd;
    uint16_t nvItem;
    uint8_t padding;
    uint8_t data[DIAG_NV_ITEM_SIZE];
}) qcdm_nv_alt_rx_t;
PACKED(typedef struct {
    uint8_t cmd;
    uint16_t nvItem;
    uint8_t padding;
    uint8_t firstByte;
    uint8_t data[DIAG_NV_ITEM_SIZE];
}) qcdm_nv_alt2_rx_t;

#endif // _QC_DIAG_H
