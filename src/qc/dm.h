/**
* LICENSE PLACEHOLDER
*
* @file dm.h
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

#ifndef DIAG_MAX_PACKET_SIZE
#define DIAG_MAX_PACKET_SIZE (2048 * 2)
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


enum DIAG_SUBSYS_CMD {
	DIAG_SUBSYS_OEM = 0,       /* Reserved for OEM use */
	DIAG_SUBSYS_ZREX = 1,       /* ZREX */
	DIAG_SUBSYS_SD = 2,       /* System Determination */
	DIAG_SUBSYS_BT = 3,       /* Bluetooth */
	DIAG_SUBSYS_WCDMA = 4,       /* WCDMA */
	DIAG_SUBSYS_HDR = 5,       /* 1xEvDO */
	DIAG_SUBSYS_DIABLO = 6,       /* DIABLO */
	DIAG_SUBSYS_TREX = 7,       /* TREX - Off-target testing environments */
	DIAG_SUBSYS_GSM = 8,       /* GSM */
	DIAG_SUBSYS_UMTS = 9,       /* UMTS */
	DIAG_SUBSYS_HWTC = 10,      /* HWTC */
	DIAG_SUBSYS_FTM = 11,      /* Factory Test Mode */
	DIAG_SUBSYS_REX = 12,      /* Rex */
	DIAG_SUBSYS_OS = DIAG_SUBSYS_REX,
	DIAG_SUBSYS_GPS = 13,      /* Global Positioning System */
	DIAG_SUBSYS_WMS = 14,      /* Wireless Messaging Service (WMS, SMS) */
	DIAG_SUBSYS_CM = 15,      /* Call Manager */
	DIAG_SUBSYS_HS = 16,      /* Handset */
	DIAG_SUBSYS_AUDIO_SETTINGS = 17,      /* Audio Settings */
	DIAG_SUBSYS_DIAG_SERV = 18,      /* DIAG Services */
	DIAG_SUBSYS_FS = 19,      /* File System - EFS2 */
	DIAG_SUBSYS_PORT_MAP_SETTINGS = 20,      /* Port Map Settings */
	DIAG_SUBSYS_MEDIAPLAYER = 21,      /* QCT Mediaplayer */
	DIAG_SUBSYS_QCAMERA = 22,      /* QCT QCamera */
	DIAG_SUBSYS_MOBIMON = 23,      /* QCT MobiMon */
	DIAG_SUBSYS_GUNIMON = 24,      /* QCT GuniMon */
	DIAG_SUBSYS_LSM = 25,      /* Location Services Manager */
	DIAG_SUBSYS_QCAMCORDER = 26,      /* QCT QCamcorder */
	DIAG_SUBSYS_MUX1X = 27,      /* Multiplexer */
	DIAG_SUBSYS_DATA1X = 28,      /* Data */
	DIAG_SUBSYS_SRCH1X = 29,      /* Searcher */
	DIAG_SUBSYS_CALLP1X = 30,      /* Call Processor */
	DIAG_SUBSYS_APPS = 31,      /* Applications */
	DIAG_SUBSYS_SETTINGS = 32,      /* Settings */
	DIAG_SUBSYS_GSDI = 33,      /* Generic SIM Driver Interface */
	DIAG_SUBSYS_UIMDIAG = DIAG_SUBSYS_GSDI,
	DIAG_SUBSYS_TMC = 34,      /* Task Main Controller */
	DIAG_SUBSYS_USB = 35,      /* Universal Serial Bus */
	DIAG_SUBSYS_PM = 36,      /* Power Management */
	DIAG_SUBSYS_DEBUG = 37,
	DIAG_SUBSYS_QTV = 38,
	DIAG_SUBSYS_CLKRGM = 39,      /* Clock Regime */
	DIAG_SUBSYS_DEVICES = 40,
	DIAG_SUBSYS_WLAN = 41,      /* 802.11 Technology */
	DIAG_SUBSYS_PS_DATA_LOGGING = 42,      /* Data Path Logging */
	DIAG_SUBSYS_PS = DIAG_SUBSYS_PS_DATA_LOGGING,
	DIAG_SUBSYS_MFLO = 43,      /* MediaFLO */
	DIAG_SUBSYS_DTV = 44,      /* Digital TV */
	DIAG_SUBSYS_RRC = 45,      /* WCDMA Radio Resource Control state */
	DIAG_SUBSYS_PROF = 46,      /* Miscellaneous Profiling Related */
	DIAG_SUBSYS_TCXOMGR = 47,
	DIAG_SUBSYS_NV = 48,      /* Non Volatile Memory */
	DIAG_SUBSYS_AUTOCONFIG = 49,
	DIAG_SUBSYS_PARAMS = 50,      /* Parameters required for debugging subsystems */
	DIAG_SUBSYS_MDDI = 51,      /* Mobile Display Digital Interface */
	DIAG_SUBSYS_DS_ATCOP = 52,
	DIAG_SUBSYS_L4LINUX = 53,      /* L4/Linux */
	DIAG_SUBSYS_MVS = 54,      /* Multimode Voice Services */
	DIAG_SUBSYS_CNV = 55,      /* Compact NV */
	DIAG_SUBSYS_APIONE_PROGRAM = 56,      /* apiOne */
	DIAG_SUBSYS_HIT = 57,      /* Hardware Integration Test */
	DIAG_SUBSYS_DRM = 58,      /* Digital Rights Management */
	DIAG_SUBSYS_DM = 59,      /* Device Management */
	DIAG_SUBSYS_FC = 60,      /* Flow Controller */
	DIAG_SUBSYS_MEMORY = 61,      /* Malloc Manager */
	DIAG_SUBSYS_FS_ALTERNATE = 62,      /* Alternate File System */
	DIAG_SUBSYS_REGRESSION = 63,      /* Regression Test Commands */
	DIAG_SUBSYS_SENSORS = 64,      /* The sensors subsystem */
	DIAG_SUBSYS_FLUTE = 65,      /* FLUTE */
	DIAG_SUBSYS_ANALOG = 66,      /* Analog die subsystem */
	DIAG_SUBSYS_APIONE_PROGRAM_MODEM = 67,    /* apiOne Program On Modem Processor */
	DIAG_SUBSYS_LTE = 68,      /* LTE */
	DIAG_SUBSYS_BREW = 69,      /* BREW */
	DIAG_SUBSYS_PWRDB = 70,      /* Power Debug Tool */
	DIAG_SUBSYS_CHORD = 71,      /* Chaos Coordinator */
	DIAG_SUBSYS_SEC = 72,      /* Security */
	DIAG_SUBSYS_TIME = 73,      /* Time Services */
	DIAG_SUBSYS_Q6_CORE = 74,      /* Q6 core services */
	DIAG_SUBSYS_COREBSP = 75,      /* CoreBSP */
									/* Command code allocation:
									[0 - 2047]	- HWENGINES
									[2048 - 2147]	- MPROC
									[2148 - 2247]	- BUSES
									[2248 - 2347]	- USB
									[2348 - 2447]   - FLASH
									[2448 - 3447]   - UART
									[3448 - 3547]   - PRODUCTS
									[3547 - 65535]	- Reserved
									*/

	DIAG_SUBSYS_MFLO2 = 76,      /* Media Flow */
								/* Command code allocation:
								[0 - 1023]       - APPs
								[1024 - 65535]   - Reserved
								*/
	DIAG_SUBSYS_ULOG = 77,  /* ULog Services */
	DIAG_SUBSYS_APR = 78,  /* Asynchronous Packet Router (Yu, Andy)*/
	DIAG_SUBSYS_QNP = 79, /*QNP (Ravinder Are , Arun Harnoor)*/
	DIAG_SUBSYS_STRIDE = 80, /* Ivailo Petrov */
	DIAG_SUBSYS_OEMDPP = 81, /* to read/write calibration to DPP partition */
	DIAG_SUBSYS_Q5_CORE = 82, /* Requested by ADSP team */
	DIAG_SUBSYS_USCRIPT = 83, /* core/power team USCRIPT tool */
	DIAG_SUBSYS_NAS = 84, /* Requested by 3GPP NAS team */
	DIAG_SUBSYS_CMAPI = 85, /* Requested by CMAPI */
	DIAG_SUBSYS_SSM = 86,
	DIAG_SUBSYS_TDSCDMA = 87,  /* Requested by TDSCDMA team */
	DIAG_SUBSYS_SSM_TEST = 88,
	DIAG_SUBSYS_MPOWER = 89,  /* Requested by MPOWER team */
	DIAG_SUBSYS_QDSS = 90,  /* For QDSS STM commands */
	DIAG_SUBSYS_CXM = 91,
	DIAG_SUBSYS_GNSS_SOC = 92,  /* Secondary GNSS system */
	DIAG_SUBSYS_TTLITE = 93,
	DIAG_SUBSYS_FTM_ANT = 94,
	DIAG_SUBSYS_MLOG = 95,
	DIAG_SUBSYS_LIMITSMGR = 96,
	DIAG_SUBSYS_EFSMONITOR = 97,
	DIAG_SUBSYS_DISPLAY_CALIBRATION = 98,
	DIAG_SUBSYS_VERSION_REPORT = 99,
	DIAG_SUBSYS_DS_IPA = 100,
	DIAG_SUBSYS_SYSTEM_OPERATIONS = 101,
	DIAG_SUBSYS_CNSS_POWER = 102,
	DIAG_SUBSYS_LAST,

	/* Subsystem IDs reserved for OEM use */
	DIAG_SUBSYS_RESERVED_OEM_0 = 250,
	DIAG_SUBSYS_RESERVED_OEM_1 = 251,
	DIAG_SUBSYS_RESERVED_OEM_2 = 252,
	DIAG_SUBSYS_RESERVED_OEM_3 = 253,
	DIAG_SUBSYS_RESERVED_OEM_4 = 254,
	DIAG_SUBSYS_LEGACY = 255
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


