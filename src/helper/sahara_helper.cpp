#include "sahara_helper.h"
#include "qc_sahara.h"

namespace SaharaHelper
{
    /**
     * @brief SaharaHelper::getNamedMode
     * @param mode
     * @return
     */
    const char* getNamedMode(uint32_t mode)
    {
        switch(mode) {
            case SAHARA_MODE_IMAGE_TX_PENDING:
                return "Image Transfer Pending";
                break;
            case SAHARA_MODE_IMAGE_TX_COMPLETE:
                return "Image Transfer Complete";
                break;
            case SAHARA_MODE_MEMORY_DEBUG:
                return "Memory Debug";
                break;
            case SAHARA_MODE_COMMAND:
                return "Command Mode";
                break;
            default:
                return "Unknown";
                break;
        }
    }

    /**
     * @brief SaharaHelper::getNamedClientCommand
     * @param command
     * @return
     */
    const char* getNamedClientCommand(uint32_t command)
    {
        switch(command) {
            case SAHARA_EXEC_CMD_NOP:
                return "NOP";
                break;
            case SAHARA_EXEC_CMD_SERIAL_NUM_READ:
                return "Read Serial Number";
                break;
            case SAHARA_EXEC_CMD_MSM_HW_ID_READ:
                return "Read MSM HW ID";
                break;
            case SAHARA_EXEC_CMD_OEM_PK_HASH_READ:
                return "Read OEM PK Hash";
                break;
            case SAHARA_EXEC_CMD_SWITCH_TO_DMSS_DLOAD:
                return "Switch To DMSS DLOAD";
                break;
            case SAHARA_EXEC_CMD_SWITCH_TO_STREAM_DLOAD:
                return "Switch To Streaming DLOAD";
                break;
            case SAHARA_EXEC_CMD_READ_DEBUG_DATA:
                return "Read Debug Data";
                break;
            case SAHARA_EXEC_CMD_GET_SOFTWARE_VERSION_SBL:
                return "Read SBL SW Version";
                break;
            default:
                return "Unknown";
                break;
        }
    }

