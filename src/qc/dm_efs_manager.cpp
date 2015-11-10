
/**
* LICENSE PLACEHOLDER
*
* @file dm_efs_manager.cpp
* @class OpenPST::DmEfsManager
* @package OpenPST
* @brief EFS2 management over diagnostic monitor
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "dm_efs_manager.h"

using namespace OpenPST;

/**
* @brief DmEfsManager - Constructor
* @param QcdmSerial port
*/
DmEfsManager::DmEfsManager(QcdmSerial& port) : 
    port(port),
    subsystemCommand(DIAG_SUBSYS_CMD_F),
    subsystemId(DIAG_SUBSYS_FS)
{

}
/**
* @brief ~DmEfsManager - Deconstructor
*/
DmEfsManager::~DmEfsManager()
{

}
/**
* @brief setSubsystemCommand - Set the command code used to access the DM subsystem
*
* @param uint32_t command
* @return void
*/
void DmEfsManager::setSubsystemCommand(uint32_t command)
{
    subsystemCommand = command;
}

/**
* @brief setSubsystemCommand - Get the command code used to access the DM subsystem
*
* @return uint32_t
*/
uint32_t DmEfsManager::getSubsystemCommand()
{
    return subsystemCommand;
}

/**
* @brief setSubsystemId - Set the id used to access the EFS subsystem
*
* @param uint32_t command
* @return void
*/
void DmEfsManager::setSubsystemId(uint32_t id)
{
    subsystemId = id;
}

/**
* @brief getSubsystemId - Get the id used to access the EFS subsystem
*
* @return uint32_t
*/
uint32_t DmEfsManager::getSubsystemId()
{
    return subsystemId;
}

/**
* @brief DmEfsManager::hello - Send the hello and receive configuration parameters
*
* @param QcdmEfsHelloResponse &response - Populated with the results from the device on success
* @return int
*/
int DmEfsManager::hello(QcdmEfsHelloResponse& response)
{

    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    QcdmEfsHelloRequest packet = {
        getHeader(DIAG_EFS_HELLO),
        DIAG_EFS_DEFAULT_WINDOW_SIZE,
        DIAG_EFS_DEFAULT_WINDOW_BYTE_SIZE,
        DIAG_EFS_DEFAULT_WINDOW_SIZE,
        DIAG_EFS_DEFAULT_WINDOW_BYTE_SIZE,
        DIAG_EFS_DEFAULT_WINDOW_SIZE,
        DIAG_EFS_DEFAULT_WINDOW_BYTE_SIZE,
        DIAG_EFS_VERSION,
        DIAG_EFS_MIN_VERSION,
        DIAG_EFS_MAX_VERSION,
        0x000000
    };
    
    int commandResult = sendCommand(DIAG_EFS_HELLO, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    std::memcpy(&response, buffer, sizeof(response));

    return kDmEfsSuccess;
}

/**
* @brief getDeviceInfo - Get the device information
*
* @param QcdmEfsDeviceInfoResponse &response - Populated with the results from the device on success
* @return int
*/
int DmEfsManager::getDeviceInfo(QcdmEfsDeviceInfoResponse& response)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }
    
    int commandResult = sendCommand(DIAG_EFS_DEV_INFO);

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsDeviceInfoResponse* resp = (QcdmEfsDeviceInfoResponse*)buffer;

    std::memcpy(&response, buffer, sizeof(response));

    return kDmEfsSuccess;
}

/**
* @brief query - Query for file system settings
*
* @param QcdmEfsQueryResponse &response - Populated with the results from the device on success
* @return int
*/
int DmEfsManager::query(QcdmEfsQueryResponse& response)
{
    if (!port.isOpen()) {       
        return kDmEfsIOError;
    }

    int commandResult = sendCommand(DIAG_EFS_QUERY);

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsQueryResponse* resp = (QcdmEfsQueryResponse*)buffer;

    std::memcpy(&response, buffer, sizeof(response));

    return kDmEfsSuccess; 
}

/**
* @brief DmEfsManager::open - Open a file for r/rw
*
* @param std::string path - full path to the file
* @param int32_t mode - Mode to open file, standard open flags apply
* @param int32_t& - fp of opened file on success
*
* @return int
*/
int DmEfsManager::open(std::string path, int32_t flags, int32_t mode, int32_t& fp)
{
    
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsOpenFileRequest) + path.size();
    QcdmEfsOpenFileRequest* packet = (QcdmEfsOpenFileRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_OPEN);
    packet->flags = flags;
    packet->mode = mode;    
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_OPEN, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }
    
    QcdmEfsOpenFileResponse* response = (QcdmEfsOpenFileResponse*)buffer;

    if (response->error) {
        return kDmEfsError;
    }

    fp = response->fp;

    return kDmEfsSuccess;
}

