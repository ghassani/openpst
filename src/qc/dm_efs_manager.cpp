
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
* @brief DmEfsManager::hello - Send the hello and recieve configuration parameters
*
* @param diag_subsys_efs_hello_rx_t &response - Populated with the results from the device on success
* @return int
*/
int DmEfsManager::hello(diag_subsys_efs_hello_rx_t& response)
{

	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	diag_subsys_efs_hello_tx_t packet = {
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
* @param diag_subsys_efs_device_info_rx_t &response - Populated with the results from the device on success
* @return int
*/
int DmEfsManager::getDeviceInfo(diag_subsys_efs_device_info_rx_t& response)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}
	
	int commandResult = sendCommand(DIAG_EFS_DEV_INFO);

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_device_info_rx_t* resp = (diag_subsys_efs_device_info_rx_t*)buffer;

	std::memcpy(&response, buffer, sizeof(response));

	return kDmEfsSuccess;
}

/**
* @brief query - Query for file system settings
*
* @param diag_subsys_efs_query_rx_t &response - Populated with the results from the device on success
* @return int
*/
int DmEfsManager::query(diag_subsys_efs_query_rx_t& response)
{
	if (!port.isOpen()) {		
		return kDmEfsIOError;
	}

	int commandResult = sendCommand(DIAG_EFS_QUERY);

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_query_rx_t* resp = (diag_subsys_efs_query_rx_t*)buffer;

	std::memcpy(&response, buffer, sizeof(response));

	return kDmEfsSuccess; 
}


int DmEfsManager::open(std::string path, int32_t flags, int32_t mode, int32_t& fp)
{
	
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_open_file_tx_t) + path.size();
	diag_subsys_efs_open_file_tx_t* packet = (diag_subsys_efs_open_file_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_OPEN);
	packet->flags = flags;
	packet->mode = mode;	
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_OPEN, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}
	
	diag_subsys_efs_open_file_rx_t* response = (diag_subsys_efs_open_file_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}

	fp = response->fp;

	return kDmEfsSuccess;
}

int DmEfsManager::close(int32_t fp)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	diag_subsys_efs_close_file_tx_t packet = {
		getHeader(DIAG_EFS_CLOSE),
		fp
	};

	int commandResult = sendCommand(DIAG_EFS_CLOSE, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_close_file_rx_t* response = (diag_subsys_efs_close_file_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}

int DmEfsManager::read(int32_t fp, size_t size, uint32_t offset, std::vector<uint8_t>& data)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	diag_subsys_efs_read_file_tx_t packet = {
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
	
	diag_subsys_efs_read_file_rx_t* response = (diag_subsys_efs_read_file_rx_t*)buffer;
	
	hexdump(response->data, rxSize - sizeof(diag_subsys_efs_read_file_rx_t));

	data.insert(data.end(), response->data, response->data + (rxSize - sizeof(diag_subsys_efs_read_file_rx_t)));
	
	return kDmEfsSuccess;
}

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

	diag_subsys_efs_fstat_rx_t fileInfo;

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

	std::ofstream out(outPath.c_str(), std::ios::binary | std::ios::out || ios::trunc);

	if (!out.is_open()) {
		return kDmEfsIOError;
	}

	out.write(reinterpret_cast<char*>(&data), data.size());

	return kDmEfsSuccess;
}

int DmEfsManager::write(int32_t fp, uint8_t* data, size_t amount, uint32_t offset)
{
	return 0;
}

int DmEfsManager::symlink(std::string path, std::string newPath)
{
	return 0;
}

int DmEfsManager::readSimlink(std::string path, std::string& out)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_read_link_tx_t) + path.size();
	diag_subsys_efs_read_link_tx_t* packet = (diag_subsys_efs_read_link_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_READLINK);
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_READLINK, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}
	
	diag_subsys_efs_read_link_rx_t* response = (diag_subsys_efs_read_link_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}

	out = response->path;

	return kDmEfsSuccess;
}

int DmEfsManager::unlink(std::string path)
{
	
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_unlink_tx_t) + path.size();
	diag_subsys_efs_unlink_tx_t* packet = (diag_subsys_efs_unlink_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_UNLINK);
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_UNLINK, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_unlink_rx_t* response = (diag_subsys_efs_unlink_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}

