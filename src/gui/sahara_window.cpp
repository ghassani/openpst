/**
* LICENSE PLACEHOLDER
*
* @file sahara_window.cpp
* @class SaharaWindow
* @package OpenPST
* @brief Sahara GUI interface class
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "sahara_window.h"

SaharaWindow::SaharaWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SaharaWindow),
    port("", 115200)
{
    ui->setupUi(this);

	ui->writeHelloSwitchModeValue->addItem("", -1);
	ui->writeHelloSwitchModeValue->addItem("Image Transfer Pending",	SAHARA_MODE_IMAGE_TX_PENDING);
	ui->writeHelloSwitchModeValue->addItem("Image Transfer Complete",	SAHARA_MODE_IMAGE_TX_COMPLETE);
	ui->writeHelloSwitchModeValue->addItem("Memory Debug",				SAHARA_MODE_MEMORY_DEBUG);
	ui->writeHelloSwitchModeValue->addItem("Client Command Mode",		SAHARA_MODE_COMMAND);

	ui->clientCommandValue->addItem("", -1);
	ui->clientCommandValue->addItem("NOP",						 SAHARA_EXEC_CMD_NOP);
	ui->clientCommandValue->addItem("Read Serial Number",		 SAHARA_EXEC_CMD_SERIAL_NUM_READ);
	ui->clientCommandValue->addItem("Read MSM HW ID",			 SAHARA_EXEC_CMD_MSM_HW_ID_READ);
	ui->clientCommandValue->addItem("Read OEM PK Hash",			 SAHARA_EXEC_CMD_OEM_PK_HASH_READ);
	ui->clientCommandValue->addItem("Switch To DMSS DLOAD",		 SAHARA_EXEC_CMD_SWITCH_TO_DMSS_DLOAD);
	ui->clientCommandValue->addItem("Switch To Streaming DLOAD", SAHARA_EXEC_CMD_SWITCH_TO_STREAM_DLOAD);
	ui->clientCommandValue->addItem("Read Debug Data",			 SAHARA_EXEC_CMD_READ_DEBUG_DATA);
	ui->clientCommandValue->addItem("Get SBL SW Version",		 SAHARA_EXEC_CMD_GET_SOFTWARE_VERSION_SBL);

	ui->switchModeValue->addItem("", -1);
	ui->switchModeValue->addItem("Image Transfer Pending",  SAHARA_MODE_IMAGE_TX_PENDING);
	ui->switchModeValue->addItem("Image Transfer Complete", SAHARA_MODE_IMAGE_TX_COMPLETE);
	ui->switchModeValue->addItem("Memory Debug",			SAHARA_MODE_MEMORY_DEBUG);
    ui->switchModeValue->addItem("Client Command Mode",     SAHARA_MODE_COMMAND);

    updatePortList();

    QObject::connect(ui->portRefreshButton,             SIGNAL(clicked()), this, SLOT(updatePortList()));
    QObject::connect(ui->readHelloButton,				SIGNAL(clicked()), this, SLOT(readHello()));
    QObject::connect(ui->writeHelloButton,				SIGNAL(clicked()), this, SLOT(writeHello()));
    QObject::connect(ui->portDisconnectButton,          SIGNAL(clicked()), this, SLOT(disconnectPort()));
    QObject::connect(ui->portConnectButton,             SIGNAL(clicked()), this, SLOT(connectToPort()));
    QObject::connect(ui->switchModeButton,				SIGNAL(clicked()), this, SLOT(switchMode()));
    QObject::connect(ui->clientCommandButton,			SIGNAL(clicked()), this, SLOT(sendClientCommand()));
	QObject::connect(ui->resetButton,					SIGNAL(clicked()), this, SLOT(sendReset()));
	QObject::connect(ui->doneButton,					SIGNAL(clicked()), this, SLOT(sendDone()));
	QObject::connect(ui->sendImageFileBrowseButton,		SIGNAL(clicked()), this, SLOT(browseForImage()));
	QObject::connect(ui->sendImageButton, SIGNAL(clicked()), this, SLOT(sendImage()));
	QObject::connect(ui->memoryDebugReadButton, SIGNAL(clicked()), this, SLOT(memoryDebugRead()));

	
	QObject::connect(ui->streamingDloadHelloButton,		SIGNAL(clicked()), this, SLOT(sendStreamingDloadHello()));
    QObject::connect(ui->clearLogButton,                SIGNAL(clicked()), this, SLOT(clearLog()));
}

/**
 * @brief SaharaWindow::~SaharaWindow
 */