/**
* @brief DmEfsManager::close - Close a file
*
* @param int32_t - fp of file
*
* @return int
*/
int DmEfsManager::close(int32_t fp)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    QcdmEfsCloseFileRequest packet = {
        getHeader(DIAG_EFS_CLOSE),
        fp
    };

    int commandResult = sendCommand(DIAG_EFS_CLOSE, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsCloseFileReponse* response = (QcdmEfsCloseFileReponse*)buffer;

    if (response->error) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

/**
* @brief DmEfsManager::read - Read from a file to a data buffer
*
* @param int32_t - fp of file
* @param size_t size - amount of data to read
* @param uint32_t offset - offset in file to start reading at
* @param std::vector<uint8_t>& - Data read
*
* @return int
*/
int DmEfsManager::read(int32_t fp, size_t size, uint32_t offset, std::vector<uint8_t>& data)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    QcdmEfsReadFileRequest packet = {
        getHeader(DIAG_EFS_READ),
        fp,
        size,
        offset
    };

    if (!port.write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet))) {
        return kDmEfsIOError;
    }
    
    size_t rxSize = port.read(buffer, DIAG_MAX_PACKET_SIZE);

    if (!rxSize) {
        return kDmEfsIOError;
    }
    
    if (!isValidResponse(DIAG_EFS_READ, buffer, rxSize)) {
        return kDmEfsError;
    }
    
    QcdmEfsReadFileResponse* response = (QcdmEfsReadFileResponse*)buffer;
    
    hexdump(response->data, rxSize - sizeof(QcdmEfsReadFileResponse));

    data.insert(data.end(), response->data, response->data + (rxSize - sizeof(QcdmEfsReadFileResponse)));
    
    return kDmEfsSuccess;
}

/**
* @brief DmEfsManager::read - Read from a file to a file
*
* @param std::string - Remote path to read
* @param std::string - Local path to write
*
* @return int
*/
int DmEfsManager::read(std::string path, std::string outPath)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    int32_t fp;
    
    int openResult = open(path, O_RDONLY, 0x00, fp);

    if (openResult != kDmEfsSuccess) {
        return openResult;
    }

    QcdmEfsFstatResponse fileInfo;

    int fstatResult = fstat(fp, fileInfo);

    if (fstatResult != kDmEfsSuccess) {
        return fstatResult;
    }

    if (fileInfo.size <= 0) {
        return kDmEfsError;
    }

    std::vector<uint8_t> data;

    int readResult = read(fp, fileInfo.size, 0x00, data);

    if (readResult != kDmEfsSuccess) {
        close(fp);
        return readResult;
    }

    close(fp);

    std::ofstream out(outPath.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

    if (!out.is_open()) {
        return kDmEfsIOError;
    }

    out.write(reinterpret_cast<char*>(&data[0]), data.size());

    return kDmEfsSuccess;
}

/**
* @brief DmEfsManager::write - Write to a file
*
* @param int32_t - fp of file
* @param uint8_t* - data to write
* @param size_t amount - amount of data to write
* @paramuint32_t offset - offset in file to start writing at
*
* @return int
*/
int DmEfsManager::write(int32_t fp, uint8_t* data, size_t amount, uint32_t offset)
{
    return 0;
}

/**
* @brief DmEfsManager::symlink - Create a symlink
*
* @param std::string path - Full path to file
* @param std::string newpath - Full path to the link
*
* @return int
*/
int DmEfsManager::symlink(std::string path, std::string linkPath)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsCreateLinkRequest) + path.size() + linkPath.size();
    QcdmEfsCreateLinkRequest* packet = (QcdmEfsCreateLinkRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_SYMLINK);
    std::memcpy(packet->path, path.c_str(), path.size());
    std::memcpy((packet->path + path.size() + 1), linkPath.c_str(), linkPath.size());

    int commandResult = sendCommand(DIAG_EFS_SYMLINK, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsCreateLinkResponse* response = (QcdmEfsCreateLinkResponse*)buffer;

    if (response->error) {
        LOGE("Error creating link from %s to %s\n", path.c_str(), linkPath.c_str());
        return kDmEfsError;
    }

    return kDmEfsSuccess; 
}

