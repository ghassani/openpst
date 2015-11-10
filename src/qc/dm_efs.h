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

#ifndef _QC_DIAG_EFS_H
#define _QC_DIAG_EFS_H

#include "qc/dm.h"

#define DIAG_EFS_DEFAULT_WINDOW_SIZE  0x100000
#define DIAG_EFS_DEFAULT_WINDOW_BYTE_SIZE 0x100000

#define DIAG_EFS_VERSION     0x0001
#define DIAG_EFS_MIN_VERSION 0x0001
#define DIAG_EFS_MAX_VERSION 0x0001

enum DIAG_EFS_COMMAND {
    DIAG_EFS_HELLO = 0,  // Parameter negotiation packet               
    DIAG_EFS_QUERY = 1,  // Send information about EFS2 params         
    DIAG_EFS_OPEN = 2,  // Open a file                                
    DIAG_EFS_CLOSE = 3,  // Close a file                               
    DIAG_EFS_READ = 4,  // Read a file                                
    DIAG_EFS_WRITE = 5,  // Write a file                               
    DIAG_EFS_SYMLINK = 6,  // Create a symbolic link                     
    DIAG_EFS_READLINK = 7,  // Read a symbolic link                       
    DIAG_EFS_UNLINK = 8,  // Remove a symbolic link or file             
    DIAG_EFS_MKDIR = 9,  // Create a directory                         
    DIAG_EFS_RMDIR = 10,  // Remove a directory                         
    DIAG_EFS_OPENDIR = 11,  // Open a directory for reading               
    DIAG_EFS_READDIR = 12,  // Read a directory                           
    DIAG_EFS_CLOSEDIR = 13,  // Close an open directory                    
    DIAG_EFS_RENAME = 14,  // Rename a file or directory                 
    DIAG_EFS_STAT = 15,  // Obtain information about a named file      
    DIAG_EFS_LSTAT = 16,  // Obtain information about a symbolic link   
    DIAG_EFS_FSTAT = 17,  // Obtain information about a file descriptor 
    DIAG_EFS_CHMOD = 18,  // Change file permissions                    
    DIAG_EFS_STATFS = 19,  // Obtain file system information             
    DIAG_EFS_ACCESS = 20,  // Check a named file for accessibility       
    DIAG_EFS_DEV_INFO = 21,  // Get flash device info             
    DIAG_EFS_FACT_IMAGE_START = 22,  // Start data output for Factory Image
    DIAG_EFS_FACT_IMAGE_READ = 23,  // Get data for Factory Image         
    DIAG_EFS_FACT_IMAGE_END = 24,  // End data output for Factory Image  
    DIAG_EFS_PREP_FACT_IMAGE = 25,  // Prepare file system for image dump 
    DIAG_EFS_PUT_DEPRECATED = 26,  // Write an EFS item file             
    DIAG_EFS_GET_DEPRECATED = 27,  // Read an EFS item file              
    DIAG_EFS_ERROR = 28,  // Semd an EFS Error Packet back through DIAG 
    DIAG_EFS_EXTENDED_INFO = 29,  // Get Extra information.                
    DIAG_EFS_CHOWN = 30,  // Change ownership                      
    DIAG_EFS_BENCHMARK_START_TEST = 31,  // Start Benchmark               
    DIAG_EFS_BENCHMARK_GET_RESULTS = 32,  // Get Benchmark Report          
    DIAG_EFS_BENCHMARK_INIT = 33,  // Init/Reset Benchmark          
    DIAG_EFS_SET_RESERVATION = 34,  // Set group reservation         
    DIAG_EFS_SET_QUOTA = 35,  // Set group quota               
    DIAG_EFS_GET_GROUP_INFO = 36,  // Retrieve Q&R values           
    DIAG_EFS_DELTREE = 37,  // Delete a Directory Tree       
    DIAG_EFS_PUT = 38,  // Write a EFS item file in order
    DIAG_EFS_GET = 39,  // Read a EFS item file in order 
    DIAG_EFS_TRUNCATE = 40,  // Truncate a file by the name   
    DIAG_EFS_FTRUNCATE = 41,  // Truncate a file by a descriptor 
    DIAG_EFS_STATVFS_V2 = 42,  // Obtains extensive file system info 
    DIAG_EFS_MD5SUM = 43,  // Calculate md5 hash of a file  
    DIAG_EFS_HOTPLUG_FORMAT = 44,  // Format a Connected device 
    DIAG_EFS_SHRED = 45,  // Shred obsolete file content. 
    DIAG_EFS_SET_IDLE_DEV_EVT_DUR = 46,  // Idle_dev_evt_dur value in mins 
    DIAG_EFS_HOTPLUG_DEVICE_INFO = 47,  // get the hotplug device info.  
    DIAG_EFS_SYNC_NO_WAIT = 48,  // non-blocking sync of remotefs device 
    DIAG_EFS_SYNC_GET_STATUS = 49,  // query previously issued sync status 
    DIAG_EFS_TRUNCATE64 = 50,  // Truncate a file by the name.        
    DIAG_EFS_FTRUNCATE64 = 51,  // Truncate a file by a descriptor.    
    DIAG_EFS_LSEEK64 = 52,  // Seek to requested file offset.      
    DIAG_EFS_MAKE_GOLDEN_COPY = 53,  // Make golden copy for Remote Storage
    DIAG_EFS_FILESYSTEM_IMAGE_OPEN = 54,  //Open FileSystem Image extraction
    DIAG_EFS_FILESYSTEM_IMAGE_READ = 55,  // Read File System Image.        
    DIAG_EFS_FILESYSTEM_IMAGE_CLOSE = 56,  // Close File System Image.      
};