SaharaWindow::~SaharaWindow()
{
	if (port.isOpen()) {
		port.close();
	}

    this->close();
    delete ui;
}

/**
 * @brief SaharaWindow::UpdatePortList
 */
void SaharaWindow::updatePortList()
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
 * @brief SaharaWindow::ConnectToPort
 */
void SaharaWindow::connectToPort()
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
    } catch(serial::IOException e) {
        log(tmp.sprintf("Error Connecting To Port %s", currentPort.port.c_str()));
        log(e.getErrorNumber() == 13 ? "Permission Denied. Try Running With Elevated Privledges." : e.what());
        return;
    }

	log(tmp.sprintf("Connected to %s", currentPort.port.c_str()));
	ui->portDisconnectButton->setEnabled(true);
}

/**
 * @brief SaharaWindow::readHello
 */
void SaharaWindow::readHello()
{
	if (!port.isOpen()) {
		log("Select a port first");
		return;
	}
	
	log("Reading Hello");

    if (!port.receiveHello()) {
        log("Did not receive hello. Not in sahara mode or requires restart.");
        return disconnectPort();
    }

    ui->deviceStateModeValue->setText(port.getNamedMode(port.deviceState.mode));

    QString tmp;
    log(tmp.sprintf("Device In Mode: %s", port.getNamedMode(port.deviceState.mode)));
    ui->deviceStateVersionValue->setText(tmp.sprintf("%i", port.deviceState.version));
    ui->deviceStateMinVersionValue->setText(tmp.sprintf("%i", port.deviceState.minVersion));
    ui->deviceStatePreferredMaxSizeValue->setText(tmp.sprintf("%i", port.deviceState.maxCommandPacketSize));

    int index = ui->writeHelloSwitchModeValue->findData(port.deviceState.mode);

    if ( index != -1 ) {
		ui->writeHelloSwitchModeValue->setCurrentIndex(index);
    }

}

/**
 * @brief SaharaWindow::writeHello
 */
void SaharaWindow::writeHello()
{
    if (!port.isOpen()) {
        log("Select a port and receive hello first");
        return;
    }

    QString tmp;

	uint32_t mode = ui->writeHelloSwitchModeValue->currentData().toUInt();
	uint32_t version = std::stoul(ui->writeHelloVersionValue->text().toStdString().c_str(), nullptr, 16);
	uint32_t minVersion = std::stoul(ui->writeHelloMinimumVersionValue->text().toStdString().c_str(), nullptr, 16);
	uint32_t status = 0x00;
	bool isSwitchMode = port.deviceState.mode != mode;

	if (!port.sendHello(mode, version, minVersion, status)) {
        log("Error Saying Hello");
        return disconnectPort();
    }

    ui->deviceStateModeValue->setText(port.getNamedMode(port.deviceState.mode));

	
	if (isSwitchMode) {
		log(tmp.sprintf("Device Switching Modes: %s", port.getNamedMode(mode)));
	}

    if (port.deviceState.mode == SAHARA_MODE_IMAGE_TX_PENDING) {
		ui->deviceStateRequestedImageValue->setText(port.getNamedRequestedImage(port.readState.imageId));
        log(tmp.sprintf("Device Requesting %lu Bytes of Image 0x%02x - %s", port.readState.length, port.readState.imageId, port.getNamedRequestedImage(port.readState.imageId)));
    }

	if (port.deviceState.mode == SAHARA_MEMORY_DEBUG) {
		ui->memoryDebugAddressValue->setText(tmp.sprintf("0x%08X", port.memoryState.address));
		ui->memoryDebugSizeValue->setText(tmp.sprintf("%lu", port.memoryState.length));
		log(tmp.sprintf("Device Saying Read Memory at address 0x%08X with length of %lu bytes", port.memoryState.address, port.memoryState.length));
	}
	
}