/**
* @brief DmEfsManager::readSimlink - Read a symlink path
*
* @param std::string path - Full path to file
* @param std::string& out - String with the links real location
*
* @return int
*/
int DmEfsManager::readSimlink(std::string path, std::string& out)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsReadLinkRequest) + path.size();
    QcdmEfsReadLinkRequest* packet = (QcdmEfsReadLinkRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_READLINK);
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_READLINK, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }
    
    QcdmEfsReadLinkResponse* response = (QcdmEfsReadLinkResponse*)buffer;

    if (response->error) {
        return kDmEfsError;
    }

    out = response->path;

    return kDmEfsSuccess;
}

/**
* @brief DmEfsManager::unlink - Delete a file
*
* @param std::string path - Full path to file
*
* @return int
*/
int DmEfsManager::unlink(std::string path)
{
    
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsUnlinkRequest) + path.size();
    QcdmEfsUnlinkRequest* packet = (QcdmEfsUnlinkRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_UNLINK);
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_UNLINK, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsUnlinkResponse* response = (QcdmEfsUnlinkResponse*)buffer;

    if (response->error) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

/**
* @brief DmEfsManager::mkdir - Create a directory
*
* @param std::string path - Full path to directory to create
* @param int16_t mode - Mode to create directory (chmod)
*
* @return int
*/
int DmEfsManager::mkdir(std::string path, int16_t mode)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsMkdirRequest) + path.size();
    QcdmEfsMkdirRequest* packet = (QcdmEfsMkdirRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_MKDIR);
    packet->mode = mode;
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_MKDIR, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsMkdirResponse* response = (QcdmEfsMkdirResponse*)buffer;

    if (response->error) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

/**
* @brief DmEfsManager::rmdir - Delete a directory
*
* @param std::string path - Full path to directory
*
* @return int
*/
int DmEfsManager::rmdir(std::string path)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsRmdirRequest) + path.size();
    QcdmEfsRmdirRequest* packet = (QcdmEfsRmdirRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_RMDIR);
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_RMDIR, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsMkdirResponse* response = (QcdmEfsMkdirResponse*)buffer;

    if (response->error) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

/**
* @brief DmEfsManager::openDir - Open a directory for reading
*
* @param std::string path - Full path to file
* @param uint32_t& dp - Directory pointer to opened directory
*
* @return int
*/
int DmEfsManager::openDir(std::string path, uint32_t& dp)
{

    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsOpenDirRequest) + path.size();
    QcdmEfsOpenDirRequest* packet = (QcdmEfsOpenDirRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_OPENDIR);
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_OPENDIR, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsOpenDirResponse* response = (QcdmEfsOpenDirResponse*)buffer;

    if (response->error) {
        LOGE("Error opening directory %s - Error: %08X\n", path.c_str(), response->error);
        return kDmEfsError;
    }

    dp = response->dp;

    return kDmEfsSuccess; 
}

/**
* @brief DmEfsManager::readDir - Read a directory contents, optionally recursively
*
* @param std:;string path - Full path to file
* @param std::vector<DmEfsNode>& - DmEfsNode vector to populate the tree
* @param bool - read recursively
*
* @return int
*/
int DmEfsManager::readDir(std::string path, std::vector<DmEfsNode>& contents, bool recursive)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t txSize, rxSize;
    uint32_t dp;

    int openResult = openResult = openDir(path, dp);

    if (openResult != kDmEfsSuccess) {
        return openResult;
    }

    QcdmEfsReadDirRequest packet = {};
    packet.header = getHeader(DIAG_EFS_READDIR);
    packet.dp = dp;
    packet.sequenceNumber = 1;

    do {

        int commandResult = sendCommand(DIAG_EFS_READDIR, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

        if (commandResult != kDmEfsSuccess) {
            if (closeDir(dp) != kDmEfsSuccess) {
                LOGI("Error closing directory %s\n", path.c_str());
            }
            return commandResult;
        }

        QcdmEfsReadDirResponse* response = (QcdmEfsReadDirResponse*)buffer;

        if (response->sequenceNumber != packet.sequenceNumber) {
            LOGI("Invalid readDir Sequence Received\n");
        }

        if (response->error) {
            LOGE("Error Reading %s/%s. Error: %08X\n", path.c_str(), response->name, response->error);
        }

        if (strlen(response->name) == 0) {
            break; // end
        }
    
        DmEfsNode node(path, response);

        contents.insert(contents.end(), node);

        packet.sequenceNumber++;

    } while (true);

    if (closeDir(dp) != kDmEfsSuccess) {
        LOGI("Error closing directory %s\n", path.c_str());
    }

    if (recursive) {
        for (auto &node : contents) {
            if (node.isDir()) {

                std::string checkPath = path;

                checkPath.append(node.name).append("/");
        
                if (readDir(checkPath, node.children, recursive) != kDmEfsSuccess) {
                    LOGE("Error on recursive readDir for %s\n", checkPath.c_str());
                    node.error = 0x000001;
                }
            }
        }
    }

    return kDmEfsSuccess;
}