PACKED(typedef struct qcdm_phone_mode_tx_t{
	uint8_t command = DIAG_CONTROL_F;
	uint8_t mode;
	uint8_t padding = 0x0;
}) qcdm_phone_mode_tx_t;

PACKED(typedef struct qcdm_phone_mode_rx_t{
	uint8_t command;
	uint8_t status;
}) qcdm_phone_mode_rx_t;

PACKED(typedef struct qcdm_spc_tx_t{
	uint8_t command = DIAG_SPC_F;
	uint8_t spc[6];
}) qcdm_spc_tx_t;

PACKED(typedef struct qcdm_spc_rx_t{
	uint8_t command;
	uint8_t status;
}) qcdm_spc_rx_t;

PACKED(typedef struct qcdm_16pw_tx_t{
	uint8_t command = DIAG_PASSWORD_F;
	uint8_t password[8];
}) qcdm_16pw_tx_t;

PACKED(typedef struct qcdm_16pw_rx_t{
	uint8_t command;
	uint8_t status;
}) qcdm_16pw_rx_t;

PACKED(typedef struct qcdm_nv_tx_t{
	uint8_t cmd;
	uint16_t nvItem;
	uint8_t data[DIAG_NV_ITEM_SIZE] = {};
}) qcdm_nv_tx_t;

