/**
* LICENSE PLACEHOLDER
*
* @file dm_efs_manager.h
* @class OpenPST::DmEfsManager
* @package OpenPST
* @brief EFS2 management over diagnostic monitor
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#ifndef _QC_DM_EFS_MANAGER_H_
#define _QC_DM_EFS_MANAGER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <fcntl.h>
#include "qc/dm.h"
#include "qc/dm_efs.h"
#include "qc/dm_efs_node.h"
#include "serial/qcdm_serial.h"

namespace OpenPST {
    
    /**
    * @brief OpenPST::DmEfsManager
    */
    class DmEfsManager {
    
        protected:
            QcdmSerial& port;
            uint8_t buffer[DIAG_MAX_PACKET_SIZE];
        private:
            uint32_t subsystemCommand;
            uint32_t subsystemId;
        public:
            
            enum DmEfsOperationResult{
                kDmEfsIOError = -1,
                kDmEfsError = 0,
                kDmEfsSuccess = 1,
            }; 

            /**
            * @brief DmEfsManager - Constructor
            * @param QcdmSerial port
            */
            DmEfsManager(QcdmSerial& port);

            /**
            * @brief ~DmEfsManager - Deconstructor
            */
            ~DmEfsManager();

            /**
            * @brief setSubsystemCommand - Set the command code used to access the DM subsystem
            *
            * @param uint32_t command
            * @return void
            */
            void setSubsystemCommand(uint32_t command);

            /**
            * @brief setSubsystemCommand - Get the command code used to access the DM subsystem
            *
            * @return uint32_t
            */
            uint32_t getSubsystemCommand();

            /**
            * @brief setSubsystemId - Set the id used to access the EFS subsystem
            *
            * @param uint32_t command
            * @return void
            */
            void setSubsystemId(uint32_t code);

            /**
            * @brief getSubsystemId - Get the id used to access the EFS subsystem
            *
            * @return uint32_t
            */
            uint32_t getSubsystemId();

            /**
            * @brief hello - Send the hello and recieve configuration parameters
            *
            * @param QcdmEfsHelloResponse &response - Populated with the results from the device on success
            * @return int
            */
            int hello(QcdmEfsHelloResponse& response);

            /**
            * @brief getDeviceInfo - Get the device information
            *
            * @param QcdmEfsDeviceInfoResponse &response - Populated with the results from the device on success
            * @return int
            */
            int getDeviceInfo(QcdmEfsDeviceInfoResponse& response);

            /**
            * @brief query - Query for file system settings
            *
            * @param QcdmEfsQueryResponse &response - Populated with the results from the device on success
            * @return int
            */
            int query(QcdmEfsQueryResponse& response);

            /**
            * @brief open - Open a file for r/rw
            *
            * @param std::string path - full path to the file
            * @param int32_t mode - Mode to open file, standard open flags apply
            * @param int32_t& - fp of opened file on success
            *
            * @return int
            */
            int open(std::string path, int32_t flags, int32_t mode, int32_t& fp);

            /**
            * @brief close - Close a file
            *
            * @param int32_t - fp of file
            *
            * @return int
            */
            int close(int32_t fp);

            /**
            * @brief read - Read from a file to a data buffer
            *
            * @param int32_t - fp of file
            * @param size_t size - amount of data to read
            * @param uint32_t offset - offset in file to start reading at
            * @param std::vector<uint8_t>& - Data read
            *
            * @return int
            */
            int read(int32_t fp, size_t size, uint32_t offset, std::vector<uint8_t>& data);
            
            /**
            * @brief read - Read from a file to a file
            *
            * @param std::string - Remote path to read
            * @param std::string - Local path to write
            *
            * @return int
            */
            int read(std::string path, std::string outPath);

            /**
            * @brief write - Write to a file
            *
            * @param int32_t - fp of file
            * @param uint8_t* - data to write
            * @param size_t amount - amount of data to write
            * @paramuint32_t offset - offset in file to start writing at
            *
            * @return int
            */
            int write(int32_t fp, uint8_t* data, size_t amount, uint32_t offset);
            
            /**
            * @brief symlink - Create a symlink
            *
            * @param std::string path - Full path to file
            * @param std::string linkPath - Full path to the link
            *
            * @return int
            */
            int symlink(std::string path, std::string linkPath);

            /**
            * @brief readSimlink - Read a symlink path
            *
            * @param std::string path - Full path to file
            * @param std::string& out - String with the links real location
            *
            * @return int
            */
            int readSimlink(std::string path, std::string& out);

            /**
            * @brief unlink - Delete a file
            *
            * @param std::string path - Full path to file
            *
            * @return int
            */
            int unlink(std::string path);

            /**
            * @brief mkdir - Create a directory
            *
            * @param std::string path - Full path to directory to create
            * @param int16_t mode - Mode to create directory (chmod)
            *
            * @return int
            */
            int mkdir(std::string path, int16_t mode);
            
            /**
            * @brief rmdir - Delete a directory
            *
            * @param std::string path - Full path to directory
            *
            * @return int
            */
            int rmdir(std::string path);

            /**
            * @brief openDir - Open a directory for reading
            *
            * @param std::string path - Full path to file
            * @param uint32_t& dp - Directory pointer to opened directory
            *
            * @return int
            */
            int openDir(std::string path, uint32_t& dp);
                
            /**
            * @brief readDir - Read a directory contents, optionally recursively
            *
            * @param std:;string path - Full path to file
            * @param std::vector<DmEfsNode>& - DmEfsNode vector to populate the tree
            * @param bool - read recursively
            *
            * @return int
            */
            int readDir(std::string path, std::vector<DmEfsNode>& contents, bool recursive = false);

            /**
            * @brief readDir - Read a directory contents, not recursive
            *
            * @param uint32_t dp - dp from openDir operation
            * @param std::vector<DmEfsNode>& - DmEfsNode vector to populate the tree
            *
            * @return int
            */
            int readDir(uint32_t dp, std::vector<DmEfsNode>& contents);

            /**
            * @brief closeDir - Close a open directory.
            *
            * @param uint32_t dp - Directory pointer to opened directory
            */
            int closeDir(uint32_t dp);

            /**
            * @brief rename - Rename a file or directory
            *
            * @param std::string path
            * @param std::string newPath
            *
            * @return int
            */
            int rename(std::string path, std::string newPath);
            
            /**
            * @brief stat - Stat a file or directory by path
            *
            * @param std::string path
            * @param QcdmEfsStatResponse& response
            *
            * @return int
            */
            int stat(std::string path, QcdmEfsStatResponse& response);
            
            /**
            * @brief lstat - Stat a link by path
            *
            * @param std::string path
            * @param QcdmEfsLstatResponse& response
            *
            * @return int
            */
            int lstat(std::string path, QcdmEfsLstatResponse& response);
            
            /**
            * @brief fstat - Stat a file by fp
            *
            * @param std::string path
            * @param QcdmEfsLstatResponse& response
            *
            * @return int
            */
            int fstat(int32_t fp, QcdmEfsFstatResponse& response);
            
            /**
            * @brief chmod - Change file permissions
            *
            * @param std::string path
            * @param int16_t mode
            *
            * @return int
            */
            int chmod(std::string path, int16_t mode);
            
            /**
            * @brief statfs - Stat the filesystem
            *
            * @param std::string path
            * @param QcdmEfsStatfsResponse& response
            *
            * @return int
            */
            int statfs(std::string path, QcdmEfsStatfsResponse& response);
            
            /**
            * @brief access - Check access permissions for a path
            *
            * @param std::string path
            * @param char checkPermissionBits
            *
            * @return int
            */
            int access(std::string path, char checkPermissionBits);

            /**
            * @brief getFactoryImage - Get EFS factory image
            *
            * @param std::ofstream& out
            *
            * @return int
            */
            int getFactoryImage(std::ofstream& out);
            
            /**
            * @brief factoryImageStart - Start transfer of factory image
            *
            * @return int
            */
            int factoryImageStart();

            /**
            * @brief factoryImageStart - Start transfer of factory image
            *
            * @return int
            */
            int factoryImageRead(std::vector<uint8_t>& data);

            /**
            * @brief factoryImageEnd - End transfer of factory image
            *
            * @return int
            */
            int factoryImageEnd();

            /**
            * @brief factoryImagePrepare - Prepare for transfer of factory image
            *
            * @return int
            */
            int factoryImagePrepare();

            /**
            * @brief chown - Change file or directory ownership
            *
            * @param std::string path
            * @param int32_t uid
            * @param int32_t gid
            *
            * @return int
            */
            int chown(std::string path, int32_t uid, int32_t gid);

            /**
            * @brief setQuota - Limit the size of a directory for a group
            *
            * @param std::string path
            * @param int32_t gid
            * @param size_t size
            *
            * @return int
            */
            int setQuota(std::string path, int32_t gid, size_t size);
            
            /**
            * @brief getGroupInfo - Get information about a group for a path
            *
            * @param std::string path
            * @param int32_t gid
            *
            * @return int
            */
            int getGroupInfo(std::string path, int32_t gid);
            
            /**
            * @brief deltree - Delete a directory tree
            *
            * @param std::string path
            * @param int32_t sequence
            *
            * @return int
            */
            int deltree(std::string path, int32_t sequence = 1);
            
            /*
            int startBenchmarkTest();
            int getBenchmarkResults();
            int initBenchmark();
            int resetBenchmark();
            int setReservation();
            int put();
            int get();
            */
            
            /**
            * @brief truncate - Truncate a file by path
            *
            * @param std::string path
            * @param size_t amount
            * @param int32_t sequence
            *
            * @return int
            */
            int truncate(std::string path, size_t amount, int32_t sequence = 1);
            
            /**
            * @brief truncate - Truncate a file by fp
            *
            * @param int32_t fp
            * @param size_t amount
            * @param int32_t sequence
            *
            * @return int
            */
            int ftruncate(int32_t fp, size_t amount, int32_t sequence = 1);
            
            /**
            * @brief statfsV2 - STATFS v2
            *
            * @param QcdmEfsStatfsV2Response& response
            * @param int32_t sequence
            *
            * @return int
            */
            int statfsV2(QcdmEfsStatfsV2Response& response, int32_t sequence = 1);
            
            /**
            * @brief md5sum - Get MD5 Sum for a file.
            *
            * @param std::string path
            * @param int32_t sequence
            *
            * @return int
            */
            int md5sum(std::string path, std::string& hash, int32_t sequence = 1);
            
            /**
            * @brief formatHotplugDevice - Format a hot plug device
            *
            * @param std::string path
            * @param int32_t sequence
            *
            * @return int
            */
            int formatHotplugDevice(std::string path, int32_t sequence = 1);
            
            /**
            * @brief shred - Erase obsolete data
            *
            * @param std::string path
            * @param int32_t sequence
            *
            * @return int
            */
            int shred(std::string path, int32_t sequence = 1);
            

            /*
            int setIdleDevEventTime();
            int getHotplugDeviceInfo(std::string path, int32_t sequence = 1);
			*/


			QcdmEfsSyncResponse syncNoWait(std::string path, uint16_t sequence = 1);

			QcdmEfsGetSyncStatusResponse getSyncStatus(uint32_t token, uint16_t sequence = 1);
            
			/*int truncate64();
            int ftruncate64(int32_t fp);
            int lseek64();
            */
            int makeGoldenCopy(std::string path, int32_t sequence = 1);

            int openFilesystemImage(std::string path, uint8_t imageType, int32_t& handle, int32_t sequence = 1);
            int readFilesystemImage(int32_t handle, std::vector<uint8_t>& data, int32_t sequence = 1);
            int closeFilesystemImage(int32_t handle, int32_t sequence = 1);

        
        private:
            
            /**
            * @brief getHeader - Used internally to quickly assemble a packet header
            * 
            * @param uint16_t - The command to create the header data for
            *
            * @return QcdmSubsysHeader
            */
            QcdmSubsysHeader getHeader(uint16_t command);
            
            /**
            * @brief sendCommand - Same as sendCommand(uint16_t command) but does not construct a packet,
            *                       just writes the raw data and validates the response
            *
            *
            * @param uint16_t - The basic packet command to send as well as expected response command
            * @param uint8_t* - Opaque data pointer to validate against
            * @param size_t - Size of the data
            *
            * @return int
            */
            int sendCommand(uint16_t command);
            
            /**
            * @brief sendCommand - Same as sendCommand(uint16_t command) but does not construct a packet, 
            *                       just writes the raw data (hdlc encoded for you) and validates the response
            *
            *
            * @param uint16_t - The expected response command
            * @param uint8_t* - Opaque data pointer to validate against
            * @param size_t - Size of the data
            *
            * @return int
            */
            int sendCommand(uint16_t command, uint8_t* packet, size_t packetSize);

            /**
            * @brief isValidResponse - used internally to validate responses
            *
            *
            * @param uint16_t - The expected response command
            * @param uint8_t* - Opaque data pointer to validate against
            * @param size_t - Size of the data
            *
            * @return bool
            */
            bool isValidResponse(uint16_t command, uint8_t* data, size_t size);
    };




    

}

#endif // _QC_DM_EFS_MANAGER_H_