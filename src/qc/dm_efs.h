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

enum DIAG_EFS_FILE_TYPES {
	DIAG_EFS_FILE_TYPE_FILE			= 0x00,
	DIAG_EFS_FILE_TYPE_DIR			= 0x01,
	DIAG_EFS_FILE_TYPE_LINK			= 0x02,
	DIAG_EFS_FILE_TYPE_IMMOVABLE	= 0x03,
};

enum DIAG_EFS_DEVICE_TYPES {
	DIAG_EFS_DEVICE_TYPE_NOR  = 0x00,
	DIAG_EFS_DEVICE_TYPE_NAND = 0x01
};

PACKED(typedef struct diag_subsys_efs_hello_tx_t{
	qcdm_subsys_header_t header;
	uint32_t targetPacketWindowSize;		// Window size in packets for sends from phone
	uint32_t targetPacketWindowByteSize;	// Window size in bytes for sends from phone
	uint32_t hostPacketWindowSize;			// Window size in packets for sends from host
	uint32_t hostPacketWindowByteSize;		// Window size in bytes for sends from host
	uint32_t dirIteratorWindowSize;			// Window size in packets for dir iteration
	uint32_t dirIteratorWindowByteSize;		// Window size in bytes for dir iteration
	uint32_t version;         
	uint32_t minVersion;     
	uint32_t maxVersion;     
	uint32_t featureBits;    
}) diag_subsys_efs_hello_tx_t;

PACKED(typedef struct diag_subsys_efs_hello_rx_t{
	qcdm_subsys_header_t header;
	uint32_t targetPacketWindowSize;		// Window size in packets for sends from phone
	uint32_t targetPacketWindowByteSize;	// Window size in bytes for sends from phone
	uint32_t hostPacketWindowSize;			// Window size in packets for sends from host
	uint32_t hostPacketWindowByteSize;		// Window size in bytes for sends from host
	uint32_t dirIteratorWindowSize;			// Window size in packets for dir iteration
	uint32_t dirIteratorWindowByteSize;		// Window size in bytes for dir iteration
	uint32_t version;
	uint32_t minVersion;
	uint32_t maxVersion;
	uint32_t featureBits;
}) diag_subsys_efs_hello_rx_t;

/**
* Query Command
*/
PACKED(typedef struct diag_subsys_efs_query_tx_t{
	qcdm_subsys_header_t header;
}) diag_subsys_efs_query_tx_t;

PACKED(typedef struct diag_subsys_efs_query_rx_t{
	qcdm_subsys_header_t header;
	int32_t maxFilenameLength;
	int32_t mapPathnameLength;
	int32_t maxSymlinkDepth;
	int32_t maxFileSize;
	int32_t maxDirectories;
	int32_t maxMounts;
}) diag_subsys_efs_query_rx_t;

/**
* Open File Command
*/
PACKED(typedef struct diag_subsys_efs_open_file_tx_t{
	qcdm_subsys_header_t header;
	int32_t flags;
	int32_t mode;
	char path[1];
}) diag_subsys_efs_open_file_tx_t;

PACKED(typedef struct diag_subsys_efs_open_file_rx_t{
	qcdm_subsys_header_t header;
	int32_t fp;
	int32_t error;
}) diag_subsys_efs_open_file_rx_t;

/**
* Close File Command
*/
PACKED(typedef struct diag_subsys_efs_close_file_tx_t{
	qcdm_subsys_header_t header;
	int32_t fp;
}) diag_subsys_efs_close_file_tx_t;

PACKED(typedef struct diag_subsys_efs_close_file_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
}) diag_subsys_efs_close_file_rx_t;

/**
* Read File Command
*/
PACKED(typedef struct diag_subsys_efs_read_file_tx_t{
	qcdm_subsys_header_t header;
	int32_t fp;
	uint32_t size;
	uint32_t offset;
}) diag_subsys_efs_read_file_tx_t;

PACKED(typedef struct diag_subsys_efs_read_file_rx_t{
	qcdm_subsys_header_t header;
	int32_t fp;
	uint32_t offset;
	int32_t bytesRead;
	int32_t error;
	uint8_t data[0];
}) diag_subsys_efs_read_file_rx_t;

