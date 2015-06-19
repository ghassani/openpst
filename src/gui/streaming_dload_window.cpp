/**
* LICENSE PLACEHOLDER
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

	ui->eccSetValue->addItem("0x00 - Disable", 0x00);
	ui->eccSetValue->addItem("0x01 - Enable", 0x01);
	ui->eccSetValue->setCurrentIndex(0);

	UpdatePortList();

	QObject::connect(ui->portRefreshButton, SIGNAL(clicked()), this, SLOT(UpdatePortList()));
	QObject::connect(ui->portDisconnectButton, SIGNAL(clicked()), this, SLOT(DisconnectPort()));
	QObject::connect(ui->portConnectButton, SIGNAL(clicked()), this, SLOT(ConnectToPort()));	
	QObject::connect(ui->helloButton, SIGNAL(clicked()), this, SLOT(SendHello()));	
	QObject::connect(ui->unlockButton, SIGNAL(clicked()), this, SLOT(SendUnlock()));
	QObject::connect(ui->securityModeButton, SIGNAL(clicked()), this, SLOT(SetSecurityMode()));
	QObject::connect(ui->nopButton, SIGNAL(clicked()), this, SLOT(SendNop()));
	QObject::connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(SendReset()));
	QObject::connect(ui->powerDownButton, SIGNAL(clicked()), this, SLOT(SendPowerDown()));
	QObject::connect(ui->eccReadButton, SIGNAL(clicked()), this, SLOT(ReadEccState()));
	QObject::connect(ui->eccSetButton, SIGNAL(clicked()), this, SLOT(SetEccState()));
	QObject::connect(ui->openModeButton, SIGNAL(clicked()), this, SLOT(OpenMode()));
	QObject::connect(ui->closeModeButton, SIGNAL(clicked()), this, SLOT(CloseMode()));
	QObject::connect(ui->clearLogButton, SIGNAL(clicked()), this, SLOT(ClearLog()));

}

StreamingDloadWindow::~StreamingDloadWindow()
{
    delete ui;
}

/**
* @brief StreamingDloadWindow::UpdatePortList
*/
void StreamingDloadWindow::UpdatePortList()
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
* @brief StreamingDloadWindow::ConnectToPort
*/
void StreamingDloadWindow::ConnectToPort()
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
* @brief StreamingDloadWindow::DisconnectPort
*/
void StreamingDloadWindow::DisconnectPort()
{
	if (port.isOpen()) {
		port.close();
		log("Port Closed");
	}
}

/**
* @brief StreamingDloadWindow::SendHello
*/
void StreamingDloadWindow::SendHello()
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
* @brief StreamingDloadWindow::SetSecurityMode
*/
void StreamingDloadWindow::SetSecurityMode()
{
	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}
	if (!port.setSecurityMode(ui->securityModeValue->currentData().toUInt())) {
		log("Error Setting Security Mode");
		return;
	}
}

/**
* @brief StreamingDloadWindow::SendUnlock
*/
void StreamingDloadWindow::SendUnlock()
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
* @brief SendNop
*/
void StreamingDloadWindow::SendNop()
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
* @brief SendReset
*/
void StreamingDloadWindow::SendReset()
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
* @brief SendPowerDown
*/
void StreamingDloadWindow::SendPowerDown()
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
* @brief CloseMode
*/
void StreamingDloadWindow::OpenMode()
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
* @brief CloseMode
*/
void StreamingDloadWindow::CloseMode()
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
* @brief ReadEccState
*/
void StreamingDloadWindow::ReadEccState()
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
* @brief SetEccState
*/
void StreamingDloadWindow::SetEccState()
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
* @brief StreamingDloadWindow::ClearLog
*/
void StreamingDloadWindow::ClearLog()
{
	ui->log->clear();
}

/**
* @brief StreamingDloadWindow::SaveLog
*/
void StreamingDloadWindow::SaveLog()
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