/**
* @brief DmEfsManager::readDir - Read a directory contents
*
* @param uint32_t dp - dp from openDir operation
* @param std::vector<DmEfsNode>& - DmEfsNode vector to populate the tree
*
* @return int
*/
int DmEfsManager::readDir(uint32_t dp, std::vector<DmEfsNode>& contents)
{

    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t txSize, rxSize;

    QcdmEfsReadDirRequest packet = {};
    packet.header = getHeader(DIAG_EFS_READDIR);
    packet.dp = dp;
    packet.sequenceNumber = 1;
            
    do {
        int commandResult = sendCommand(DIAG_EFS_READDIR, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

        if (commandResult != kDmEfsSuccess) {
            if (closeDir(dp) != kDmEfsSuccess) {
                LOGI("Error closing directory dp %d\n", dp);
            }
            return commandResult;
        }
        
        QcdmEfsReadDirResponse* response = (QcdmEfsReadDirResponse*)buffer;

        if (response->sequenceNumber != packet.sequenceNumber) {
            LOGI("Invalid sequence received in read directory response\n");
        }

        if (response->error) {
            LOGE("Error reading directory fp %s. Error %08X\n", response->name, response->error);
        }

        if (strlen(response->name) == 0) {
            break; // end
        }

        DmEfsNode node("", response);

        contents.insert(contents.end(), node);

        packet.sequenceNumber++;

    } while (true);

    return kDmEfsSuccess;
}

/**
* @brief DmEfsManager::closeDir - Close a open directory.
*
* @param uint32_t dp - Directory pointer to opened directory
*/
int DmEfsManager::closeDir(uint32_t dp)
{

    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    QcdmEfsCloseDirRequest packet = {
        getHeader(DIAG_EFS_CLOSEDIR),
        dp
    };

    int commandResult = sendCommand(DIAG_EFS_CLOSEDIR, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsCloseDirResponse* response = (QcdmEfsCloseDirResponse*)buffer;

    if (response->error) {
        LOGE("Error closing directory %d - Error: %08X\n", dp, response->error);
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

/**
* @brief DmEfsManager::rename - Rename a file or directory
*
* @param std::string path
* @param std::string newPath
*
* @return int
*/
int DmEfsManager::rename(std::string path, std::string newPath)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsRenameRequest) + path.size() + newPath.size();
    QcdmEfsRenameRequest* packet = (QcdmEfsRenameRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_RENAME);
    std::memcpy(packet->path, path.c_str(), path.size());
    std::memcpy((packet->path + path.size() + 1), newPath.c_str(), newPath.size());

    int commandResult = sendCommand(DIAG_EFS_RENAME, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsAccessResponse* response = (QcdmEfsAccessResponse*)buffer;

    if (response->error) {
        LOGE("Error renaming %s to %s\n", path.c_str(), newPath.c_str());
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

/**
* @brief DmEfsManager::stat - Stat a file or directory by path
*
* @param std::string path
* @param QcdmEfsStatResponse& response
*
* @return int
*/
int DmEfsManager::stat(std::string path, QcdmEfsStatResponse& response)
{
if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsStatRequest) + path.size();
    QcdmEfsStatRequest* packet = (QcdmEfsStatRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_STAT);
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_STAT, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsStatResponse* resp = (QcdmEfsStatResponse*)buffer;
    
    if (resp->error) {      
        return kDmEfsError;
    }

    std::memcpy(&response, buffer, sizeof(response));

    return kDmEfsSuccess;
}

/**
* @brief lstat - Stat a link by path
*
* @param std::string path
* @param QcdmEfsLstatResponse& response
*
* @return int
*/
int DmEfsManager::lstat(std::string path, QcdmEfsLstatResponse& response)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsLstatRequest) + path.size();
    QcdmEfsLstatRequest* packet = (QcdmEfsLstatRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_LSTAT);
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_LSTAT, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsLstatResponse* resp = (QcdmEfsLstatResponse*)buffer;
    
    if (resp->error) {      
        return kDmEfsError;
    }

    std::memcpy(&response, buffer, sizeof(response));

    return kDmEfsSuccess;
}

/**
* @brief fstat - Stat a file by fp
*
* @param std::string path
* @param QcdmEfsLstatResponse& response
*
* @return int
*/
int DmEfsManager::fstat(int32_t fp, QcdmEfsFstatResponse& response)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    QcdmEfsFstatRequest packet = {};
    packet.header = getHeader(DIAG_EFS_FSTAT);
    packet.fp = fp;
    
    int commandResult = sendCommand(DIAG_EFS_FSTAT, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsFstatResponse* resp = (QcdmEfsFstatResponse*)buffer;

    if (resp->error) {
        return kDmEfsError;
    }

    std::memcpy(&response, buffer, sizeof(response));

    return kDmEfsSuccess;
}

/**
* @brief chmod - Change file permissions
*
* @param std::string path
* @param int16_t mode
*
* @return int
*/
int DmEfsManager::chmod(std::string path, int16_t mode)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsChmodRequest) + path.size();
    QcdmEfsChmodRequest* packet = (QcdmEfsChmodRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_CHMOD);
    packet->mode = mode;
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_CHMOD, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsChmodResponse* response = (QcdmEfsChmodResponse*)buffer;

    if (response->error) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

/**
* @brief statfs - Stat the filesystem
*
* @param std::string path
* @param QcdmEfsStatfsResponse& response
*
* @return int
*/
int DmEfsManager::statfs(std::string path, QcdmEfsStatfsResponse& response)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsStatfsRequest) + path.size();
    QcdmEfsStatfsRequest* packet = (QcdmEfsStatfsRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_STATFS);
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_STATFS, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }
    
    QcdmEfsStatfsRequest* resp = (QcdmEfsStatfsRequest*)buffer;

    std::memcpy(&response, buffer, sizeof(response));

    return kDmEfsSuccess;
}

