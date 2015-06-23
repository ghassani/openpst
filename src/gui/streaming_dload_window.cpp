/**
* LICENSE PLACEHOLDER
*
* @file streaming_dload_window.cpp
* @class StreamingDloadWindow
* @package OpenPST
* @brief Streaming DLOAD GUI interface class
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#include "streaming_dload_window.h"


StreamingDloadWindow::StreamingDloadWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::StreamingDloadWindow),
	port("", 115200)
{
    ui->setupUi(this);
	 
	this->statusBar()->setSizeGripEnabled(false);
	
	ui->securityModeValue->addItem("0x01 - Trusted", STREAMING_DLOAD_SECURITY_MODE_TRUSTED);
	ui->securityModeValue->addItem("0x00 - Untrusted", STREAMING_DLOAD_SECURITY_MODE_UNTRUSTED);
	ui->securityModeValue->setCurrentIndex(0);
	
	ui->openModeValue->addItem("0x01 - Bootloader Download", STREAMING_DLOAD_OPEN_MODE_BOOTLOADER_DOWNLOAD);
	ui->openModeValue->addItem("0x02 - Bootable Image Download", STREAMING_DLOAD_OPEN_MODE_BOOTABLE_IMAGE_DOWNLOAD);
	ui->openModeValue->addItem("0x03 - CEFS Image Download", STREAMING_DLOAD_OPEN_MODE_CEFS_IMAGE_DOWNLOAD);
	ui->openModeValue->setCurrentIndex(0);

	//ui->eccSetValue->addItem("", NULL);
	ui->eccSetValue->addItem("0x00 - Disable", 0x00);
	ui->eccSetValue->addItem("0x01 - Enable", 0x01);
	ui->eccSetValue->setCurrentIndex(0);

	ui->qfpromReadAddressTypeValue->addItem("0x00 - Corrected", 0x00);
	ui->qfpromReadAddressTypeValue->addItem("0x01 - RAW", 0x01);
	ui->qfpromReadAddressTypeValue->setCurrentIndex(0);

	//ui->openMultiValue->addItem("", NULL);
	ui->openMultiValue->addItem("0x01 - PBL", STREAMING_DLOAD_OPEN_MULTI_MODE_PBL);
	ui->openMultiValue->addItem("0x02 - QC SBL Header & Config", STREAMING_DLOAD_OPEN_MULTI_MODE_QCSBLHDCFG);
	ui->openMultiValue->addItem("0x03 - QC SBL", STREAMING_DLOAD_OPEN_MULTI_MODE_QCSBL);
	ui->openMultiValue->addItem("0x04 - OEM SBL", STREAMING_DLOAD_OPEN_MULTI_MODE_OEMSBL);
	ui->openMultiValue->addItem("0x05 - AMSS", STREAMING_DLOAD_OPEN_MULTI_MODE_AMSS);
	ui->openMultiValue->addItem("0x06 - APPS", STREAMING_DLOAD_OPEN_MULTI_MODE_APPS);
	ui->openMultiValue->addItem("0x07 - OBL - MSM6250", STREAMING_DLOAD_OPEN_MULTI_MODE_OBL);
	ui->openMultiValue->addItem("0x08 - FOTA UI", STREAMING_DLOAD_OPEN_MULTI_MODE_FOTAUI);
	ui->openMultiValue->addItem("0x09 - CEFS", STREAMING_DLOAD_OPEN_MULTI_MODE_CEFS);
	ui->openMultiValue->addItem("0x0A - AMSS applications boot loader", STREAMING_DLOAD_OPEN_MULTI_MODE_APPS_CEFS);
	ui->openMultiValue->addItem("0x0B - Apps CEFS", STREAMING_DLOAD_OPEN_MULTI_MODE_FLASH_BIN);
	ui->openMultiValue->addItem("0x0C - Flash.bin for Windows Mobile", STREAMING_DLOAD_OPEN_MULTI_MODE_DSP1);
	ui->openMultiValue->addItem("0x0D  -DSP1 runtime image", STREAMING_DLOAD_OPEN_MULTI_MODE_DSP1);
	ui->openMultiValue->addItem("0x0E - User defined partition", STREAMING_DLOAD_OPEN_MULTI_MODE_CUSTOM);
	ui->openMultiValue->addItem("0x0F - DBL - Secure Boot 2.0", STREAMING_DLOAD_OPEN_MULTI_MODE_DBL);
	ui->openMultiValue->addItem("0x10 - OSBL - Secure Boot 2.0", STREAMING_DLOAD_OPEN_MULTI_MODE_OSBL);
	ui->openMultiValue->addItem("0x11 - FSBL - Secure Boot 2.0", STREAMING_DLOAD_OPEN_MULTI_MODE_FSBL);
	ui->openMultiValue->addItem("0x12 - DSP2 executable ", STREAMING_DLOAD_OPEN_MULTI_MODE_DSP2);
	ui->openMultiValue->addItem("0x13 - Apps EFS2 raw image ", STREAMING_DLOAD_OPEN_MULTI_MODE_RAW);
	ui->openMultiValue->addItem("0x14 - ROFS1 - Symbian", STREAMING_DLOAD_OPEN_MULTI_MODE_ROFS1);
	ui->openMultiValue->addItem("0x15 - ROFS2 - Symbian", STREAMING_DLOAD_OPEN_MULTI_MODE_ROFS2);
	ui->openMultiValue->addItem("0x16 - ROFS3 - Symbian", STREAMING_DLOAD_OPEN_MULTI_MODE_ROFS3);
	ui->openMultiValue->addItem("0x21 - EMMC USER partition ", STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_USER);
	ui->openMultiValue->addItem("0x22 - EMMC BOOT0 partition ", STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_BOOT0);
	ui->openMultiValue->addItem("0x23 - EMMC BOOT1 partition ", STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_BOOT1);
	ui->openMultiValue->addItem("0x24 - EMMC RPMB", STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_RPMB);
	ui->openMultiValue->addItem("0x25 - EMMC GPP1", STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_GPP1);
	ui->openMultiValue->addItem("0x26 - EMMC GPP2", STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_GPP2);
	ui->openMultiValue->addItem("0x27 - EMMC GPP3", STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_GPP3);
	ui->openMultiValue->addItem("0x28 - EMMC GPP4", STREAMING_DLOAD_OPEN_MULTI_MODE_EMMC_GPP4);
	ui->openMultiValue->setCurrentIndex(0);
	
	updatePortList();

	QObject::connect(ui->portRefreshButton, SIGNAL(clicked()), this, SLOT(updatePortList()));
	QObject::connect(ui->portDisconnectButton, SIGNAL(clicked()), this, SLOT(disconnectPort()));
	QObject::connect(ui->portConnectButton, SIGNAL(clicked()), this, SLOT(connectToPort()));	
	QObject::connect(ui->helloButton, SIGNAL(clicked()), this, SLOT(sendHello()));	
	QObject::connect(ui->unlockButton, SIGNAL(clicked()), this, SLOT(sendUnlock()));
	QObject::connect(ui->securityModeButton, SIGNAL(clicked()), this, SLOT(setSecurityMode()));
	QObject::connect(ui->nopButton, SIGNAL(clicked()), this, SLOT(sendNop()));
	QObject::connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(sendReset()));
	QObject::connect(ui->powerDownButton, SIGNAL(clicked()), this, SLOT(sendPowerDown()));
	QObject::connect(ui->eccReadButton, SIGNAL(clicked()), this, SLOT(readEccState()));
	QObject::connect(ui->eccSetButton, SIGNAL(clicked()), this, SLOT(setEccState()));
	QObject::connect(ui->openModeButton, SIGNAL(clicked()), this, SLOT(openMode()));
	QObject::connect(ui->openMultiButton, SIGNAL(clicked()), this, SLOT(openMultiMode()));
	QObject::connect(ui->closeModeButton, SIGNAL(clicked()), this, SLOT(closeMode()));
	QObject::connect(ui->openMultiCloseButton, SIGNAL(clicked()), this, SLOT(closeMode()));
	QObject::connect(ui->readButton, SIGNAL(clicked()), this, SLOT(read()));
	QObject::connect(ui->qfpromReadButton, SIGNAL(clicked()), this, SLOT(readQfprom()));
	QObject::connect(ui->writePartitionTableButton, SIGNAL(clicked()), this, SLOT(writePartitionTable()));

	
	QObject::connect(ui->clearLogButton, SIGNAL(clicked()), this, SLOT(clearLog()));
	QObject::connect(ui->writePartitionTableFileBrowseButton, SIGNAL(clicked()), this, SLOT(browseForParitionTable()));
	QObject::connect(ui->writeFileBrowseButton, SIGNAL(clicked()), this, SLOT(browseForWriteFile()));
}

StreamingDloadWindow::~StreamingDloadWindow()
{
	if (port.isOpen()) {
		port.close();
	}

	delete ui;
}

/**
* @brief StreamingDloadWindow::updatePortList
*/
void StreamingDloadWindow::updatePortList()
{
	if (port.isOpen()) {
		log("Port is currently open");
		return;
	}

	std::vector<serial::PortInfo> devices = serial::list_ports();
	std::vector<serial::PortInfo>::iterator iter = devices.begin();

	ui->portList->clear();
	ui->portList->addItem("- Select a Port -");

	QString tmp;

	log(tmp.sprintf("Found %d devices", devices.size()));

	while (iter != devices.end()) {
		serial::PortInfo device = *iter++;

		log(tmp.sprintf("%s %s %s",
			device.port.c_str(),
			device.hardware_id.c_str(),
			device.description.c_str()
		));

		ui->portList->addItem(tmp, device.port.c_str());
	}
}
/**
* @brief StreamingDloadWindow::connectToPort
*/
void StreamingDloadWindow::connectToPort()
{
	QString selected = ui->portList->currentData().toString();
	QString tmp;

	if (selected.compare("0") == 0) {
		log("Select a Port First");
		return;
	}

	std::vector<serial::PortInfo> devices = serial::list_ports();
	std::vector<serial::PortInfo>::iterator iter = devices.begin();
	while (iter != devices.end()) {
		serial::PortInfo device = *iter++;
		if (selected.compare(device.port.c_str(), Qt::CaseInsensitive) == 0) {
			currentPort = device;
			break;
		}
	}

	if (!currentPort.port.length()) {
		log("Invalid Port Type");
		return;
	}

	try {
		port.setPort(currentPort.port);

		if (!port.isOpen()){
			port.open();
		}
	}
	catch (serial::IOException e) {
		log(tmp.sprintf("Error Connecting To Port %s", currentPort.port.c_str()));
		log(e.getErrorNumber() == 13 ? "Permission Denied. Try Running With Elevated Privledges." : e.what());
		return;
	}

	log(tmp.sprintf("Connected to %s", currentPort.port.c_str()));
}

