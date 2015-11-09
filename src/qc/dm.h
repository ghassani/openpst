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
#define DIAG_NV_ITEM_SIZE   128
#endif

#ifndef DIAG_NV_PEEK_MAX_SIZE
#define DIAG_NV_PEEK_MAX_SIZE   32
#endif

#ifndef DIAG_MAX_PACKET_SIZE
#define DIAG_MAX_PACKET_SIZE (2048 * 2)
#endif

#ifndef DIAG_RX_TIMEOUT
#define DIAG_RX_TIMEOUT (15*1000)
#endif


enum QcdmCommand {
    DIAG_VERNO_F = 0x00, // 0
    DIAG_ESN_F = 0x01, // 1
    DIAG_MEMORY_PEEK_BYTE_F = 0x02, // 2
    DIAG_MEMORY_PEEK_WORD_F = 0x03, // 3
    DIAG_MEMORY_PEEK_DWORD_F = 0x04, // 4
    DIAG_MEMORY_POKE_BYTE_F = 0x05, // 5
    DIAG_MEMORY_POKE_WORD_F = 0x06, // 6
    DIAG_MEMORY_POKE_DWORD_F = 0x07, // 7
    DIAG_OUTPUT_BYTE_F = 0x08, // 8
    DIAG_OUTPUT_WORD_F = 0x09, // 9
    DIAG_INPUT_BYTE_F = 0x0a, // 10
    DIAG_INPUT_WORD_F = 0x0b, // 11
    DIAG_STATUS_F = 0x0c, // 12
    DIAG_LOGMASK_F = 0x0f, // 15
    DIAG_LOG_F = 0x10, // 16
    DIAG_NV_PEEK_F = 0x11, // 17
    DIAG_NV_POKE_F = 0x12, // 18
    DIAG_BAD_CMD_F = 0x13, // 19
    DIAG_BAD_PARM_F = 0x14, // 20
    DIAG_BAD_LEN_F = 0x15, // 21
    DIAG_BAD_MODE_F = 0x18, // 24
    DIAG_TAGRAPH_F = 0x19, // 25
    DIAG_MARKOV_F = 0x1a, // 26
    DIAG_MARKOV_RESET_F = 0x1b, // 27
    DIAG_DIAG_VER_F = 0x1c, // 28
    DIAG_TS_F = 0x1d, // 29
    DIAG_TA_PARM_F = 0x1e, // 30
    DIAG_MSG_F = 0x1f, // 31
    DIAG_HS_KEY_F = 0x20, // 32
    DIAG_HS_LOCK_F = 0x21, // 33
    DIAG_HS_SCREEN_F = 0x22, // 34
    DIAG_PARM_SET_F = 0x24, // 36
    DIAG_NV_READ_F = 0x26, // 38
    DIAG_NV_WRITE_F = 0x27, // 39
    DIAG_CONTROL_F = 0x29, // 41
    DIAG_ERR_READ_F = 0x2a, // 42
    DIAG_ERR_CLEAR_F = 0x2b, // 43
    DIAG_SER_RESET_F = 0x2c, // 44
    DIAG_SER_REPORT_F = 0x2d, // 45
    DIAG_TEST_F = 0x2e, // 46
    DIAG_GET_DIPSW_F = 0x2f, // 47
    DIAG_SET_DIPSW_F = 0x30, // 48
    DIAG_VOC_PCM_LB_F = 0x31, // 49
    DIAG_VOC_PKT_LB_F = 0x32, // 50
    DIAG_ORIG_F = 0x35, // 53
    DIAG_END_F = 0x36, // 54
    DIAG_DLOAD_F = 0x3a, // 58
    DIAG_FTM_CMD_F = 0x3b, // 59
    DIAG_TEST_STATE_F = 0x3d, // 61
    DIAG_STATE_F = 0x3f, // 63
    DIAG_PILOT_SETS_F = 0x40, // 64
    DIAG_SPC_F = 0x41, // 65
    DIAG_BAD_SPC_MODE_F = 0x42, // 66
    DIAG_PARM_GET2_F = 0x43, // 67
    DIAG_SERIAL_CHG_F = 0x44, // 68
    DIAG_PASSWORD_F = 0x46, // 70
    DIAG_BAD_SEC_MODE_F = 0x47, // 71
    DIAG_PR_LIST_WR_F = 0x48, // 72
    DIAG_PR_LIST_RD_F = 0x49, // 73
    DIAG_SUBSYS_CMD_F = 0x4b, // 75
    DIAG_FEATURE_QUERY_F = 0x51, // 81
    DIAG_SMS_READ_F = 0x53, // 83
    DIAG_SMS_WRITE_F = 0x54, // 84
    DIAG_SUP_FER_F = 0x55, // 85
    DIAG_SUP_WALSH_CODES_F = 0x56, // 86
    DIAG_SET_MAX_SUP_CH_F = 0x57, // 87
    DIAG_PARM_GET_IS95B_F = 0x58, // 88
    DIAG_FS_OP_F = 0x59, // 89
    DIAG_AKEY_VERIFY_F = 0x5a, // 90
    DIAG_BMP_HS_SCREEN_F = 0x5b, // 91
    DIAG_CONFIG_COMM_F = 0x5c, // 92
    DIAG_EXT_LOGMASK_F = 0x5d, // 93
    DIAG_EVENT_REPORT_F = 0x60, // 96
    DIAG_STREAMING_CONFIG_F = 0x61, // 97
    DIAG_PARM_RETRIEVE_F = 0x62, // 98
    DIAG_STATUS_SNAPSHOT_F = 0x63, // 99
    DIAG_RPC_F = 0x64, // 100
    DIAG_GET_PROPERTY_F = 0x65, // 101
    DIAG_PUT_PROPERTY_F = 0x66, // 102
    DIAG_GET_GUID_F = 0x67, // 103
    DIAG_USER_CMD_F = 0x68, // 104
    DIAG_GET_PERM_PROPERTY_F = 0x69, // 105
    DIAG_PUT_PERM_PROPERTY_F = 0x6a, // 106
    DIAG_PERM_USER_CMD_F = 0x6b, // 107
    DIAG_GPS_SESS_CTRL_F = 0x6c, // 108
    DIAG_GPS_GRID_F = 0x6d, // 109
    DIAG_GPS_STATISTICS_F = 0x6e, // 110
    DIAG_ROUTE_F = 0x6f, // 111
    DIAG_IS2000_STATUS_F = 0x70, // 112
    DIAG_RLP_STAT_RESET_F = 0x71, // 113
    DIAG_TDSO_STAT_RESET_F = 0x72, // 114
    DIAG_LOG_CONFIG_F = 0x73, // 115
    DIAG_TRACE_EVENT_REPORT_F = 0x74, // 116
    DIAG_SBI_READ_F = 0x75, // 117
    DIAG_SBI_WRITE_F = 0x76, // 118
    DIAG_SSD_VERIFY_F = 0x77, // 119
    DIAG_LOG_ON_DEMAND_F = 0x78, // 120
    DIAG_EXT_MSG_F = 0x79, // 121
    DIAG_ONCRPC_F = 0x7a, // 122
    DIAG_PROTOCOL_LOOPBACK_F = 0x7b, // 123
    DIAG_EXT_BUILD_ID_F = 0x7c, // 124
    DIAG_EXT_MSG_CONFIG_F = 0x7d, // 125
    DIAG_EXT_MSG_TERSE_F = 0x7e, // 126
    DIAG_EXT_MSG_TERSE_XLATE_F = 0x7f, // 127
    DIAG_SUBSYS_CMD_VER_2_F = 0x80, // 128
    DIAG_EVENT_MASK_GET_F = 0x81, // 129
    DIAG_EVENT_MASK_SET_F = 0x82, // 130
    DIAG_CHANGE_PORT_SETTINGS = 0x8c, // 140
    DIAG_CNTRY_INFO_F = 0x8d, // 141
    DIAG_SUPS_REQ_F = 0x8e, // 142
    DIAG_MMS_ORIG_SMS_REQUEST_F = 0x8f, // 143
    DIAG_MEAS_MODE_F = 0x90, // 144
    DIAG_MEAS_REQ_F = 0x91, // 145
    DIAG_MAX_F = 0x92, // 146
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
                                    [0 - 2047]  - HWENGINES
                                    [2048 - 2147]   - MPROC
                                    [2148 - 2247]   - BUSES
                                    [2248 - 2347]   - USB
                                    [2348 - 2447]   - FLASH
                                    [2448 - 3447]   - UART
                                    [3448 - 3547]   - PRODUCTS
                                    [3547 - 65535]  - Reserved
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

enum QcdmPhoneMode : uint8_t {
	kQcdmPhoneModeOfflineA  = 0x00,
	kQcdmPhoneModeOfflineD  = 0x01,
	kQcdmPhoneModeReset		= 0x02,
	kQcdmPhoneModeFtm		= 0x03,
	kQcdmPhoneModeOnline	= 0x04,
	kQcdmPhoneModeLpm		= 0x05,
	kQcdmPhoneModePowerOff  = 0x06
};

enum QcdmRuimConfigType : uint8_t {
	kQcdmRuimConfigTypeRuim  = 0x00,
	kQcdmRuimConfigTypeNv	 = 0x01,
	kQcdmRuimConfigTypePref  = 0x02,
	kQcdmRuimConfigTypeGsm1x = 0x03
};


PACKED(typedef struct QcdmRequestHeader{
    uint8_t command;
}) QcdmRequestHeader;

PACKED(typedef struct QcdmResponseHeader{
	uint8_t command;
}) QcdmResponseHeader;

typedef QcdmRequestHeader QcdmGenericRequest;

PACKED(typedef struct QcdmPhoneModeRequest{
    uint8_t command;
    uint8_t mode;
    uint8_t padding;
}) QcdmPhoneModeRequest;

/**
* Phone Mode
*/
PACKED(typedef struct QcdmPhoneModeResponse{
    uint8_t command;
    uint8_t status;
}) QcdmPhoneModeResponse;

/**
* SPC
*/
PACKED(typedef struct QcdmSpcRequest{
    uint8_t command;
    uint8_t spc[6];
}) QcdmSpcRequest;

PACKED(typedef struct QcdmSpcResponse{
    uint8_t command;
    uint8_t status;
}) QcdmSpcResponse;

/**
* Password
*/
PACKED(typedef struct QcdmPasswordRequest{
    uint8_t command = DIAG_PASSWORD_F;
    uint8_t password[8];
}) QcdmPasswordRequest;

PACKED(typedef struct QcdmPasswordResponse{
    uint8_t command;
    uint8_t status;
}) QcdmPasswordResponse;

/**
* NV
*/
PACKED(typedef struct QcdmNvRequest{
    uint8_t command;
    uint16_t nvItem;
    uint8_t data[DIAG_NV_ITEM_SIZE];
}) QcdmNvRequest;

PACKED(typedef struct QcdmNvResponse {
	uint8_t		command;
    uint16_t	nvItem;
    uint8_t		data[DIAG_NV_ITEM_SIZE];
}) QcdmNvResponse;

PACKED(typedef struct QcdmNvPeekRequest{
	uint8_t command;
	uint32_t address;
	uint8_t size; // max DIAG_NV_PEEK_MAX_SIZE bytes
}) QcdmNvPeekRequest;

PACKED(typedef struct QcdmNvPeekResponse {
	uint8_t		command;
	uint8_t		data[DIAG_NV_PEEK_MAX_SIZE];
}) QcdmNvPeekResponse;

/**
* Subsystem General 
*/
PACKED(typedef struct QcdmSubsysHeader{
    uint8_t command;
    uint8_t  subsysId;
    uint16_t subsysCommand;
}) QcdmSubsysHeader;

PACKED(typedef struct QcdmSubsysRequest{
    QcdmSubsysHeader header;
    uint8_t  data[0];
}) QcdmSubsysRequest;


/**
* Feature Query
*/
PACKED(typedef struct QcdmQueryFeaturesRequest{
    uint8_t command;
    uint16_t featureMaskSize;
    uint8_t  featureMask[0];
}) QcdmQueryFeaturesRequest;

PACKED(typedef struct QcdmLogResponse{
    uint8_t command;
    uint8_t entries;
    uint16_t  length;
    uint8_t  logs[0];
}) QcdmLogResponse;

/**
* Version
*/
PACKED(typedef struct QcdmVersionResponse{
    uint8_t command;
	char  cdate[11];
	char  ctime[8];
	char  rdate[11];
	char  rtime[8];
	char  ver_dir[8];
	uint8_t scm;
	uint8_t caiRev;
	uint8_t model;
	uint8_t firmwareRev;
	uint8_t slotCycleIndex;
	uint8_t hwVersionMajor;
	uint8_t hwVersionMinor;
}) QcdmVersionResponse;

PACKED(typedef struct QcdmDiagVersionResponse{
	uint8_t command;
	uint16_t version;
}) QcdmDiagVersionResponse;

/**
* Status
*/
PACKED(typedef struct QcdmStatusResponse{
	uint8_t command;
	uint8_t demod;
	uint8_t decode;
	uint8_t interleaverId;
	uint32_t esn;
	uint16_t rmMode;
	uint32_t min1[2];
	uint32_t min2[2];
	uint8_t minIndex;
	uint16_t cdmaRxState;
	uint8_t cdmaGoodFrames;
	uint16_t analogCorrectedFrames;
	uint16_t analogBadFrames;
	uint16_t  analogWordSyncs;
	uint16_t entryReason;
	uint16_t currentChannel;
	uint8_t cdmaCodeChan;
	uint16_t pilotBase;
	uint16_t sid;
	uint16_t nid;
	uint16_t locationId;
	uint16_t rssiLevel;
	uint8_t power;
}) QcdmStatusResponse;

/**
* GUID
*/
PACKED(typedef struct QcdmGuidResponse{
	uint8_t command;
	uint32_t guid[4];
}) QcdmGuidResponse;

/**
* Subsys: DIAG_SUBSYS_DIAG_SERV
* QPST Cookie Structures
*/
PACKED(typedef struct QcdmSaveQpstCookieRequest {
    QcdmSubsysHeader header;
    uint8_t version;
    uint8_t cookieLength;
    uint8_t cookie[1];
}) QcdmSaveQpstCookieRequest;

PACKED(typedef struct QcdmSaveQpstCookieResponse {
    QcdmSubsysHeader header;
    uint8_t version;
    uint8_t error;
}) QcdmSaveQpstCookieResponse;

PACKED(typedef struct QcdmQueryQpstCookieRequest {
    QcdmSubsysHeader header;
    uint8_t version;
}) QcdmQueryQpstCookieRequest;

PACKED(typedef struct QcdmQueryQpstCookieResponse {
    QcdmSubsysHeader header;
    uint8_t version;
    uint8_t error;
    uint8_t cookieLength;
    uint8_t cookie[1];
}) QcdmQueryQpstCookieResponse;






PACKED(typedef struct QcdmTestStruct {
    QcdmSubsysHeader header;
    uint8_t processorId;
}) QcdmTestStruct;

#endif // _QC_DIAG_H