/**
* @brief access - Check access permissions for a path
*
* @param std::string path
* @param char checkPermissionBits
*
* @return int
*/
int DmEfsManager::access(std::string path, char checkPermissionBits)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsAccessRequest) + path.size();
    QcdmEfsAccessRequest* packet = (QcdmEfsAccessRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_ACCESS);
    packet->permissionBits = checkPermissionBits;
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_ACCESS, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsAccessResponse* response = (QcdmEfsAccessResponse*)buffer;

    if (response->error) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

/**
* @brief getFactoryImage - Get EFS factory image
*
* @param std::ofstream& out
*
* @return int
*/
int DmEfsManager::getFactoryImage(std::ofstream& out)
{
    return  kDmEfsError;
}

/**
* @brief factoryImageStart - Start transfer of factory image
*
* @return int
*/
int DmEfsManager::factoryImageStart()
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    int commandResult = sendCommand(DIAG_EFS_FACT_IMAGE_START);

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsFactoryImageStartResponse* response = (QcdmEfsFactoryImageStartResponse*)buffer;

    if (response->error) {
        return kDmEfsError;
    }
    return kDmEfsSuccess;
}

/**
* @brief factoryImageStart - Start transfer of factory image
*
* @return int
*/
int DmEfsManager::factoryImageRead(std::vector<uint8_t>& data)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    QcdmEfsFactoryImageReadRequest packet = {};
    packet.header = getHeader(DIAG_EFS_FACT_IMAGE_START);
    packet.streamState = 0x00;
    packet.infoClusterSent = 0x00;
    packet.clusterMapSequence = 0x00;
    packet.clusterDataSequence = 0x00;

    int commandResult = sendCommand(DIAG_EFS_FACT_IMAGE_START, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsFactoryImageReadResponse* response = (QcdmEfsFactoryImageReadResponse*)buffer;

    if (response->error) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

/**
* @brief factoryImageEnd - End transfer of factory image
*
* @return int
*/
int DmEfsManager::factoryImageEnd()
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    int commandResult = sendCommand(DIAG_EFS_FACT_IMAGE_START);

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsFactoryImageEndResponse* response = (QcdmEfsFactoryImageEndResponse*)buffer;

    if (response->error) {
        return kDmEfsError;
    }
    return kDmEfsSuccess;
}

/**
* @brief factoryImagePrepare - Prepare for transfer of factory image
*
* @return int
*/
int DmEfsManager::factoryImagePrepare()
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    int commandResult = sendCommand(DIAG_EFS_PREP_FACT_IMAGE);

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsFactoryImagePrepareResponse* response = (QcdmEfsFactoryImagePrepareResponse*)buffer;

    if (response->error) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