/**
* @brief StreamingDloadWindow::disconnectPort
*/
void StreamingDloadWindow::disconnectPort()
{
	if (port.isOpen()) {
		port.close();
		log("Port Closed");
	}
}

/**
* @brief StreamingDloadWindow::sendHello
*/
void StreamingDloadWindow::sendHello()
{

	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}

	if (!ui->helloMagicValue->text().length()) {
		log("Magic is required\n");
		return;
	}

	if (!ui->helloVersionValue->text().length()) {
		log("Version is required\n");
		return;
	}

	if (!ui->helloCompatibleVersionValue->text().length()) {
		log("Compatible version is required\n");
		return;
	}

	if (!ui->helloFeatureBitsValue->text().length()) {
		log("Feature bits are required\n");
		return;
	}
	
	QString tmp;
	
	std::string magic = ui->helloMagicValue->text().toStdString().c_str();
	uint8_t version = std::stoul(ui->helloVersionValue->text().toStdString().c_str(), nullptr, 16);
	uint8_t compatibleVersion = std::stoul(ui->helloCompatibleVersionValue->text().toStdString().c_str(), nullptr, 16);
	uint8_t featureBits = std::stoul(ui->helloFeatureBitsValue->text().toStdString().c_str(), nullptr, 16);

	if (!port.sendHello(magic, version, compatibleVersion, featureBits)) {
		log("Error Sending Hello");
		return;
	}	

	log(tmp.sprintf("Hello Response: %02X", port.deviceState.command));
	log(tmp.sprintf("Magic: %s", port.deviceState.magic));
	log(tmp.sprintf("Version: %02X", port.deviceState.version));
	log(tmp.sprintf("Compatible Version: %02X", port.deviceState.compatibleVersion));
	log(tmp.sprintf("Max Prefered Block Size: %d", port.deviceState.maxPreferredBlockSize));
	log(tmp.sprintf("Base Flash Address: 0x%08X", port.deviceState.baseFlashAddress));
	log(tmp.sprintf("Flash ID Length: %d", port.deviceState.flashIdLength));	
	log(tmp.sprintf("Flash Identifier: %s", port.deviceState.flashIdenfier));
	log(tmp.sprintf("Window Size: %d", port.deviceState.windowSize));
	log(tmp.sprintf("Number of Sectors: %d", port.deviceState.numberOfSectors));

	/*
	// dump all sector sizes
	for (int i = 0; i < port.deviceState.numberOfSectors; i++) {
		log(tmp.sprintf("Sector %d: %d", i, port.deviceState.sectorSizes[i*4]));
	}*/

	log(tmp.sprintf("Feature Bits: %04X", port.deviceState.featureBits));
}