/**
* Write File Command
*/
PACKED(typedef struct diag_subsys_efs_write_file_tx_t{
	qcdm_subsys_header_t header;
	int32_t fp;
	uint32_t offset;
	uint8_t data[1];
}) diag_subsys_efs_write_file_tx_t;

PACKED(typedef struct diag_subsys_efs_write_file_rx_t{
	qcdm_subsys_header_t header;
	int32_t fp;
	uint32_t offset;
	int32_t bytesWritten;
	int32_t error;
}) diag_subsys_efs_write_file_rx_t;

/**
* Create Symlink Command
*/
PACKED(typedef struct diag_subsys_efs_create_link_tx_t{
	qcdm_subsys_header_t header;
	char path[1];
	char newPath[1];
}) diag_subsys_efs_create_link_tx_t;

PACKED(typedef struct diag_subsys_efs_create_link_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
}) diag_subsys_efs_create_link_rx_t;

/**
* Read Symlink Command
*/
PACKED(typedef struct diag_subsys_efs_read_link_tx_t{
	qcdm_subsys_header_t header;
	char path[1];
}) diag_subsys_efs_read_link_tx_t;

PACKED(typedef struct diag_subsys_efs_read_link_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
	char path[1];
}) diag_subsys_efs_read_link_rx_t;

/**
* Unlink Command
*/
PACKED(typedef struct diag_subsys_efs_unlink_tx_t{
	qcdm_subsys_header_t header;
	char path[1];
}) diag_subsys_efs_unlink_tx_t;

PACKED(typedef struct diag_subsys_efs_unlink_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
}) diag_subsys_efs_unlink_rx_t;

/**
* Create Directory Command
*/
PACKED(typedef struct diag_subsys_efs_mkdir_tx_t{
	qcdm_subsys_header_t header;
	int16_t mode;
	char path[1];
}) diag_subsys_efs_mkdir_tx_t;

PACKED(typedef struct diag_subsys_efs_mkdir_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
}) diag_subsys_efs_mkdir_rx_t;

/**
* Remove Directory Command
*/
PACKED(typedef struct diag_subsys_efs_rmdir_tx_t{
	qcdm_subsys_header_t header;
	char path[1];
}) diag_subsys_efs_rmdir_tx_t;

PACKED(typedef struct diag_subsys_efs_rmdir_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
}) diag_subsys_efs_rmdir_rx_t;

/**
* Open Directory Command
*/
PACKED(typedef struct diag_subsys_efs_open_dir_tx_t{
	qcdm_subsys_header_t header;
	char path[1];
}) diag_subsys_efs_open_dir_tx_t;

PACKED(typedef struct diag_subsys_efs_open_dir_rx_t{
	qcdm_subsys_header_t header;
	uint32_t dp;
	int32_t error;
}) diag_subsys_efs_open_dir_rx_t;

/**
* Read Directory Command
*/
PACKED(typedef struct diag_subsys_efs_read_dir_tx_t{
	qcdm_subsys_header_t header;
	uint32_t dp;
	int32_t sequenceNumber;
}) diag_subsys_efs_read_dir_tx_t;

PACKED(typedef struct diag_subsys_efs_read_dir_rx_t{
	qcdm_subsys_header_t header;
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
}) diag_subsys_efs_read_dir_rx_t;

/**
* Close Directory Command
*/
PACKED(typedef struct diag_subsys_efs_close_dir_tx_t{
	qcdm_subsys_header_t header;
	uint32_t dp;
}) diag_subsys_efs_close_dir_tx_t;

PACKED(typedef struct diag_subsys_efs_close_dir_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
}) diag_subsys_efs_close_dir_rx_t;

/**
* Rename Command
*/
PACKED(typedef struct diag_subsys_efs_rename_tx_t{
	qcdm_subsys_header_t header;
	char path[1];
	char newPath[1];
}) diag_subsys_efs_rename_tx_t;

PACKED(typedef struct diag_subsys_efs_rename_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
}) diag_subsys_efs_rename_rx_t;

/**
* Stat Command
*/
PACKED(typedef struct diag_subsys_efs_stat_tx_t{
	qcdm_subsys_header_t header;
	char path[1];
}) diag_subsys_efs_stat_tx_t;

PACKED(typedef struct diag_subsys_efs_stat_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
	int32_t mode;
	int32_t size;
	int32_t linkCount;
	int32_t atime;
	int32_t mtime;
	int32_t ctime;
}) diag_subsys_efs_stat_rx_t;