PACKED(typedef struct qcdm_nv_alt_tx_t{
	uint8_t padding = 0x0;
	uint8_t data[DIAG_NV_ITEM_SIZE - 1] = {};
}) qcdm_nv_alt_tx_t;

PACKED(typedef struct qcdm_nv_alt2_tx_t{
	uint16_t padding = 0x0;
	uint8_t data[DIAG_NV_ITEM_SIZE - 2] = {};
}) qcdm_nv_alt2_tx_t;

PACKED(typedef struct qcdm_nv_raw_tx_t{
	uint8_t data[DIAG_NV_ITEM_SIZE - 3] = {};
}) qcdm_nv_raw_tx_t;

PACKED(typedef struct qcdm_nv_rx_t{
	uint8_t cmd;
	uint16_t nvItem;
	uint8_t data[DIAG_NV_ITEM_SIZE];
}) qcdm_nv_rx_t;

PACKED(typedef struct qcdm_nv_alt_rx_t{
	uint8_t cmd;
	uint16_t nvItem;
	uint8_t padding;
	uint8_t data[DIAG_NV_ITEM_SIZE - 1];
}) qcdm_nv_alt_rx_t;

PACKED(typedef struct qcdm_nv_alt2_rx_t{
	uint8_t cmd;
	uint16_t nvItem;
	uint8_t padding[2];
	uint8_t data[DIAG_NV_ITEM_SIZE - 2];
}) qcdm_nv_alt2_rx_t;


PACKED(typedef struct qcdm_subsys_header_t{
	uint8_t command;
	uint8_t  subsysId;
	uint16_t subsysCommand;
}) qcdm_subsys_header_t;

PACKED(typedef struct qcdm_subsys_header_v2_t{
	uint16_t command;
	uint16_t subsysId;
	uint16_t subsysCommand;
}) qcdm_subsys_header_v2_t;

PACKED(typedef struct qcdm_subsys_packet_t{
	qcdm_subsys_header_t header;
	uint8_t  data[0];
}) qcdm_subsys_packet_t;

#endif // _QC_DIAG_H