/**
* @brief StreamingDloadWindow::setSecurityMode
*/
void StreamingDloadWindow::setSecurityMode()
{
	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}
	
	QString tmp;
	uint8_t mode = ui->securityModeValue->currentData().toUInt();

	if (!port.setSecurityMode(mode)) {
		log(tmp.sprintf("Error setting security mode to %s", mode == 0x00 ? "Untrusted" : "Trusted" ));
		return;
	}

	log(tmp.sprintf("Setting security mode set to %s", mode == 0x00 ? "Untrusted" : "Trusted"));
}

/**
* @brief StreamingDloadWindow::sendUnlock
*/
void StreamingDloadWindow::sendUnlock()
{
	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}

	if (!ui->unlockCodeValue->text().length()) {
		ui->unlockCodeValue->setText("0000");
	}

	if (!port.sendUnlock(ui->unlockCodeValue->text().toStdString())) {
		log("Error Sending Unlock");
		return;
	}
}


/**
* @brief sendNop
*/
void StreamingDloadWindow::sendNop()
{
	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}

	if (!port.sendNop()) {
		log("Error Sending NOP");
		return;
	}

	log("NOP Success");
}

/**
* @brief sendReset
*/
void StreamingDloadWindow::sendReset()
{
	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}

	if (!port.sendReset()) {
		log("Error Sending Reset");
		return;
	}

	log("Device Resetting");
	port.close();
}