/**
* @brief chown - Change file or directory ownership
*
* @param std::string path
* @param int32_t uid
* @param int32_t gid
*
* @return int
*/
int DmEfsManager::chown(std::string path, int32_t uid, int32_t gid)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsChownRequest) + path.size();
    QcdmEfsChownRequest* packet = (QcdmEfsChownRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_CHOWN);
    packet->uid = uid;
    packet->gid = gid;
    std::memcpy(packet->path, path.c_str(), path.size());
    
    int commandResult = sendCommand(DIAG_EFS_CHOWN, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsChownResponse* response = (QcdmEfsChownResponse*)buffer;

    if (response->error) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

/**
* @brief setQuota - Limit the size of a directory for a group
*
* @param std::string path
* @param int32_t gid
* @param size_t size
*
* @return int
*/
int DmEfsManager::setQuota(std::string path, int32_t gid, size_t size)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsSetQuotaRequest) + path.size();
    QcdmEfsSetQuotaRequest* packet = (QcdmEfsSetQuotaRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_SET_QUOTA);
    packet->gid = gid;
    packet->amount = size;
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_SET_QUOTA, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsSetQuotaResponse* response = (QcdmEfsSetQuotaResponse*)buffer;

    if (response->error) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

/**
* @brief getGroupInfo - Get information about a group for a path
*
* @param std::string path
* @param int32_t gid
*
* @return int
*/
int DmEfsManager::getGroupInfo(std::string path, int32_t gid)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsGetGroupInfoRequest) + path.size();
    QcdmEfsGetGroupInfoRequest* packet = (QcdmEfsGetGroupInfoRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_GET_GROUP_INFO);
    packet->gid = gid;
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_GET_GROUP_INFO, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsGetGroupInfoResponse* response = (QcdmEfsGetGroupInfoResponse*)buffer;

    if (response->error) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

/**
* @brief deltree - Delete a directory tree
*
* @param std::string path
* @param int32_t sequence
*
* @return int
*/
int DmEfsManager::deltree(std::string path, int sequence)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsDeltreeRequest) + path.size() + 1;
    QcdmEfsDeltreeRequest* packet = (QcdmEfsDeltreeRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_DELTREE);
    packet->sequence = sequence;
    std::memcpy(packet->path, path.c_str(), path.size()+1);

    int commandResult = sendCommand(DIAG_EFS_DELTREE, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsDeltreeResponse* response = (QcdmEfsDeltreeResponse*)buffer;

    if (response->error || response->sequence != sequence) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

/**
* @brief truncate - Truncate a file by path
*
* @param std::string path
* @param size_t amount
* @param int32_t sequence
*
* @return int
*/
int DmEfsManager::truncate(std::string path, size_t amount, int32_t sequence)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsTruncateRequest) + path.size();
    QcdmEfsTruncateRequest* packet = (QcdmEfsTruncateRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_TRUNCATE);
    packet->sequence = sequence;
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_TRUNCATE, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsTruncateResponse* response = (QcdmEfsTruncateResponse*)buffer;

    if (response->error || response->sequence != sequence) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

/**
* @brief ftruncate - Truncate a file by fp
*
* @param int32_t fp
* @param size_t amount
* @param int32_t sequence
*
* @return int
*/
int DmEfsManager::ftruncate(int32_t fp, size_t amount, int32_t sequence)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    QcdmEfsFtncateRequest packet = {};

    packet.header = getHeader(DIAG_EFS_FTRUNCATE);
    packet.sequence = sequence;
    packet.fp = fp;

    int commandResult = sendCommand(DIAG_EFS_FTRUNCATE, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsFtncateResponse* response = (QcdmEfsFtncateResponse*)buffer;

    if (response->error || response->sequence != sequence) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}


int DmEfsManager::statfsV2(QcdmEfsStatfsV2Response& response, int32_t sequence)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    QcdmEfsStatfsV2Request packet = {
        getHeader(DIAG_EFS_STATVFS_V2),
        sequence
    };

    int commandResult = sendCommand(DIAG_EFS_STATVFS_V2, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsStatfsV2Response* resp = (QcdmEfsStatfsV2Response*)buffer;

    if (resp->error || resp->sequence != sequence) {
        return kDmEfsError;
    }

    std::memcpy(&response, buffer, sizeof(response));

    return kDmEfsSuccess;
}