int DmEfsManager::mkdir(std::string path, int16_t mode)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_mkdir_tx_t) + path.size();
	diag_subsys_efs_mkdir_tx_t* packet = (diag_subsys_efs_mkdir_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_MKDIR);
	packet->mode = mode;
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_MKDIR, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_mkdir_rx_t* response = (diag_subsys_efs_mkdir_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}

int DmEfsManager::rmdir(std::string path)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_rmdir_tx_t) + path.size();
	diag_subsys_efs_rmdir_tx_t* packet = (diag_subsys_efs_rmdir_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_RMDIR);
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_RMDIR, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_mkdir_rx_t* response = (diag_subsys_efs_mkdir_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}


int DmEfsManager::openDir(std::string path, uint32_t& dp)
{

	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_open_dir_tx_t) + path.size();
	diag_subsys_efs_open_dir_tx_t* packet = (diag_subsys_efs_open_dir_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_OPENDIR);
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_OPENDIR, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_open_dir_rx_t* response = (diag_subsys_efs_open_dir_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}

	dp = response->dp;

	return kDmEfsSuccess; 
}

int DmEfsManager::closeDir(uint32_t dp)
{

	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	diag_subsys_efs_close_dir_tx_t packet = {
		getHeader(DIAG_EFS_CLOSEDIR),
		dp
	};

	int commandResult = sendCommand(DIAG_EFS_CLOSEDIR, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
	
	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_close_dir_rx_t* response = (diag_subsys_efs_close_dir_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}

	return kDmEfsSuccess; 
}

int DmEfsManager::readDir(std::string path, std::vector<DmEfsNode>& contents, bool recursive)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	uint32_t dp;

	if (int openResult = openDir(path, dp) != kDmEfsSuccess) {
		return openResult;
	}

	int readResult = readDir(dp, contents);

	if (recursive) {
		for (auto &node : contents) {
			if (node.isDir()) {

				std::string checkPath = path;

				if (0 != path.compare("/")) {
					checkPath.append("/");
				}

				checkPath.append(node.getName());

				printf("Parent Path: %s Check Path: %s\n", path.c_str(), checkPath.c_str());

				if (readDir(checkPath, node.getChildren(), recursive) != kDmEfsSuccess) {
					printf("Error Reading Dir\n");
					continue;
				}
			}
		}
	}

	closeDir(dp);

	return readResult;
}

int DmEfsManager::readDir(uint32_t dp, std::vector<DmEfsNode>& contents)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t txSize, rxSize;

	diag_subsys_efs_read_dir_tx_t packet = {};
	packet.header = getHeader(DIAG_EFS_READDIR);
	packet.dp = dp;
	packet.sequenceNumber = 1;
			
	do {

		txSize = port.write((uint8_t*)&packet, sizeof(packet));

		if (!txSize) {
			return kDmEfsIOError;
		}

		rxSize = port.read(buffer, DIAG_MAX_PACKET_SIZE);

		if (!rxSize) {
			return kDmEfsIOError;
		}

		diag_subsys_efs_read_dir_rx_t* response = (diag_subsys_efs_read_dir_rx_t*)buffer;

		if (response->error || response->sequenceNumber != packet.sequenceNumber || strlen(response->name) == 0) {
			break;
		}

		DmEfsNode node("", response);

		contents.insert(contents.end(), node);

		packet.sequenceNumber++;

	} while (true);

	return kDmEfsSuccess;
}


int DmEfsManager::rename(std::string path, std::string newPath)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_rename_tx_t) + path.size() + newPath.size();
	diag_subsys_efs_rename_tx_t* packet = (diag_subsys_efs_rename_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_RENAME);
	std::memcpy(packet->path, path.c_str(), path.size());
	std::memcpy((packet->path + path.size() + 1), newPath.c_str(), newPath.size());

	int commandResult = sendCommand(DIAG_EFS_RENAME, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_access_rx_t* response = (diag_subsys_efs_access_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}

int DmEfsManager::stat(std::string path, diag_subsys_efs_stat_rx_t& response)
{
if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_stat_tx_t) + path.size();
	diag_subsys_efs_stat_tx_t* packet = (diag_subsys_efs_stat_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_STAT);
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_STAT, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_stat_rx_t* resp = (diag_subsys_efs_stat_rx_t*)buffer;
	
	if (resp->error) {		
		return kDmEfsError;
	}

	std::memcpy(&response, buffer, sizeof(response));

	return kDmEfsSuccess;
}