/**
* Lstat Command
*/
PACKED(typedef struct diag_subsys_efs_lstat_tx_t{
	qcdm_subsys_header_t header;
	char path[1];
}) diag_subsys_efs_lstat_tx_t;

PACKED(typedef struct diag_subsys_efs_lstat_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
	int32_t mode;
	int32_t atime;
	int32_t mtime;
	int32_t ctime;
}) diag_subsys_efs_lstat_rx_t;

/**
* Fstat Command
*/
PACKED(typedef struct diag_subsys_efs_fstat_tx_t{
	qcdm_subsys_header_t header;
	int32_t fp;
}) diag_subsys_efs_fstat_tx_t;

PACKED(typedef struct diag_subsys_efs_fstat_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
	int32_t mode;
	int32_t size;
	int32_t linkCount;
	int32_t atime;
	int32_t mtime;
	int32_t ctime;
}) diag_subsys_efs_fstat_rx_t;

/**
* CHMOD Command
*/
PACKED(typedef struct diag_subsys_efs_chmod_tx_t{
	qcdm_subsys_header_t header;
	int16_t mode;
	char path[1];
}) diag_subsys_efs_chmod_tx_t;

PACKED(typedef struct diag_subsys_efs_chmod_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
}) diag_subsys_efs_chmod_rx_t;

/**
* Statfs Command
*/
PACKED(typedef struct diag_subsys_efs_statfs_tx_t{
	qcdm_subsys_header_t header;
	char path[1];
}) diag_subsys_efs_statfs_tx_t;

PACKED(typedef struct diag_subsys_efs_statfs_rx_t{
	qcdm_subsys_header_t header;
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
}) diag_subsys_efs_statfs_rx_t;

/**
* Access Command
*/
PACKED(typedef struct diag_subsys_efs_access_tx_t{
	qcdm_subsys_header_t header;
	char permissionBits;
	char path[1];
}) diag_subsys_efs_access_tx_t;

PACKED(typedef struct diag_subsys_efs_access_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
}) diag_subsys_efs_access_rx_t;

/**
* Device Info Command
*/
PACKED(typedef struct diag_subsys_efs_device_info_tx_t{
	qcdm_subsys_header_t header;
}) diag_subsys_efs_device_info_tx_t;

PACKED(typedef struct diag_subsys_efs_device_info_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
	int32_t totalNumberOfBlocks;
	int32_t pagesPerBlock;
	int32_t pageSize;
	int32_t totalPageSize;
	int32_t makerId;
	int32_t deviceId;
	uint8_t deviceType;
	uint8_t name[1];
}) diag_subsys_efs_device_info_rx_t;


/**
* Factory Image Start
*/
PACKED(typedef struct diag_subsys_efs_factory_image_start_tx_t{
	qcdm_subsys_header_t header;
}) diag_subsys_efs_factory_image_start_tx_t;

PACKED(typedef struct diag_subsys_efs_factory_image_start_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
}) diag_subsys_efs_factory_image_start_rx_t;

/**
* Factory Image Read
*/
PACKED(typedef struct diag_subsys_efs_factory_image_read_tx_t{
	qcdm_subsys_header_t header;
	int8_t streamState;
	int8_t infoClusterSent;
	int16_t clusterMapSequence;
	int16_t clusterDataSequence;
}) diag_subsys_efs_factory_image_read_tx_t;

PACKED(typedef struct diag_subsys_efs_factory_image_read_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
	int8_t streamState;
	int8_t infoClusterSent;
	int16_t clusterMapSequence;
	int16_t clusterDataSequence;
	uint8_t data[0];
}) diag_subsys_efs_factory_image_read_rx_t;

/**
* Factory Image End
*/
PACKED(typedef struct diag_subsys_efs_factory_image_end_tx_t{
	qcdm_subsys_header_t header;
}) diag_subsys_efs_factory_image_end_tx_t;

PACKED(typedef struct diag_subsys_efs_factory_image_end_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
}) diag_subsys_efs_factory_image_end_rx_t;

/**
* Factory Image Prepare
*/
PACKED(typedef struct diag_subsys_efs_factory_image_prepare_tx_t{
	qcdm_subsys_header_t header;
}) diag_subsys_efs_factory_image_prepare_tx_t;