/**
* @brief sendPowerDown
*/
void StreamingDloadWindow::sendPowerDown()
{
	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}

	if (!port.sendPowerOff()) {
		log("Error Sending Power Down");
		return;
	}

	log("Device Powering Down");
	port.close();
}

/**
* @brief closeMode
*/
void StreamingDloadWindow::openMode()
{
	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}

	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}

	QString tmp;
	uint8_t mode = ui->openModeValue->currentData().toUInt();

	if (!port.openMode(mode)) {
		log(tmp.sprintf("Error Opening Mode %s", port.getNamedOpenMode(mode)));
		return;
	}

	log(tmp.sprintf("Opened Mode %s", port.getNamedOpenMode(mode)));
}


/**
* @brief closeMode
*/
void StreamingDloadWindow::closeMode()
{
	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}

	if (!port.closeMode()) {
		log("Error Closing Mode");
		return;
	}

	log("Mode Closed");
}

/**
* @brief readEccState
*/
void StreamingDloadWindow::readEccState()
{
	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}

	uint8_t status;

	if (!port.readEcc(status)) {
		log("Error Reading ECC");
		return;
	}

	if (status == 0x01) {
		log("ECC Enabled");
	} else if (status == 0x00) {
		log("ECC Disabled");
	} else {
		log("Unknown ECC State");
	}

	// set the ecc set choice box value to the matching
	// state
	int choiceIdx = ui->eccSetValue->findData(status);
	if (choiceIdx) {
		ui->eccSetValue->setCurrentIndex(1);
	}
}

/**
* @brief setEccState
*/
void StreamingDloadWindow::setEccState()
{
	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}

	uint8_t state = ui->eccSetValue->currentData().toUInt();
	if (!port.setEcc(state)) {
		log("Error Setting ECC State");
		return;
	}

	if (state == 0x00) {
		log("ECC Disabled");
	} else if (state == 0x01) {
		log("ECC Enabled");
	}
}