enum DIAG_EFS_ERROR {
    DIAG_EFS_INCONSISTENT_STATE  = 0x01,
    DIAG_EFS_INVALID_SEQ_NO      = 0x02,
    DIAG_EFS_DIR_NOT_OPEN        = 0x03,
    DIAG_EFS_DIRENT_NOT_FOUND    = 0x04,
    DIAG_EFS_INVALID_PATH        = 0x05,
    DIAG_EFS_PATH_TOO_LONG       = 0x06,
    DIAG_EFS_TOO_MANY_OPEN_DIRS  = 0x07,
    DIAG_EFS_INVALID_DIR_ENTRY   = 0x08,
    DIAG_EFS_TOO_MANY_OPEN_FILES = 0x09,
    DIAG_EFS_UNKNOWN_FILETYPE    = 0x0A,
    DIAG_EFS_NOT_NAND_FLASH      = 0x0B,
    DIAG_EFS_UNAVAILABLE_INFO    = 0x0C,
};


enum DIAG_EFS_FILE_TYPES {
    DIAG_EFS_FILE_TYPE_FILE         = 0x00,
    DIAG_EFS_FILE_TYPE_DIR          = 0x01,
    DIAG_EFS_FILE_TYPE_LINK         = 0x02,
    DIAG_EFS_FILE_TYPE_IMMOVABLE    = 0x03,
};

enum DIAG_EFS_DEVICE_TYPES {
    DIAG_EFS_DEVICE_TYPE_NOR  = 0x00,
    DIAG_EFS_DEVICE_TYPE_NAND = 0x01
};

enum DIAG_EFS_FILESYSTEM_IMAGE_TYPES {
    DIAG_EFS_FILESYSTEM_IMAGE_TAR = 0,  
    DIAG_EFS_FILESYSTEM_IMAGE_ZIP = 1
};

PACKED(typedef struct QcdmEfsHelloRequest{
    QcdmSubsysHeader header;
    uint32_t targetPacketWindowSize;        // Window size in packets for sends from phone
    uint32_t targetPacketWindowByteSize;    // Window size in bytes for sends from phone
    uint32_t hostPacketWindowSize;          // Window size in packets for sends from host
    uint32_t hostPacketWindowByteSize;      // Window size in bytes for sends from host
    uint32_t dirIteratorWindowSize;         // Window size in packets for dir iteration
    uint32_t dirIteratorWindowByteSize;     // Window size in bytes for dir iteration
    uint32_t version;         
    uint32_t minVersion;     
    uint32_t maxVersion;     
    uint32_t featureBits;    
}) QcdmEfsHelloRequest;

PACKED(typedef struct QcdmEfsHelloResponse{
    QcdmSubsysHeader header;
    uint32_t targetPacketWindowSize;        // Window size in packets for sends from phone
    uint32_t targetPacketWindowByteSize;    // Window size in bytes for sends from phone
    uint32_t hostPacketWindowSize;          // Window size in packets for sends from host
    uint32_t hostPacketWindowByteSize;      // Window size in bytes for sends from host
    uint32_t dirIteratorWindowSize;         // Window size in packets for dir iteration
    uint32_t dirIteratorWindowByteSize;     // Window size in bytes for dir iteration
    uint32_t version;
    uint32_t minVersion;
    uint32_t maxVersion;
    uint32_t featureBits;
}) QcdmEfsHelloResponse;