PACKED(typedef struct diag_subsys_efs_factory_image_prepare_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
}) diag_subsys_efs_factory_image_prepare_rx_t;


/**
* Cown
*/
PACKED(typedef struct diag_subsys_efs_chown_tx_t{
	qcdm_subsys_header_t header;
	uint32_t uid;
	uint32_t gid;
	uint8_t path[0];
}) diag_subsys_efs_chown_tx_t;

PACKED(typedef struct diag_subsys_efs_chown_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
}) diag_subsys_efs_chown_rx_t;

/**
* Set Reservation
*/
PACKED(typedef struct diag_subsys_efs_set_reservation_tx_t{
	qcdm_subsys_header_t header;
	uint32_t gid;
	size_t amount;
	uint8_t path[0];
}) diag_subsys_efs_set_reservation_tx_t;

PACKED(typedef struct diag_subsys_efs_set_reservation_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
}) diag_subsys_efs_set_reservation_rx_t;

/**
* Set Quota
*/
PACKED(typedef struct diag_subsys_efs_set_quota_tx_t{
	qcdm_subsys_header_t header;
	uint32_t gid;
	size_t amount;
	uint8_t path[0];
}) diag_subsys_efs_set_quota_tx_t;

PACKED(typedef struct diag_subsys_efs_set_quota_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
}) diag_subsys_efs_set_quota_rx_t;

/**
* Get Group Info
*/
PACKED(typedef struct diag_subsys_efs_get_group_info_tx_t{
	qcdm_subsys_header_t header;
	uint32_t gid;
	uint8_t path[0];
}) diag_subsys_efs_get_group_info_tx_t;

PACKED(typedef struct diag_subsys_efs_get_group_info_rx_t{
	qcdm_subsys_header_t header;
	int32_t error;
	uint32_t quotaSize;
	uint32_t reservationSize;
	uint32_t spaceUsed;
}) diag_subsys_efs_get_group_info_rx_t;

/**
* Deltree
*/
PACKED(typedef struct diag_subsys_efs_deltree_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint8_t path[0];
}) diag_subsys_efs_deltree_tx_t;

PACKED(typedef struct diag_subsys_efs_deltree_rx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	int32_t error;
}) diag_subsys_efs_deltree_rx_t;


/**
* Truncate
*/
PACKED(typedef struct diag_subsys_efs_truncate_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint32_t length;
	uint8_t path[0];
}) diag_subsys_efs_truncate_tx_t;

PACKED(typedef struct diag_subsys_efs_truncate_rx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	int32_t error;
}) diag_subsys_efs_truncate_rx_t;

/**
* Ftruncate
*/
PACKED(typedef struct diag_subsys_efs_ftruncate_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint32_t length;
	int32_t fp;
}) diag_subsys_efs_ftruncate_tx_t;

PACKED(typedef struct diag_subsys_efs_ftruncate_rx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	int32_t error;
}) diag_subsys_efs_ftruncate_rx_t;

/**
* Statfs V2
*/
PACKED(typedef struct diag_subsys_efs_statfs_v2_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint8_t path[0];
}) diag_subsys_efs_statfs_v2_tx_t;

PACKED(typedef struct diag_subsys_efs_statfs_v2_rx_t{
	qcdm_subsys_header_t header;
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
}) diag_subsys_efs_statfs_v2_rx_t;


/**
* MD5 Sum
*/
PACKED(typedef struct diag_subsys_efs_md5_sum_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint8_t path[0];
}) diag_subsys_efs_md5_sum_tx_t;

PACKED(typedef struct diag_subsys_efs_md5_sum_rx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	int32_t error;
	uint8_t hash[16];
}) diag_subsys_efs_md5_sum_rx_t;

/**
* Hotplug Format
*/
PACKED(typedef struct diag_subsys_efs_hotplug_format_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint8_t path[0];
}) diag_subsys_efs_hotplug_format_tx_t;

PACKED(typedef struct diag_subsys_efs_hotplug_format_rx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	int32_t error;
	uint8_t hash[16];
}) diag_subsys_efs_hotplug_format_rx_t;

/**
* Shred
*/
PACKED(typedef struct diag_subsys_efs_shred_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint8_t path[0];
}) diag_subsys_efs_shred_tx_t;