/**
* @brief StreamingDloadWindow::read - Read address and size
*/
void StreamingDloadWindow::read()
{
	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}

	uint32_t address = std::stoul(ui->readAddressValue->text().toStdString().c_str(), nullptr, 16);
	size_t length = std::stoi(ui->readSizeValue->text().toStdString().c_str(), nullptr, 10);
	size_t chunkSize = std::stoi(ui->readChunkSizeValue->currentText().toStdString().c_str(), nullptr, 10);

	QString tmp;
	uint8_t* data = NULL;
	size_t dataSize = 0;

	log(tmp.sprintf("Attempting Read %lu bytes starting from address %08X", length, address));

	if (!port.readAddress(address, length, &data, dataSize, chunkSize)) {
		log(tmp.sprintf("Error reading data from address %08X with size %lu", address, length));
		return;
	}
	
	log(tmp.sprintf("Read %lu bytes starting from address %08X", dataSize, address));

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Read Data"), "", tr("Binary Files (*.bin)"));

	if (fileName.length()) {
		FILE* outFile = fopen(fileName.toStdString().c_str(), "a+b");
		if (outFile) {
			fwrite(data, sizeof(uint8_t), dataSize, outFile);
			fclose(outFile);
		}
	}

	if (dataSize > 0 && data != NULL) {
		free(data);
	}
}


/**
* @brief StreamingDloadWindow::openMultiMode
*/
void StreamingDloadWindow::openMultiMode()
{
	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}

	uint8_t imageType = ui->openMultiValue->currentData().toUInt();
	QString tmp;

	if (!port.openMultiImage(imageType)) {
		log(tmp.sprintf("Error opening multi image mode for %s", port.getNamedMultiImage(imageType)));
		return;
	}

	log(tmp.sprintf("Opening multi image mode for %s", port.getNamedMultiImage(imageType)));
	
}

/**
* @brief StreamingDloadWindow::readQfprom
*/
void StreamingDloadWindow::readQfprom()
{
	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}
	
	uint32_t rowAddress = std::stoul(ui->qfpromReadRowAddressValue->text().toStdString().c_str(), nullptr, 16);
	uint32_t addressType = ui->qfpromReadAddressTypeValue->currentData().toUInt();

	if (!port.readQfprom(rowAddress, addressType)) {
		log("Error Reading QFPROM Row Address");
		return;
	}

	log("Read QFPROM Row Address");
}

/**
* @brief StreamingDloadWindow::browseForParitionTable
*/
void StreamingDloadWindow::browseForParitionTable()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Select Partition Image To Send", "", "Partition Image Files (*.bin)");

	if (fileName.length()) {
		ui->writePartitionTableFileValue->setText(fileName);
	}
}

/**
* @brief StreamingDloadWindow::writePartitionTable
*/
void StreamingDloadWindow::writePartitionTable()
{

	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}

	QString fileName = ui->writePartitionTableFileValue->text();

	if (!fileName.length()) {
		log("Enter Partition File Path");
		return;
	}

	bool overwrite = ui->writeParitionTableOverrideExistingCheckbox->isChecked();
	uint8_t status = NULL;
	QString tmp;

	if (!port.writePartitionTable(fileName.toStdString(), status, overwrite)) {
		if (status) {
			log(tmp.sprintf("Error sending partition write. Status Received: %02X", status));
		} else {
			log("Error sending partition write");
		}
		return;
	}
	
	log(tmp.sprintf("Partition Table Response: %02X", status));
}


void StreamingDloadWindow::eraseFlash()
{
	/*
	QMessageBox::StandardButton confirmation = QMessageBox::question(this, "Test", "Quit?",
		QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes) {
		qDebug() << "Yes was clicked";
		QApplication::quit();
	}
	else {
		qDebug() << "Yes was *not* clicked";
	}*/
}


/**
* @brief StreamingDloadWindow::browseForWriteFile
*/
void StreamingDloadWindow::browseForWriteFile()
{

	QString fileName = QFileDialog::getOpenFileName(this, "Browse For File", "", "Image Files (*.bin, 8.mbn)");

	if (fileName.length()) {
		ui->writeFileValue->setText(fileName);
	}
}

/**
* @brief StreamingDloadWindow::clearLog
*/
void StreamingDloadWindow::clearLog()
{
	ui->log->clear();
}

/**
* @brief StreamingDloadWindow::saveLog
*/
void StreamingDloadWindow::saveLog()
{
	log("Not Implemented Yet");
}

/**
* @brief StreamingDloadWindow::log
* @param message
*/
void StreamingDloadWindow::log(const char* message)
{
	ui->log->appendPlainText(message);
}

/**
* @brief StreamingDloadWindow::log
* @param message
*/
void StreamingDloadWindow::log(QString message)
{
	ui->log->appendPlainText(message);
}