/**
* Query Command
*/
PACKED(typedef struct QcdmEfsQueryRequest{
    QcdmSubsysHeader header;
}) QcdmEfsQueryRequest;

PACKED(typedef struct QcdmEfsQueryResponse{
    QcdmSubsysHeader header;
    int32_t maxFilenameLength;
    int32_t mapPathnameLength;
    int32_t maxSymlinkDepth;
    int32_t maxFileSize;
    int32_t maxDirectories;
    int32_t maxMounts;
}) QcdmEfsQueryResponse;

/**
* Open File Command
*/
PACKED(typedef struct QcdmEfsOpenFileRequest{
    QcdmSubsysHeader header;
    int32_t flags;
    int32_t mode;
    char path[1];
}) QcdmEfsOpenFileRequest;

PACKED(typedef struct QcdmEfsOpenFileResponse{
    QcdmSubsysHeader header;
    int32_t fp;
    int32_t error;
}) QcdmEfsOpenFileResponse;

/**
* Close File Command
*/
PACKED(typedef struct QcdmEfsCloseFileRequest{
    QcdmSubsysHeader header;
    int32_t fp;
}) QcdmEfsCloseFileRequest;

PACKED(typedef struct QcdmEfsCloseFileReponse{
    QcdmSubsysHeader header;
    int32_t error;
}) QcdmEfsCloseFileReponse;

/**
* Read File Command
*/
PACKED(typedef struct QcdmEfsReadFileRequest{
    QcdmSubsysHeader header;
    int32_t fp;
    uint32_t size;
    uint32_t offset;
}) QcdmEfsReadFileRequest;

PACKED(typedef struct QcdmEfsReadFileResponse{
    QcdmSubsysHeader header;
    int32_t fp;
    uint32_t offset;
    int32_t bytesRead;
    int32_t error;
    uint8_t data[0];
}) QcdmEfsReadFileResponse;

/**
* Write File Command
*/
PACKED(typedef struct QcdmEfsWriteFileRequest{
    QcdmSubsysHeader header;
    int32_t fp;
    uint32_t offset;
    uint8_t data[1];
}) QcdmEfsWriteFileRequest;

PACKED(typedef struct QcdmEfsWriteFileResponse{
    QcdmSubsysHeader header;
    int32_t fp;
    uint32_t offset;
    int32_t bytesWritten;
    int32_t error;
}) QcdmEfsWriteFileResponse;

/**
* Create Symlink Command
*/
PACKED(typedef struct QcdmEfsCreateLinkRequest{
    QcdmSubsysHeader header;
    char path[1];
    char newPath[1];
}) QcdmEfsCreateLinkRequest;

PACKED(typedef struct QcdmEfsCreateLinkResponse{
    QcdmSubsysHeader header;
    int32_t error;
}) QcdmEfsCreateLinkResponse;

/**
* Read Symlink Command
*/
PACKED(typedef struct QcdmEfsReadLinkRequest{
    QcdmSubsysHeader header;
    char path[1];
}) QcdmEfsReadLinkRequest;

PACKED(typedef struct QcdmEfsReadLinkResponse{
    QcdmSubsysHeader header;
    int32_t error;
    char path[1];
}) QcdmEfsReadLinkResponse;

/**
* Unlink Command
*/
PACKED(typedef struct QcdmEfsUnlinkRequest{
    QcdmSubsysHeader header;
    char path[1];
}) QcdmEfsUnlinkRequest;

PACKED(typedef struct QcdmEfsUnlinkResponse{
    QcdmSubsysHeader header;
    int32_t error;
}) QcdmEfsUnlinkResponse;

/**
* Create Directory Command
*/
PACKED(typedef struct QcdmEfsMkdirRequest{
    QcdmSubsysHeader header;
    int16_t mode;
    char path[1];
}) QcdmEfsMkdirRequest;

PACKED(typedef struct QcdmEfsMkdirResponse{
    QcdmSubsysHeader header;
    int32_t error;
}) QcdmEfsMkdirResponse;