/**
 * @brief SaharaWindow::browseForImage
 */
void SaharaWindow::browseForImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select Image To Send", "", "Image Files (*.mbn *.bin *.img)");

    if (fileName.length()) {
        ui->sendImageFileValue->setText(fileName);
    }
}

/**
 * @brief SaharaWindow::sendImage
 */
void SaharaWindow::sendImage()
{
    if (!port.isOpen()) {
        log("Select a port and receive hello first.");
        return;
    }

	QString fileName = ui->sendImageFileValue->text();

    if (!fileName.length()) {
        log("Please Enter or Browse For a Image File");
        return;
    }

    if (port.readState.imageId == MBN_IMAGE_NONE) {
        log("Device has not requested an image");
        return;
    }

	if (!port.sendImage(fileName.toStdString())) {
        log("Error Sending Image");
		return;
    }

    log("Image Transfer Complete");
}

/**
 * @brief SaharaWindow::SwitchMode
 */
void SaharaWindow::switchMode()
{
    if (!port.isOpen()) {
        log("Select a port and receive hello first.");
        return;
    }

    QString tmp;
    uint16_t requestMode =  ui->switchModeValue->currentData().toUInt();

    log(tmp.sprintf("Requesting Mode Switch From 0x%02x - %s  to 0x%02x - %s",
        port.deviceState.mode,
        port.getNamedMode(port.deviceState.mode),
        requestMode,
        port.getNamedMode(requestMode)
    ));

    if (!port.switchMode(requestMode)) {
        log("Error Switching Modes");
        return disconnectPort();
    }

    log(tmp.sprintf("Device In Mode: %s", port.getNamedMode(port.deviceState.mode)));
    ui->deviceStateModeValue->setText(port.getNamedMode(port.deviceState.mode));
}

/**
 * @brief SaharaWindow::sendClientCommand
 *
 */
void SaharaWindow::sendClientCommand()
{
    if (!port.isOpen()) {
        log("Select a port and receive hello first and switch to client command mode.");
        return;
    }

    uint16_t requestedCommand =  ui->clientCommandValue->currentData().toUInt();

    uint8_t* readData = NULL;
    size_t readDataSize = 0;
    QString tmp;

    if (!port.sendClientCommand(requestedCommand, &readData, readDataSize)) {
        log("Error Sending Client Comand");
        return disconnectPort();
    }
		
	if (readData != NULL && readDataSize > 0) {
		
		log(tmp.sprintf("========\nDumping Data For Command: 0x%02x - %s - %lu Bytes\n========\n\n",
			requestedCommand, port.getNamedClientCommand(requestedCommand), readDataSize
			));

		logHex(readData, readDataSize);

		free(readData);
	}
}

/**
 * @brief SaharaWindow::sendReset
 */
void SaharaWindow::sendReset()
{
    if (!port.isOpen()) {
        log("Select a port and receive hello first.");
        return;
    }

    log("Sending Reset Command");

    if (!port.sendReset()) {
        log("Error Sending Reset");
        return disconnectPort();
    }

}


/**
 * @brief SaharaWindow::sendDone
 */
void SaharaWindow::sendDone()
{
    if (!port.isOpen()) {
        log("Select a port and receive hello first.");
        return;
    }
	
    log("Sending Done Command");

	try {
		if (!port.sendDone()) {
			log("Error Sending Done");
			return disconnectPort();
		}
	} catch (serial::IOException e) {
		log(e.what());
	}

	log("Done Command Successfully Sent");
}

/**
 * @brief SaharaWindow::disconnectPort
 */
void SaharaWindow::disconnectPort()
{
    if (port.isOpen()) {
		port.close();
		log("Port Closed");
		ui->deviceStateVersionValue->setText("Waiting Hello");
		ui->deviceStateMinVersionValue->setText("Waiting Hello");
		ui->deviceStatePreferredMaxSizeValue->setText("Waiting Hello");
		ui->deviceStateRequestedImageValue->setText("None");
		ui->memoryDebugAddressValue->setText("");
		ui->memoryDebugSizeValue->setText("");
		ui->portDisconnectButton->setEnabled(false);
	}
	
}