    /**
     * @brief SaharaHelper::getNamedErrorStatus
     * @param status
     * @return
     */
    const char* getNamedErrorStatus(uint32_t status)
    {
        switch(status) {
            case SAHARA_STATUS_SUCCESS:
                return "Success";
                break;
            case SAHARA_NAK_INVALID_CMD:
                return "Invalid Command";
                break;
            case SAHARA_NAK_PROTOCOL_MISMATCH:
                return "Protocol Mismatch";
                break;
            case SAHARA_NAK_INVALID_TARGET_PROTOCOL:
                return "Invalid Target Protocol";
                break;
            case SAHARA_NAK_INVALID_HOST_PROTOCOL:
                return "Invalid Host Protocol";
                break;
            case SAHARA_NAK_INVALID_PACKET_SIZE:
                return "Invalid Packet Size";
                break;
            case SAHARA_NAK_UNEXPECTED_IMAGE_ID:
                return "Unexpected Image ID";
                break;
            case SAHARA_NAK_INVALID_HEADER_SIZE:
                return "Invalid Header Size";
                break;
            case SAHARA_NAK_INVALID_DATA_SIZE:
                return "Invalid Data Size";
                break;
            case SAHARA_NAK_INVALID_IMAGE_TYPE:
                return "Invalid Image Type";
                break;
            case SAHARA_NAK_INVALID_TX_LENGTH:
                return "Invalid TX Length";
                break;
            case SAHARA_NAK_INVALID_RX_LENGTH:
                return "Invalid RX Length";
                break;
            case SAHARA_NAK_GENERAL_TX_RX_ERROR:
                return "General TX RX Error";
                break;
            case SAHARA_NAK_UNSUPPORTED_NUM_PHDRS:
                return " Cannot receive specified number of program headers";
                break;
            case SAHARA_NAK_INVALID_PDHR_SIZE:
                return "Invalid data length received for program headers";
                break;
            case SAHARA_NAK_MULTIPLE_SHARED_SEG:
                return "Multiple shared segments found in ELF image";
                break;
            case SAHARA_NAK_UNINIT_PHDR_LOC:
                return "Uninitialized program header location";
                break;
            case SAHARA_NAK_INVALID_DEST_ADDR:
                return "Invalid destination address";
                break;
            case SAHARA_NAK_INVALID_IMG_HDR_DATA_SIZE:
                return "Invalid data size receieved in image header";
                break;
            case SAHARA_NAK_INVALID_ELF_HDR:
                return "Invalid ELF header received";
                break;
            case SAHARA_NAK_UNKNOWN_HOST_ERROR:
                return "Unknown Host Error";
                break;
            case SAHARA_NAK_TIMEOUT_RX:
                return "RX Timeout";
                break;
            case SAHARA_NAK_TIMEOUT_TX:
                return "TX Timeout";
                break;
            case SAHARA_NAK_INVALID_HOST_MODE:
                return "Invalid Host Mode";
                break;
            case SAHARA_NAK_INVALID_MEMORY_READ:
                return "Invalid Memory Read";
                break;
            case SAHARA_NAK_INVALID_DATA_SIZE_REQUEST:
                return "Invalid Data Size Request";
                break;
            case SAHARA_NAK_MEMORY_DEBUG_NOT_SUPPORTED:
                return "Memory Debug Not Supported";
                break;
            case SAHARA_NAK_INVALID_MODE_SWITCH:
                return "Invalid Mode Switch";
                break;
            case SAHARA_NAK_CMD_EXEC_FAILURE:
                return "Command Execute Failure";
                break;
            case SAHARA_NAK_EXEC_CMD_INVALID_PARAM:
                return "Invalid Command Parameter";
                break;
            case SAHARA_NAK_EXEC_CMD_UNSUPPORTED:
                return "Command Unsupported";
                break;
            case SAHARA_NAK_EXEC_DATA_INVALID_CLIENT_CMD:
                return "Invalid Client Command";
                break;
            case SAHARA_NAK_HASH_TABLE_AUTH_FAILURE:
                return "Hash Table Auth Failure";
                break;
            case SAHARA_NAK_HASH_VERIFICATION_FAILURE:
                return "Hash Table Verification Failure";
                break;
            case SAHARA_NAK_HASH_TABLE_NOT_FOUND:
                return "Hash Table Not Found";
                break;
            case SAHARA_NAK_TARGET_INIT_FAILURE:
                return "Target Init Failure";
                break;
            case SAHARA_NAK_IMAGE_AUTH_FAILURE:
                return "Image Auth Failure";
                break;
            default:
                return "Unknown";
                break;
        }
    }