/**
* Remove Directory Command
*/
PACKED(typedef struct QcdmEfsRmdirRequest{
    QcdmSubsysHeader header;
    char path[1];
}) QcdmEfsRmdirRequest;

PACKED(typedef struct QcdmEfsRmdirResponse{
    QcdmSubsysHeader header;
    int32_t error;
}) QcdmEfsRmdirResponse;

/**
* Open Directory Command
*/
PACKED(typedef struct QcdmEfsOpenDirRequest{
    QcdmSubsysHeader header;
    char path[1];
}) QcdmEfsOpenDirRequest;

PACKED(typedef struct QcdmEfsOpenDirResponse{
    QcdmSubsysHeader header;
    uint32_t dp;
    int32_t error;
}) QcdmEfsOpenDirResponse;

/**
* Read Directory Command
*/
PACKED(typedef struct QcdmEfsReadDirRequest{
    QcdmSubsysHeader header;
    uint32_t dp;
    int32_t sequenceNumber;
}) QcdmEfsReadDirRequest;

PACKED(typedef struct QcdmEfsReadDirResponse{
    QcdmSubsysHeader header;
    uint32_t dp;
    int32_t sequenceNumber;
    int32_t error;
    int32_t entryType;
    int32_t mode;
    int32_t size;
    int32_t atime;
    int32_t mtime;
    int32_t ctime;
    char name[1];
}) QcdmEfsReadDirResponse;

/**
* Close Directory Command
*/
PACKED(typedef struct QcdmEfsCloseDirRequest{
    QcdmSubsysHeader header;
    uint32_t dp;
}) QcdmEfsCloseDirRequest;

PACKED(typedef struct QcdmEfsCloseDirResponse{
    QcdmSubsysHeader header;
    int32_t error;
}) QcdmEfsCloseDirResponse;

/**
* Rename Command
*/
PACKED(typedef struct QcdmEfsRenameRequest{
    QcdmSubsysHeader header;
    char path[1];
    char newPath[1];
}) QcdmEfsRenameRequest;

PACKED(typedef struct QcdmEfsRenameResponse{
    QcdmSubsysHeader header;
    int32_t error;
}) QcdmEfsRenameResponse;

/**
* Stat Command
*/
PACKED(typedef struct QcdmEfsStatRequest{
    QcdmSubsysHeader header;
    char path[1];
}) QcdmEfsStatRequest;

PACKED(typedef struct QcdmEfsStatResponse{
    QcdmSubsysHeader header;
    int32_t error;
    int32_t mode;
    int32_t size;
    int32_t linkCount;
    int32_t atime;
    int32_t mtime;
    int32_t ctime;
}) QcdmEfsStatResponse;

/**
* Lstat Command
*/
PACKED(typedef struct QcdmEfsLstatRequest{
    QcdmSubsysHeader header;
    char path[1];
}) QcdmEfsLstatRequest;

PACKED(typedef struct QcdmEfsLstatResponse{
    QcdmSubsysHeader header;
    int32_t error;
    int32_t mode;
    int32_t atime;
    int32_t mtime;
    int32_t ctime;
}) QcdmEfsLstatResponse;

/**
* Fstat Command
*/
PACKED(typedef struct QcdmEfsFstatRequest{
    QcdmSubsysHeader header;
    int32_t fp;
}) QcdmEfsFstatRequest;

PACKED(typedef struct QcdmEfsFstatResponse{
    QcdmSubsysHeader header;
    int32_t error;
    int32_t mode;
    int32_t size;
    int32_t linkCount;
    int32_t atime;
    int32_t mtime;
    int32_t ctime;
}) QcdmEfsFstatResponse;

/**
* CHMOD Command
*/
PACKED(typedef struct QcdmEfsChmodRequest{
    QcdmSubsysHeader header;
    int16_t mode;
    char path[1];
}) QcdmEfsChmodRequest;

PACKED(typedef struct QcdmEfsChmodResponse{
    QcdmSubsysHeader header;
    int32_t error;
}) QcdmEfsChmodResponse;

/**
* Statfs Command
*/
PACKED(typedef struct QcdmEfsStatfsRequest{
    QcdmSubsysHeader header;
    char path[1];
}) QcdmEfsStatfsRequest;