/**
* @brief md5sum - Get MD5 Sum for a file.
*
* @param std::string path
* @param int32_t sequence
*
* @return int
*/
int DmEfsManager::md5sum(std::string path, std::string& hash, int32_t sequence)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsMd5SumRequest) + path.size();
    QcdmEfsMd5SumRequest* packet = (QcdmEfsMd5SumRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_MD5SUM);
    packet->sequence = sequence;
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_MD5SUM, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsMd5SumResponse* response = (QcdmEfsMd5SumResponse*)buffer;

    if (response->error || response->sequence != sequence) {
        return kDmEfsError;
    }

    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0'); 
    for (int i = 0; i < 16; i++) {
        ss << (int)response->hash[i];
    }
    
    std::string tmp = ss.str();
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);

    hash.clear();
    hash.append(ss.str());

    return kDmEfsSuccess;
}

/**
* @brief formatHotplugDevice - Format a hot plug device
*
* @param std::string path
* @param int32_t sequence
*
* @return int
*/
int DmEfsManager::formatHotplugDevice(std::string path, int32_t sequence)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsHotplugFormatRequest) + path.size();
    QcdmEfsHotplugFormatRequest* packet = (QcdmEfsHotplugFormatRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_HOTPLUG_FORMAT);
    packet->sequence = sequence;
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_HOTPLUG_FORMAT, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }
    
    QcdmEfsHotplugFormatResponse* response = (QcdmEfsHotplugFormatResponse*)buffer;

    if (response->error || response->sequence != sequence) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

/**
* @brief shred - Erase obsolete data
*
* @param std::string path
* @param int32_t sequence
*
* @return int
*/
int DmEfsManager::shred(std::string path, int32_t sequence)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsShredRequest) + path.size();
    QcdmEfsShredRequest* packet = (QcdmEfsShredRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_SHRED);
    packet->sequence = sequence;
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_SHRED, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsShredResponse* response = (QcdmEfsShredResponse*)buffer;

    if (response->error || response->sequence != sequence) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}


QcdmEfsSyncResponse DmEfsManager::syncNoWait(std::string path, uint16_t sequence)
{
	QcdmEfsSyncResponse ret = {};
	size_t packetSize = sizeof(QcdmEfsSyncRequest) + path.size() + 1;
	QcdmEfsSyncRequest* packet = (QcdmEfsSyncRequest*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_SYNC_NO_WAIT);
	packet->sequence = sequence;
	std::memcpy(packet->path, path.c_str(), path.size() + 1);

	int commandResult = sendCommand(packet->header.subsysCommand, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		throw QcdmResponseError("Command Error");
	}

	QcdmEfsSyncResponse* response = (QcdmEfsSyncResponse*)buffer;

	if (response->error || response->sequence != sequence) {
		throw QcdmResponseError("Command Error");

	}

	std::memcpy(&ret, response, sizeof(QcdmEfsSyncResponse));

	return ret;	
}

QcdmEfsGetSyncStatusResponse DmEfsManager::getSyncStatus(uint32_t token, uint16_t sequence)
{
	QcdmEfsGetSyncStatusResponse ret = {};
	QcdmEfsGetSyncStatusRequest packet = {};

	packet.header	= getHeader(DIAG_EFS_SYNC_GET_STATUS);
	packet.sequence = sequence;
	packet.token	= token;

	int commandResult = sendCommand(packet.header.subsysCommand, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (commandResult != kDmEfsSuccess) {
		throw QcdmResponseError("Command Error");
	}

	QcdmEfsSyncResponse* response = (QcdmEfsSyncResponse*)buffer;

	if (response->error) {
		throw QcdmResponseError(port.getErrorString(response->header.command));
	}

	if (response->sequence != sequence) {
		throw QcdmResponseError("Invalid Sequence");
	}
	
	std::memcpy(&ret, response, sizeof(packet));

	return ret;
}


int DmEfsManager::makeGoldenCopy(std::string path, int32_t sequence)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsMakeGoldenCopyRequest) + path.size();
    QcdmEfsMakeGoldenCopyRequest* packet = (QcdmEfsMakeGoldenCopyRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_MAKE_GOLDEN_COPY);
    packet->sequence = sequence;
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_MAKE_GOLDEN_COPY, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {       
        return commandResult;
    }   

    QcdmEfsMakeGoldenCopyResponse* response = (QcdmEfsMakeGoldenCopyResponse*)buffer;

    if (response->error || response->sequence != sequence) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}