    const char* getNamedRequestedImage(uint32_t imageId)
    {
        switch(imageId) {
            case SAHARA_IMAGE_NONE:
                return "None";
                break;
            case SAHARA_IMAGE_OEM_SBL_IMG:
                return "OEM SBL";
                break;
            case SAHARA_IMAGE_AMSS_IMG:
                return "AMSS";
                break;
            case SAHARA_IMAGE_QCSBL_IMG:
                return "QCSBL";
                break;
            case SAHARA_IMAGE_HASH_IMG:
                return "Hash";
                break;
            case SAHARA_IMAGE_APPSBL_IMG:
                return "APPSBL";
                break;
            case SAHARA_IMAGE_HOSTDL:
                return "HOSTDL";
                break;
            case SAHARA_IMAGE_DSP1:
                return "DSP1";
                break;
            case SAHARA_IMAGE_FSBL:
                return "FSBL";
                break;
            case SAHARA_IMAGE_DBL:
                return "DBL";
                break;
            case SAHARA_IMAGE_OSBL:
                return "OSBL";
                break;
            case SAHARA_IMAGE_DSP2:
                return "DSP2";
                break;
            case SAHARA_IMAGE_EHOSTDL:
                return "EHOSTDL";
                break;
            case SAHARA_IMAGE_NANDPRG:
                return "NANDPRG";
                break;
            case SAHARA_IMAGE_NORPRG:
                return "NORPRG";
                break;
            case SAHARA_IMAGE_RAMFS1:
                return "RAMFS1";
                break;
            case SAHARA_IMAGE_RAMFS2:
                return "RAMFS2";
                break;
            case SAHARA_IMAGE_ADSP_Q5:
                return "ADSP Q5";
                break;
            case SAHARA_IMAGE_APPS_KERNEL:
                return "APPS Kernel";
                break;
            case SAHARA_IMAGE_BACKUP_RAMFS:
                return "Backup RAMFS";
                break;
            case SAHARA_IMAGE_SBL1:
                return "SBL1";
                break;
            case SAHARA_IMAGE_SBL2:
                return "SBL2";
                break;
            case SAHARA_IMAGE_RPM:
                return "RPM";
                break;
            case SAHARA_IMAGE_SBL3:
                return "SBL3";
                break;
            case SAHARA_IMAGE_TZ:
                return "TZ";
                break;
            case SAHARA_IMAGE_SSD_KEYS:
                return "SSD Keys";
                break;
            case SAHARA_IMAGE_GEN:
                return "GEN";
                break;
            case SAHARA_IMAGE_DSP3:
                return "DSP3";
                break;
            case SAHARA_IMAGE_ACDB:
                return "ACDB";
                break;
            case SAHARA_IMAGE_WDT:
                return "WDT";
                break;
            case SAHARA_IMAGE_MBA:
                return "MBA";
                break;
            default:
                return "Unknown";
                break;

        }
    }

    const char* getNamedRequestedImageAlt(uint32_t imageId)
    {
        switch(imageId) {
            case SAHARA_IMAGE_ALT_NONE:
                return "None";
                break;
            case SAHARA_IMAGE_ALT_OEM_SBL:
                return "OEM SBL";
                break;
            case SAHARA_IMAGE_ALT_AMSS:
                return "AMSS";
                break;
            case SAHARA_IMAGE_ALT_QCSBL:
                return "QCSBL";
                break;
            case SAHARA_IMAGE_ALT_HASH:
                return "Hash";
                break;
            case SAHARA_IMAGE_ALT_NANDPRG:
                return "NANDPRG";
                break;
            case SAHARA_IMAGE_ALT_CFG_DATA:
                return "CFG DATA";
                break;
            case SAHARA_IMAGE_ALT_NORPRG:
                return "NORPRG";
                break;
            case SAHARA_IMAGE_ALT_HOSTDL:
                return "HOSTDL";
                break;
            case SAHARA_IMAGE_ALT_FSBL:
                return "FSBL";
                break;
            case SAHARA_IMAGE_ALT_DBL:
                return "DBL";
                break;
            case SAHARA_IMAGE_ALT_DBL_OLD:
                return "OLD DBL";
                break;
            case SAHARA_IMAGE_ALT_OSBL:
                return "OSBL";
                break;
            case SAHARA_IMAGE_ALT_APPS:
                return "APPS";
                break;
            case SAHARA_IMAGE_ALT_APPSBL:
                return "APPSBL";
                break;
            case SAHARA_IMAGE_ALT_DSP1:
                return "DSP1";
                break;
            case SAHARA_IMAGE_ALT_DSP2:
                return "DSP2";
                break;
            case SAHARA_IMAGE_ALT_EHOSTDL:
                return "EHOSTDL";
                break;
            case SAHARA_IMAGE_ALT_RAMFS1:
                return "RAMFS1";
                break;
            case SAHARA_IMAGE_ALT_RAMFS2:
                return "RAMFS2";
                break;
            case SAHARA_IMAGE_ALT_ADSP_Q5:
                return "ADSPQ5";
                break;
            case SAHARA_IMAGE_ALT_APPS_KERNEL:
                return "APPS KERNEL";
                break;
            default:
                return "Unknown";
                break;

        }

    }
}