PACKED(typedef struct QcdmEfsStatfsResponse{
    QcdmSubsysHeader header;
    int32_t error;
    int32_t filesystemId;
    uint8_t filesystemType[8];
    int32_t blockSize;
    int32_t totalBlocks;
    int32_t availableBlocks;
    int32_t freeBlocks;
    int32_t maxFileSize;
    int32_t numberofFiles;
    int32_t maxNumberOfFiles;
}) QcdmEfsStatfsResponse;

/**
* Access Command
*/
PACKED(typedef struct QcdmEfsAccessRequest{
    QcdmSubsysHeader header;
    char permissionBits;
    char path[1];
}) QcdmEfsAccessRequest;

PACKED(typedef struct QcdmEfsAccessResponse{
    QcdmSubsysHeader header;
    int32_t error;
}) QcdmEfsAccessResponse;

/**
* Device Info Command
*/
PACKED(typedef struct QcdmEfsDeviceInfoRequest{
    QcdmSubsysHeader header;
}) QcdmEfsDeviceInfoRequest;

PACKED(typedef struct QcdmEfsDeviceInfoResponse{
    QcdmSubsysHeader header;
    int32_t error;
    int32_t totalNumberOfBlocks;
    int32_t pagesPerBlock;
    int32_t pageSize;
    int32_t totalPageSize;
    int32_t makerId;
    int32_t deviceId;
    uint8_t deviceType;
    uint8_t name[1];
}) QcdmEfsDeviceInfoResponse;


/**
* Factory Image Start
*/
PACKED(typedef struct QcdmEfsFactoryImageStartRequest{
    QcdmSubsysHeader header;
}) QcdmEfsFactoryImageStartRequest;

PACKED(typedef struct QcdmEfsFactoryImageStartResponse{
    QcdmSubsysHeader header;
    int32_t error;
}) QcdmEfsFactoryImageStartResponse;

/**
* Factory Image Read
*/
PACKED(typedef struct QcdmEfsFactoryImageReadRequest{
    QcdmSubsysHeader header;
    int8_t streamState;
    int8_t infoClusterSent;
    int16_t clusterMapSequence;
    int16_t clusterDataSequence;
}) QcdmEfsFactoryImageReadRequest;

PACKED(typedef struct QcdmEfsFactoryImageReadResponse{
    QcdmSubsysHeader header;
    int32_t error;
    int8_t streamState;
    int8_t infoClusterSent;
    int16_t clusterMapSequence;
    int16_t clusterDataSequence;
    uint8_t data[0];
}) QcdmEfsFactoryImageReadResponse;

/**
* Factory Image End
*/
PACKED(typedef struct QcdmEfsFactoryImageEndRequest{
    QcdmSubsysHeader header;
}) QcdmEfsFactoryImageEndRequest;

PACKED(typedef struct QcdmEfsFactoryImageEndResponse{
    QcdmSubsysHeader header;
    int32_t error;
}) QcdmEfsFactoryImageEndResponse;

/**
* Factory Image Prepare
*/
PACKED(typedef struct QcdmEfsFactoryImagePrepareRequest{
    QcdmSubsysHeader header;
}) QcdmEfsFactoryImagePrepareRequest;

PACKED(typedef struct QcdmEfsFactoryImagePrepareResponse{
    QcdmSubsysHeader header;
    int32_t error;
}) QcdmEfsFactoryImagePrepareResponse;

/**
* Chown
*/
PACKED(typedef struct QcdmEfsChownRequest{
    QcdmSubsysHeader header;
    uint32_t uid;
    uint32_t gid;
    uint8_t path[0];
}) QcdmEfsChownRequest;

PACKED(typedef struct QcdmEfsChownResponse{
    QcdmSubsysHeader header;
    int32_t error;
}) QcdmEfsChownResponse;

/**
* Set Reservation
*/
PACKED(typedef struct QcdmEfsSetReservationRequest{
    QcdmSubsysHeader header;
    uint32_t gid;
    size_t amount;
    uint8_t path[0];
}) QcdmEfsSetReservationRequest;

PACKED(typedef struct QcdmEfsSetReservationResponse{
    QcdmSubsysHeader header;
    int32_t error;
}) QcdmEfsSetReservationResponse;

/**
* Set Quota
*/
PACKED(typedef struct QcdmEfsSetQuotaRequest{
    QcdmSubsysHeader header;
    uint32_t gid;
    size_t amount;
    uint8_t path[0];
}) QcdmEfsSetQuotaRequest;