int DmEfsManager::lstat(std::string path, diag_subsys_efs_lstat_rx_t& response)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_lstat_tx_t) + path.size();
	diag_subsys_efs_lstat_tx_t* packet = (diag_subsys_efs_lstat_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_LSTAT);
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_LSTAT, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_lstat_rx_t* resp = (diag_subsys_efs_lstat_rx_t*)buffer;
	
	if (resp->error) {		
		return kDmEfsError;
	}

	std::memcpy(&response, buffer, sizeof(response));

	return kDmEfsSuccess;
}

int DmEfsManager::fstat(int32_t fp, diag_subsys_efs_fstat_rx_t& response)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	diag_subsys_efs_fstat_tx_t packet = {};
	packet.header = getHeader(DIAG_EFS_FSTAT);
	packet.fp = fp;
	
	int commandResult = sendCommand(DIAG_EFS_FSTAT, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_fstat_rx_t* resp = (diag_subsys_efs_fstat_rx_t*)buffer;

	if (resp->error) {
		return kDmEfsError;
	}

	std::memcpy(&response, buffer, sizeof(response));

	return kDmEfsSuccess;
}

int DmEfsManager::chmod(std::string path, int16_t mode)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_chmod_tx_t) + path.size();
	diag_subsys_efs_chmod_tx_t* packet = (diag_subsys_efs_chmod_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_CHMOD);
	packet->mode = mode;
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_CHMOD, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_chmod_rx_t* response = (diag_subsys_efs_chmod_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}

int DmEfsManager::statfs(std::string path, diag_subsys_efs_statfs_rx_t& response)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_statfs_tx_t) + path.size();
	diag_subsys_efs_statfs_tx_t* packet = (diag_subsys_efs_statfs_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_STATFS);
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_STATFS, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}
	
	diag_subsys_efs_statfs_tx_t* resp = (diag_subsys_efs_statfs_tx_t*)buffer;

	std::memcpy(&response, buffer, sizeof(response));

	return kDmEfsSuccess;
}

int DmEfsManager::access(std::string path, char checkPermissionBits)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_access_tx_t) + path.size();
	diag_subsys_efs_access_tx_t* packet = (diag_subsys_efs_access_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_ACCESS);
	packet->permissionBits = checkPermissionBits;
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_ACCESS, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_access_rx_t* response = (diag_subsys_efs_access_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}

int DmEfsManager::getFactoryImage(std::ofstream& out)
{
	return  0;
}

int DmEfsManager::factoryImageStart()
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	int commandResult = sendCommand(DIAG_EFS_FACT_IMAGE_START);

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_factory_image_start_rx_t* response = (diag_subsys_efs_factory_image_start_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}
	return kDmEfsSuccess;
}


int DmEfsManager::factoryImageRead(std::vector<uint8_t>& data)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	diag_subsys_efs_factory_image_read_tx_t packet = {};
	packet.header = getHeader(DIAG_EFS_FACT_IMAGE_START);
	packet.streamState = 0x00;
	packet.infoClusterSent = 0x00;
	packet.clusterMapSequence = 0x00;
	packet.clusterDataSequence = 0x00;

	int commandResult = sendCommand(DIAG_EFS_FACT_IMAGE_START, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_factory_image_read_rx_t* response = (diag_subsys_efs_factory_image_read_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}


int DmEfsManager::factoryImageEnd()
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	int commandResult = sendCommand(DIAG_EFS_FACT_IMAGE_START);

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_factory_image_end_rx_t* response = (diag_subsys_efs_factory_image_end_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}
	return kDmEfsSuccess;
}


int DmEfsManager::factoryImagePrepare()
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	int commandResult = sendCommand(DIAG_EFS_PREP_FACT_IMAGE);

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_factory_image_prepare_rx_t* response = (diag_subsys_efs_factory_image_prepare_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}