int DmEfsManager::openFilesystemImage(std::string path, uint8_t imageType, int32_t& handle, int32_t sequence)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    size_t packetSize = sizeof(QcdmEfsFsImageOpenRequest) + path.size();
    QcdmEfsFsImageOpenRequest* packet = (QcdmEfsFsImageOpenRequest*) new uint8_t[packetSize]();

    packet->header = getHeader(DIAG_EFS_FILESYSTEM_IMAGE_OPEN);
    packet->sequence = sequence;
    packet->imageType = imageType;
    std::memcpy(packet->path, path.c_str(), path.size());

    int commandResult = sendCommand(DIAG_EFS_FILESYSTEM_IMAGE_OPEN, reinterpret_cast<uint8_t*>(packet), packetSize);

    delete packet;

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsFsImageOpenResponse* response = (QcdmEfsFsImageOpenResponse*)buffer;

    if (response->error || response->sequence != sequence) {
        return kDmEfsError;
    }

    handle = response->handle;

    return kDmEfsSuccess;
}

int DmEfsManager::readFilesystemImage(int32_t handle, std::vector<uint8_t>& data, int32_t sequence)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    QcdmEfsFsImageReadRequest packet = {
        getHeader(DIAG_EFS_FILESYSTEM_IMAGE_CLOSE),
        sequence,
        handle
    };

    int commandResult = sendCommand(DIAG_EFS_FILESYSTEM_IMAGE_READ, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsFsImageReadResponse* response = (QcdmEfsFsImageReadResponse*)buffer;

    if (response->error || response->sequence != sequence) {
        return kDmEfsError;
    }

    data.reserve(data.size() + response->length);

    data.insert(data.end(), response->data, response->data + response->length);
    
    return kDmEfsSuccess;
}

int DmEfsManager::closeFilesystemImage(int32_t handle, int32_t sequence)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    QcdmEfsFsImageCloseRequest packet = {
        getHeader(DIAG_EFS_FILESYSTEM_IMAGE_CLOSE),
        sequence,
        handle
    };

    int commandResult = sendCommand(DIAG_EFS_FILESYSTEM_IMAGE_CLOSE, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

    if (commandResult != kDmEfsSuccess) {
        return commandResult;
    }

    QcdmEfsFsImageOpenResponse* response = (QcdmEfsFsImageOpenResponse*)buffer;

    if (response->error || response->sequence != sequence) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}


/**
* @brief getHeader - Used internally to quickly assemble a packet header
*
* @param uint16_t - The command to create the header data for
*
* @return QcdmSubsysHeader
*/
QcdmSubsysHeader DmEfsManager::getHeader(uint16_t command)
{
    QcdmSubsysHeader header = {
        getSubsystemCommand(),
        getSubsystemId(),
        command
    };

    return header;
}

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

int DmEfsManager::sendCommand(uint16_t command)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    QcdmSubsysHeader packet = getHeader(command);

    return sendCommand(command, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
}

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
int DmEfsManager::sendCommand(uint16_t command, uint8_t* data, size_t dataSize)
{
    if (!port.isOpen()) {
        return kDmEfsIOError;
    }

    try {
        if (!port.write(data, dataSize)) {
            return kDmEfsIOError;
        }
    }
    catch (serial::IOException e) {
        return kDmEfsIOError;
    }

    size_t rxSize;

    try {
        rxSize = port.read(buffer, DIAG_MAX_PACKET_SIZE);

        if (!rxSize) {
            return kDmEfsIOError;
        }
    }
    catch (serial::IOException e) {
        return kDmEfsIOError;
    }

    if (!isValidResponse(command, buffer, rxSize)) {
        return kDmEfsError;
    }

    return kDmEfsSuccess;
}

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
bool DmEfsManager::isValidResponse(uint16_t command, uint8_t* data, size_t size)
{
    if (size == 0) {
        return false;
    }

    QcdmSubsysHeader* header = (QcdmSubsysHeader*)data;

    if (header->command != getSubsystemCommand() || 
        header->subsysId != getSubsystemId() ||
        header->subsysCommand != command
    ) {     
        switch (header->command) {
            case DIAG_BAD_CMD_F:
                LOGE("Error: Bad Command Subsys Command %d - %04X\n", command, command);
                break;
            case DIAG_BAD_PARM_F:
                LOGE("Error: Bad Parameter Subsys Command %d - %04X\n", command, command);
                break;
            case DIAG_BAD_LEN_F:
                LOGE("Error: Bad Length Subsys Command %d - %04X\n", command, command);
                break;
        }
        return false;
    }

    return true;
}