PACKED(typedef struct QcdmEfsSetQuotaResponse{
    QcdmSubsysHeader header;
    int32_t error;
}) QcdmEfsSetQuotaResponse;

/**
* Get Group Info
*/
PACKED(typedef struct QcdmEfsGetGroupInfoRequest{
    QcdmSubsysHeader header;
    uint32_t gid;
    uint8_t path[0];
}) QcdmEfsGetGroupInfoRequest;

PACKED(typedef struct QcdmEfsGetGroupInfoResponse{
    QcdmSubsysHeader header;
    int32_t error;
    uint32_t quotaSize;
    uint32_t reservationSize;
    uint32_t spaceUsed;
}) QcdmEfsGetGroupInfoResponse;

/**
* Deltree
*/
PACKED(typedef struct QcdmEfsDeltreeRequest{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint8_t path[0];
}) QcdmEfsDeltreeRequest;

PACKED(typedef struct QcdmEfsDeltreeResponse{
    QcdmSubsysHeader header;
    uint16_t sequence;
    int32_t error;
}) QcdmEfsDeltreeResponse;


/**
* Truncate
*/
PACKED(typedef struct QcdmEfsTruncateRequest{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint32_t length;
    uint8_t path[0];
}) QcdmEfsTruncateRequest;

PACKED(typedef struct QcdmEfsTruncateResponse{
    QcdmSubsysHeader header;
    uint16_t sequence;
    int32_t error;
}) QcdmEfsTruncateResponse;

/**
* Ftruncate
*/
PACKED(typedef struct QcdmEfsFtncateRequest{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint32_t length;
    int32_t fp;
}) QcdmEfsFtncateRequest;

PACKED(typedef struct QcdmEfsFtncateResponse{
    QcdmSubsysHeader header;
    uint16_t sequence;
    int32_t error;
}) QcdmEfsFtncateResponse;

/**
* Statfs V2
*/
PACKED(typedef struct QcdmEfsStatfsV2Request{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint8_t path[0];
}) QcdmEfsStatfsV2Request;

PACKED(typedef struct QcdmEfsStatfsV2Response{
    QcdmSubsysHeader header;
    uint16_t sequence;
    int32_t error;
    uint32_t fsId;
    int8_t fsType[8];
    uint32_t blockSize;
    uint32_t totalBlocks;
    uint32_t availableBlocks;
    uint32_t freeBlocks;
    uint32_t maxFileSize;
    uint32_t fileCount;
    uint32_t maxFiles;
    uint32_t maxWriteSize;
    uint32_t maxPathSize;
    uint32_t caseSensitive;
    uint32_t casePreserving;
    uint32_t fMaxFileSize;
    uint32_t fFileSizeUnits;
    uint32_t fMaxOpenFiles;
    uint32_t fNameRule;
    uint32_t fEameEncoding;
}) QcdmEfsStatfsV2Response;


/**
* MD5 Sum
*/
PACKED(typedef struct QcdmEfsMd5SumRequest{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint8_t path[1];
}) QcdmEfsMd5SumRequest;

PACKED(typedef struct QcdmEfsMd5SumResponse{
    QcdmSubsysHeader header;
    uint16_t sequence;
    int32_t error;
    uint8_t hash[16];
}) QcdmEfsMd5SumResponse;

/**
* Hotplug Format
*/
PACKED(typedef struct QcdmEfsHotplugFormatRequest{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint8_t path[0];
}) QcdmEfsHotplugFormatRequest;

PACKED(typedef struct QcdmEfsHotplugFormatResponse{
    QcdmSubsysHeader header;
    uint16_t sequence;
    int32_t error;
    uint8_t hash[16];
}) QcdmEfsHotplugFormatResponse;

/**
* Shred
*/
PACKED(typedef struct QcdmEfsShredRequest{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint8_t path[0];
}) QcdmEfsShredRequest;

PACKED(typedef struct QcdmEfsShredResponse{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint16_t results;
    int32_t error;
}) QcdmEfsShredResponse;

/**
* Hotplug Format
*/
PACKED(typedef struct QcdmEfsHotplugDeviceInfoRequest{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint8_t path[0];
}) QcdmEfsHotplugDeviceInfoRequest;