int DmEfsManager::chown(std::string path, int32_t uid, int32_t gid)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_chown_tx_t) + path.size();
	diag_subsys_efs_chown_tx_t* packet = (diag_subsys_efs_chown_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_CHOWN);
	packet->uid = uid;
	packet->gid = gid;
	std::memcpy(packet->path, path.c_str(), path.size());
	
	int commandResult = sendCommand(DIAG_EFS_CHOWN, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_chown_rx_t* response = (diag_subsys_efs_chown_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}


int DmEfsManager::setQuota(std::string path, int32_t gid, size_t size)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_set_quota_tx_t) + path.size();
	diag_subsys_efs_set_quota_tx_t* packet = (diag_subsys_efs_set_quota_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_SET_QUOTA);
	packet->gid = gid;
	packet->amount = size;
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_SET_QUOTA, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_set_quota_rx_t* response = (diag_subsys_efs_set_quota_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}

int DmEfsManager::getGroupInfo(std::string path, int32_t gid)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_get_group_info_tx_t) + path.size();
	diag_subsys_efs_get_group_info_tx_t* packet = (diag_subsys_efs_get_group_info_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_GET_GROUP_INFO);
	packet->gid = gid;
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_GET_GROUP_INFO, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_get_group_info_rx_t* response = (diag_subsys_efs_get_group_info_rx_t*)buffer;

	if (response->error) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}

int DmEfsManager::deltree(std::string path)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_deltree_tx_t) + path.size();
	diag_subsys_efs_deltree_tx_t* packet = (diag_subsys_efs_deltree_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_DELTREE);
	packet->sequence = 1;
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_DELTREE, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_deltree_rx_t* response = (diag_subsys_efs_deltree_rx_t*)buffer;

	if (response->error || response->sequence != 1) {
		return kDmEfsError;
	}
	
	return kDmEfsSuccess;
}

int DmEfsManager::truncate(std::string path, size_t amount, int32_t sequence)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_truncate_tx_t) + path.size();
	diag_subsys_efs_truncate_tx_t* packet = (diag_subsys_efs_truncate_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_TRUNCATE);
	packet->sequence = sequence;
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_TRUNCATE, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_truncate_rx_t* response = (diag_subsys_efs_truncate_rx_t*)buffer;

	if (response->error || response->sequence != sequence) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}

int DmEfsManager::ftruncate(int32_t fp, size_t amount, int32_t sequence)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	diag_subsys_efs_ftruncate_tx_t packet = {};

	packet.header = getHeader(DIAG_EFS_FTRUNCATE);
	packet.sequence = sequence;
	packet.fp = fp;

	int commandResult = sendCommand(DIAG_EFS_FTRUNCATE, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_ftruncate_rx_t* response = (diag_subsys_efs_ftruncate_rx_t*)buffer;

	if (response->error || response->sequence != sequence) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}


int DmEfsManager::statfsV2(diag_subsys_efs_statfs_v2_rx_t& response, int32_t sequence)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	diag_subsys_efs_statfs_v2_tx_t packet = {
		getHeader(DIAG_EFS_STATVFS_V2),
		sequence
	};

	int commandResult = sendCommand(DIAG_EFS_MD5SUM, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_statfs_v2_rx_t* resp = (diag_subsys_efs_statfs_v2_rx_t*)buffer;

	if (resp->error || resp->sequence != sequence) {
		return kDmEfsError;
	}

	std::memcpy(&response, buffer, sizeof(response));

	return kDmEfsSuccess;
}

int DmEfsManager::md5sum(std::string path, std::string& hash, int32_t sequence)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_md5_sum_tx_t) + path.size();
	diag_subsys_efs_md5_sum_tx_t* packet = (diag_subsys_efs_md5_sum_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_MD5SUM);
	packet->sequence = sequence;
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_MD5SUM, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_md5_sum_rx_t* response = (diag_subsys_efs_md5_sum_rx_t*)buffer;

	if (response->error || response->sequence != sequence) {
		return kDmEfsError;
	}

	uint8_t hex[2];
	for (int i = 0; i < 16; i++) {
		sprintf((char *)hex, "%02x", response->hash[i]);
		hash.append((char*)hex);
	}

	return kDmEfsSuccess;
}