/**
* @brief SaharaWindow::memoryDebugRead
*/
void SaharaWindow::memoryDebugRead()
{
	if (!port.isOpen()) {
		log("Select a port and receive hello first.");
		return;
	}

	uint32_t address = std::stoul(ui->memoryDebugAddressValue->text().toStdString().c_str(), nullptr, 16);
	uint32_t size = std::stoul(ui->memoryDebugSizeValue->text().toStdString().c_str(), nullptr, 10);

	uint8_t* data;
	size_t dataSize = 0;

	if (!port.readMemory(address, size, &data, dataSize)) {
		log("Error Reading Memory");
		return;
	}

	log("Memory Read Complete");
}

/**
 * @brief SaharaWindow::sendStreamingDloadHello
 */
void SaharaWindow::sendStreamingDloadHello()
{

    if (!port.isOpen()) {
        log("Port Not Open");
        return;
    }

	if (!ui->streamingDloadHelloMagicValue->text().length()) {
        log("No Magic Set");
        return;
    }

    streaming_dload_hello_tx_t dloadHello;
    uint32_t outsize = 0;
    uint8_t* outbuf = NULL;
	std::string magic = ui->streamingDloadHelloMagicValue->text().toStdString();

    dloadHello.command = STREAMING_DLOAD_HELLO;
    memcpy(dloadHello.magic, magic.c_str(), magic.size());
    dloadHello.version = 0x04;
    dloadHello.compatibleVersion = 0x02;
    dloadHello.featureBits = 0x11;

    hdlc_request((uint8_t*)&dloadHello, sizeof(dloadHello), &outbuf, &outsize);

    size_t bytesWritten = port.write(outbuf, outsize);
    printf("Wrote %lu bytes\n", bytesWritten);
    hexdump(outbuf, bytesWritten);

    size_t bytesRead = port.read(port.buffer, port.bufferSize);
    printf("Read %lu bytes\n", bytesRead);
    hexdump(port.buffer, bytesRead);

    if (outbuf != NULL) {
        free(outbuf);
    }
}

/**
 * @brief SaharaWindow::ClearLog
 */
void SaharaWindow::clearLog()
{
   ui->log->clear();
}

/**
 * @brief SaharaWindow::SaveLog
 */
void SaharaWindow::saveLog()
{
   log("Not Implemented Yet");
}

/**
 * @brief SaharaWindow::log
 * @param message
 */
void SaharaWindow::log(const char* message)
{
	ui->log->appendPlainText(message);
}
/**
 * @brief SaharaWindow::log
 * @param message
 */
void SaharaWindow::log(std::string message)
{
    ui->log->appendPlainText(message.c_str());
}

/**
 * @brief SaharaWindow::log
 * @param message
 */
void SaharaWindow::log(QString message)
{
	ui->log->appendPlainText(message);
}

/**
 * @brief SaharaWindow::logTxHex
 * @param data
 * @param amount
 */
void SaharaWindow::logTxHex(uint8_t* data, size_t amount)
{

    QString tmp;
    log(tmp.sprintf("Dumping %lu bytes written", amount));
    printf(tmp.append("\n").toStdString().c_str());
    logHex(data, amount);
}

/**
 * @brief SaharaWindow::logRxHex
 * @param data
 * @param amount
 */
void SaharaWindow::logRxHex(uint8_t* data, size_t amount)
{

    QString tmp;
    log(tmp.sprintf("Dumping %lu bytes read", amount));
    printf(tmp.append("\n").toStdString().c_str());
    logHex(data, amount);
}

/**
 * @brief SaharaWindow::logHex
 * @param data
 * @param amount
 */
void SaharaWindow::logHex(uint8_t* data, size_t amount)
{   

    QString out;
    hexdump(data, amount, out);
    log(out);
    return;
}