PACKED(typedef struct QcdmEfsHotplugDeviceInfoResponse{
    QcdmSubsysHeader header;
    uint16_t sequence;
    int32_t error;
    uint32_t manufacturerId;
    uint32_t oemId;
    uint32_t productRevision;
    uint8_t productName[16];
    uint8_t manufactureredDate[16];
}) QcdmEfsHotplugDeviceInfoResponse;

/**
* Sync
*/
PACKED(typedef struct QcdmEfsSyncRequest{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint8_t path[0];
}) QcdmEfsSyncRequest;

PACKED(typedef struct QcdmEfsSyncResponse{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint32_t token;
    int32_t error;
}) QcdmEfsSyncResponse;

/**
* Sync Get Status
*/
PACKED(typedef struct QcdmEfsGetSyncStatusRequest{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint32_t token;
    char path[1];
}) QcdmEfsGetSyncStatusRequest;

PACKED(typedef struct QcdmEfsGetSyncStatusResponse{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint8_t status;
    int32_t error;
}) QcdmEfsGetSyncStatusResponse;

/**
* Truncate64
*/
PACKED(typedef struct QcdmEfsTruncate64Request{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint32_t lengthLsb;
    uint32_t lengthMsb;
    uint8_t path[0];
}) QcdmEfsTruncate64Request;

PACKED(typedef struct QcdmEfsTruncate64Response{
    QcdmSubsysHeader header;
    uint16_t sequence;
    int32_t error;
}) QcdmEfsTruncate64Response;

/**
* Ftruncate64
*/
PACKED(typedef struct QcdmEfsFtruncate64Request{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint32_t lengthLsb;
    uint32_t lengthMsb;
    int32_t fp;
}) QcdmEfsFtruncate64Request;

PACKED(typedef struct QcdmEfsFtruncate64Response{
    QcdmSubsysHeader header;
    uint16_t sequence;
    int32_t error;
}) QcdmEfsFtruncate64Response;

/**
* Lseek64
*/
PACKED(typedef struct QcdmEfsLseek64Request{
    QcdmSubsysHeader header;
    uint16_t sequence;
    int whence;
    uint32_t lengthLsb;
    uint32_t lengthMsb;
    int32_t fp;
}) QcdmEfsLseek64Request;

PACKED(typedef struct QcdmEfsLseek64Response{
    QcdmSubsysHeader header;
    uint16_t sequence;
    int32_t error;
    uint32_t offsetLsb;
    uint32_t offsetMsb;
}) QcdmEfsLseek64Response;

/**
* Make golden copy
*/
PACKED(typedef struct QcdmEfsMakeGoldenCopyRequest{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint8_t path[0];
}) QcdmEfsMakeGoldenCopyRequest;

PACKED(typedef struct QcdmEfsMakeGoldenCopyResponse{
    QcdmSubsysHeader header;
    uint16_t sequence;
    int32_t error;
}) QcdmEfsMakeGoldenCopyResponse;

/**
* File System Image Open
*/
PACKED(typedef struct QcdmEfsFsImageOpenRequest{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint8_t imageType; // fs_diag_filesystem_image_type
    uint8_t path[0];
}) QcdmEfsFsImageOpenRequest;

PACKED(typedef struct QcdmEfsFsImageOpenResponse{
    QcdmSubsysHeader header;
    uint16_t sequence;
    int32_t handle;
    int32_t error;
}) QcdmEfsFsImageOpenResponse;

/**
* File System Image Read
*/
PACKED(typedef struct QcdmEfsFsImageReadRequest{
    QcdmSubsysHeader header;
    uint16_t sequence;
    int32_t handle; 
}) QcdmEfsFsImageReadRequest;

PACKED(typedef struct QcdmEfsFsImageReadResponse{
    QcdmSubsysHeader header;
    uint16_t sequence;
    uint16_t length;
    int32_t error;
    uint8_t data[0];
}) QcdmEfsFsImageReadResponse;

/**
* File System Image Close
*/
PACKED(typedef struct QcdmEfsFsImageCloseRequest{
    QcdmSubsysHeader header;
    uint16_t sequence;
    int32_t handle;
}) QcdmEfsFsImageCloseRequest;

PACKED(typedef struct QcdmEfsFsImageCloseResponse{
    QcdmSubsysHeader header;
    uint16_t sequence;
    int32_t error;
}) QcdmEfsFsImageCloseResponse;

#endif // _QC_DIAG_EFS_H