int DmEfsManager::formatHotplugDevice(std::string path, int32_t sequence)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_hotplug_format_tx_t) + path.size();
	diag_subsys_efs_hotplug_format_tx_t* packet = (diag_subsys_efs_hotplug_format_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_HOTPLUG_FORMAT);
	packet->sequence = sequence;
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_HOTPLUG_FORMAT, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}
	
	diag_subsys_efs_hotplug_format_rx_t* response = (diag_subsys_efs_hotplug_format_rx_t*)buffer;

	if (response->error || response->sequence != sequence) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}

int DmEfsManager::shred(std::string path, int32_t sequence)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_shred_tx_t) + path.size();
	diag_subsys_efs_shred_tx_t* packet = (diag_subsys_efs_shred_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_SHRED);
	packet->sequence = sequence;
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_SHRED, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {
		return commandResult;
	}

	diag_subsys_efs_shred_rx_t* response = (diag_subsys_efs_shred_rx_t*)buffer;

	if (response->error || response->sequence != sequence) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}


int DmEfsManager::makeGoldenCopy(std::string path, int32_t sequence)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	size_t packetSize = sizeof(diag_subsys_efs_make_golden_copy_tx_t) + path.size();
	diag_subsys_efs_make_golden_copy_tx_t* packet = (diag_subsys_efs_make_golden_copy_tx_t*) new uint8_t[packetSize]();

	packet->header = getHeader(DIAG_EFS_MAKE_GOLDEN_COPY);
	packet->sequence = sequence;
	std::memcpy(packet->path, path.c_str(), path.size());

	int commandResult = sendCommand(DIAG_EFS_MAKE_GOLDEN_COPY, reinterpret_cast<uint8_t*>(packet), packetSize);

	delete packet;

	if (commandResult != kDmEfsSuccess) {		
		return commandResult;
	}	

	diag_subsys_efs_make_golden_copy_rx_t* response = (diag_subsys_efs_make_golden_copy_rx_t*)buffer;

	if (response->error || response->sequence != sequence) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}

int DmEfsManager::openFilesystemImage()
{
	return 0;
}

int DmEfsManager::readFilesystemImage()
{
	return 0;
}

int DmEfsManager::closeFilesystemImage()
{
	return 0;
}


qcdm_subsys_header_t DmEfsManager::getHeader(uint16_t command)
{
	qcdm_subsys_header_t header = {
		getSubsystemCommand(),
		getSubsystemId(),
		command
	};

	return header;
}

bool DmEfsManager::isValidResponse(uint16_t command, uint8_t* data, size_t size)
{
	if (size == 0) {
		return false;
	}

	qcdm_subsys_header_t* header = (qcdm_subsys_header_t*)data;

	if (header->command != getSubsystemCommand() || 
		header->subsysId != getSubsystemId() ||
		header->subsysCommand != command
	) {		
		switch (header->command) {
			case DIAG_BAD_CMD_F:
				LOGE("Error: Bad Command\n");
				break;
			case DIAG_BAD_PARM_F:
				LOGE("Error: Bad Parameter\n");
				break;
			case DIAG_BAD_LEN_F:
				LOGE("Error: Bad Length\n");
				break;
		}
		return false;
	}

	return true;
}


int DmEfsManager::sendCommand(uint16_t command)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	qcdm_subsys_header_t packet = getHeader(command);

	return sendCommand(command, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
}

int DmEfsManager::sendCommand(uint16_t command, uint8_t* data, size_t dataSize)
{
	if (!port.isOpen()) {
		return kDmEfsIOError;
	}

	try {
		if (!port.write(data, dataSize)) {
			return kDmEfsIOError;
		}
	} catch (serial::IOException e) {
		return kDmEfsIOError;
	}
	

	
	size_t rxSize;
	
	try {
		rxSize = port.read(buffer, DIAG_MAX_PACKET_SIZE);
		
		if (!rxSize) {
			return kDmEfsIOError;
		}
	} catch (serial::IOException e) {
		return kDmEfsIOError;
	}


	if (!isValidResponse(command, buffer, rxSize)) {
		return kDmEfsError;
	}

	return kDmEfsSuccess;
}