PACKED(typedef struct diag_subsys_efs_shred_rx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint16_t results;
	int32_t error;
}) diag_subsys_efs_shred_rx_t;

/**
* Hotplug Format
*/
PACKED(typedef struct diag_subsys_efs_hotplug_device_info_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint8_t path[0];
}) diag_subsys_efs_hotplug_device_info_tx_t;

PACKED(typedef struct diag_subsys_efs_hotplug_device_info_rx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	int32_t error;
	uint32_t manufacturerId;
	uint32_t oemId;
	uint32_t productRevision;
	uint8_t productName[16];
	uint8_t manufactureredDate[16];
}) diag_subsys_efs_hotplug_device_info_rx_t;

/**
* Sync
*/
PACKED(typedef struct diag_subsys_efs_sync_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint8_t path[0];
}) diag_subsys_efs_sync_tx_t;

PACKED(typedef struct diag_subsys_efs_sync_rx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint32_t token;
	int32_t error;
}) diag_subsys_efs_sync_rx_t;

/**
* Sync Get Status
*/
PACKED(typedef struct diag_subsys_efs_get_sync_status_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint32_t token;
	uint8_t path[0];
}) diag_subsys_efs_get_sync_status_tx_t;

PACKED(typedef struct diag_subsys_efs_get_sync_status_rx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint8_t status;
	int32_t error;
}) diag_subsys_efs_get_sync_status_rx_t;


/**
* Truncate64
*/
PACKED(typedef struct diag_subsys_efs_truncate64_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint32_t lengthLsb;
	uint32_t lengthMsb;
	uint8_t path[0];
}) diag_subsys_efs_truncate64_tx_t;

PACKED(typedef struct diag_subsys_efs_truncate64_rx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	int32_t error;
}) diag_subsys_efs_truncate64_rx_t;

/**
* Ftruncate64
*/
PACKED(typedef struct diag_subsys_efs_ftruncate64_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint32_t lengthLsb;
	uint32_t lengthMsb;
	int32_t fp;
}) diag_subsys_efs_ftruncate64_tx_t;

PACKED(typedef struct diag_subsys_efs_ftruncate64_rx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	int32_t error;
}) diag_subsys_efs_ftruncate64_rx_t;

/**
* Lseek64
*/
PACKED(typedef struct diag_subsys_efs_lseek64_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	int whence;
	uint32_t lengthLsb;
	uint32_t lengthMsb;
	int32_t fp;
}) diag_subsys_efs_lseek64_tx_t;

PACKED(typedef struct diag_subsys_efs_lseek64_rx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	int32_t error;
	uint32_t offsetLsb;
	uint32_t offsetMsb;
}) diag_subsys_efs_lseek64_rx_t;

/**
* Make golden copy
*/
PACKED(typedef struct diag_subsys_efs_make_golden_copy_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint8_t path[0];
}) diag_subsys_efs_make_golden_copy_tx_t;

PACKED(typedef struct diag_subsys_efs_make_golden_copy_rx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	int32_t error;
}) diag_subsys_efs_make_golden_copy_rx_t;

/**
* File System Image Open
*/
PACKED(typedef struct diag_subsys_efs_fs_image_open_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint8_t imageType; // fs_diag_filesystem_image_type
	uint8_t path[0];
}) diag_subsys_efs_fs_image_open_tx_t;

PACKED(typedef struct diag_subsys_efs_fs_image_open_rx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	int32_t handle;
	int32_t error;
}) diag_subsys_efs_fs_image_open_rx_t;

/**
* File System Image Read
*/
PACKED(typedef struct diag_subsys_efs_fs_image_read_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	int32_t handle; 
}) diag_subsys_efs_fs_image_read_tx_t;

PACKED(typedef struct diag_subsys_efs_fs_image_read_rx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	uint16_t length;
	int32_t error;
	uint8_t data[0];
}) diag_subsys_efs_fs_image_read_rx_t;

/**
* File System Image Close
*/
PACKED(typedef struct diag_subsys_efs_fs_image_close_tx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	int32_t handle;
}) diag_subsys_efs_fs_image_close_tx_t;

PACKED(typedef struct diag_subsys_efs_fs_image_close_rx_t{
	qcdm_subsys_header_t header;
	uint16_t sequence;
	int32_t error;
}) diag_subsys_efs_fs_image_close_rx_t;

#endif // _QC_DIAG_EFS_H
