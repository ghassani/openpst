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

using namespace openpst;

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
	QObject::connect(ui->sendImageButton,				SIGNAL(clicked()), this, SLOT(sendImage()));
	QObject::connect(ui->memoryReadButton,				SIGNAL(clicked()), this, SLOT(memoryRead()));
	QObject::connect(ui->clearLogButton,				SIGNAL(clicked()), this, SLOT(clearLog()));
	QObject::connect(ui->saveLogButton,					SIGNAL(clicked()), this, SLOT(saveLog()));
	QObject::connect(ui->cancelOperationButton,			SIGNAL(clicked()), this, SLOT(cancelOperation()));
	
	qRegisterMetaType<sahara_memory_read_worker_request>("sahara_memory_read_worker_request");
	qRegisterMetaType<sahara_image_transfer_worker_request>("sahara_image_transfer_worker_request");

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

	log(tmp.sprintf("Found %lu devices", devices.size()));

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

	if (!port.readHello()) {
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
		log(tmp.sprintf("Device switching modes: %s", port.getNamedMode(mode)));
	}

    if (port.deviceState.mode == SAHARA_MODE_IMAGE_TX_PENDING) {
		ui->deviceStateRequestedImageValue->setText(port.getNamedRequestedImage(port.readState.imageId));
		log(tmp.sprintf("Device requesting %du bytes of image 0x%02X - %s", port.readState.size, port.readState.imageId, port.getNamedRequestedImage(port.readState.imageId)));
    }

	if (port.deviceState.mode == SAHARA_MODE_MEMORY_DEBUG) {
		log(tmp.sprintf("Memory table located at 0x%08X with size of %lu bytes", port.memoryState.memoryTableAddress, port.memoryState.memoryTableLength));
		
		uint8_t* memoryTableData = NULL;
		size_t memoryTableSize = 0;

		if (!port.readMemory(port.memoryState.memoryTableAddress, port.memoryState.memoryTableLength, &memoryTableData, memoryTableSize)) {
			log("Error reading memory table");
			return;
		}

		int totalRegions = memoryTableSize / sizeof(sahara_memory_table_entry_t);
		
		log(tmp.sprintf("Memory table references %d locations", totalRegions));
		
		sahara_memory_table_entry_t* entry;

		for (int i = 0; i < totalRegions; i++) {
			entry = (sahara_memory_table_entry_t*)&memoryTableData[i*sizeof(sahara_memory_table_entry_t)];
			log(tmp.sprintf("%s (%s) - Address: %08X Size: %u", entry->name, entry->filename, entry->address, entry->size));
		}

		QMessageBox::StandardButton userResponse = QMessageBox::question(this, "Memory Table", tmp.sprintf("Pull all %d files referenced in the memory table?", totalRegions));

		if (userResponse == QMessageBox::Yes) {

			QString dumpPath = QFileDialog::getExistingDirectory(this, tr("Select where to dump the files"), "");
			QString outFile;

			if (dumpPath.length()) {
				log("\n\n");
				for (int i = 0; i < totalRegions; i++) {
					entry = (sahara_memory_table_entry_t*)&memoryTableData[i*sizeof(sahara_memory_table_entry_t)];

					if (entry->size > 1000000) { // confirm files larger than 1mb
						QMessageBox::StandardButton largeFileUserResponse = QMessageBox::question(this, "Confirm Large File", tmp.sprintf("Pull large file %s (%lu bytes) or skip it?", entry->filename, entry->size));

						if (largeFileUserResponse != QMessageBox::Yes) {
							log(tmp.sprintf("Skipping %s - %s", entry->filename, entry->name));
							continue;
						}
					}

					outFile.sprintf("%s/%s", dumpPath.toStdString().c_str(), entry->filename);

					// queue a read request
					sahara_memory_read_worker_request memoryReadWorkerRequest;
					memoryReadWorkerRequest.address		= entry->address;
					memoryReadWorkerRequest.size		= entry->size;
					memoryReadWorkerRequest.outFilePath = outFile.toStdString();

					memoryReadQueue.push_back(memoryReadWorkerRequest);

				}

			} else {
				log("Dump all cancelled");
			}
		}

		QMessageBox::StandardButton saveMemoryTableResponse = QMessageBox::question(this, "Save Memory Table", "Save the raw memory table to a file?");

		if (saveMemoryTableResponse == QMessageBox::Yes) {
			QString memoryTableFileName = QFileDialog::getSaveFileName(this, tr("Save Raw Memory Table"), "", tr("Binary Files (*.bin)"));
			if (memoryTableFileName.length()) {
#ifdef _WIN32
				FILE* fp;
				errno_t err = fopen_s(&fp, memoryTableFileName.toStdString().c_str(), "a+b");
#else
				FILE* fp = fopen(memoryTableFileName.toStdString().c_str(), "a+b");
#endif				
				if (fp) {
					fwrite(memoryTableData, sizeof(uint8_t), memoryTableSize, fp);
					fclose(fp);
				} else {
					log("Error opening memory table file for writing");
				}
			} else {
				log("Save raw memory table cancelled");
			}
		}

		free(memoryTableData);

		if (memoryReadQueue.size()) {
			return memoryReadStartThread();
		}
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

	sahara_image_transfer_worker_request request;
	request.imageType = port.readState.imageId;
	request.imagePath = fileName.toStdString();
	request.offset = port.readState.offset;
	request.chunkSize = port.readState.size;

#ifdef _WIN32
	FILE* fp;
	errno_t err = fopen_s(&fp, fileName.toStdString().c_str(), "rb");
#else
	FILE* fp = fopen(fileName.toStdString().c_str(), "rb");
#endif	
	
	// get file size
	fseek(fp, 0, SEEK_END);
	request.fileSize = ftell(fp);
	fclose(fp);

	QString tmp;

	// setup progress bar
	ui->progressBar->reset();
	ui->progressBar->setMaximum(request.fileSize);
	ui->progressBar->setMinimum(0);
	ui->progressBar->setValue(0);

	ui->progressBarTextLabel2->setText(tmp.sprintf("%s", request.imagePath.c_str()));
	ui->progressBarTextLabel->setText(tmp.sprintf("0 / %lu bytes", request.fileSize));

	disableControls();

	imageTransferWorker = new SaharaImageTransferWorker(port, request, this);
	connect(imageTransferWorker, &SaharaImageTransferWorker::chunkDone, this, &SaharaWindow::imageTransferChunkDoneHandler, Qt::QueuedConnection);
	connect(imageTransferWorker, &SaharaImageTransferWorker::complete, this, &SaharaWindow::imageTransferCompleteHandler);
	connect(imageTransferWorker, &SaharaImageTransferWorker::error, this, &SaharaWindow::imageTransferErrorHandler);
	connect(imageTransferWorker, &SaharaImageTransferWorker::finished, imageTransferWorker, &QObject::deleteLater);
	
	imageTransferWorker->start();

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
		
		if (requestedCommand == SAHARA_EXEC_CMD_OEM_PK_HASH_READ) {
			sahara_oem_pk_hash_rx_t* resp = (sahara_oem_pk_hash_rx_t*)readData;		
			hexdump(resp->hash, sizeof(sahara_oem_pk_hash_rx_t), tmp);
			log(tmp.sprintf("OEM Public Key Hash Hex:\n %s", tmp.toStdString().c_str()));
		} else if (requestedCommand == SAHARA_EXEC_CMD_GET_SOFTWARE_VERSION_SBL) {
			sahara_sbl_version_rx_t* resp = (sahara_sbl_version_rx_t*)readData;
			log(tmp.sprintf("SBL SW Version: %u", resp->version));
		} else if (requestedCommand == SAHARA_EXEC_CMD_SERIAL_NUM_READ) {
			sahara_serial_number_rx_t* resp = (sahara_serial_number_rx_t*)readData;
			log(tmp.sprintf("Serial Number: %u - %08X", resp->serial, resp->serial));
		} else if (requestedCommand == SAHARA_EXEC_CMD_MSM_HW_ID_READ) {
			sahara_msm_hw_id_rx_t* resp = (sahara_msm_hw_id_rx_t*)readData;
			log(tmp.sprintf("Unknown ID 1: %u", resp->unknown1));
			log(tmp.sprintf("Unknown ID 2: %u", resp->unknown2));
			log(tmp.sprintf("MSM HW ID: %u - %08X", resp->msmId, resp->msmId));
		}
		else {
			log(tmp.sprintf("========\nDumping Data For Command: 0x%02x - %s - %lu Bytes\n========\n\n",
				requestedCommand, port.getNamedClientCommand(requestedCommand), readDataSize
			));
			tmp.clear();
			log(tmp);
		}

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
    }

	return disconnectPort();
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
	port.close();
	log("Port Closed");
	ui->deviceStateVersionValue->setText("Waiting Hello");
	ui->deviceStateMinVersionValue->setText("Waiting Hello");
	ui->deviceStatePreferredMaxSizeValue->setText("Waiting Hello");
	ui->deviceStateRequestedImageValue->setText("None");
	ui->memoryReadAddressValue->setText("");
	ui->memoryReadSizeValue->setText("");
	ui->portDisconnectButton->setEnabled(false);	
}


/**
* @brief SaharaWindow::memoryRead
*/
void SaharaWindow::memoryRead()
{
	if (!port.isOpen()) {
		log("Select a port and receive hello first.");
		return;
	}

	if (!ui->memoryReadAddressValue->text().length()) {
		log("Enter a starting read address in hexidecimal format");
		return;
	}

	if (!ui->memoryReadSizeValue->text().length()) {
		log("Enter an amount in bytes to read");
		return;
	}

	uint32_t address = std::stoul(ui->memoryReadAddressValue->text().toStdString().c_str(), nullptr, 16);
	uint32_t size = std::stoul(ui->memoryReadSizeValue->text().toStdString().c_str(), nullptr, 10);
	
	QString tmp;	

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Read Data"), "", tr("Binary Files (*.bin)"));

	if (!fileName.length()) {
		log("No file set to save memory content. Operation cancelled");
		return;
	}

	log(tmp.sprintf("Reading %lu bytes from address 0x%08X", size, address));

#ifdef _WIN32
	FILE* fp;
	errno_t err = fopen_s(&fp, fileName.toStdString().c_str(), "a+b");
#else
	FILE* fp = fopen(fileName.toStdString().c_str(), "a+b");
#endif	

	if (!fp) {
		log(tmp.sprintf("Error opening file %s for writing", fileName.toStdString().c_str()));
		return;
	}

	// queue a read request and setup the worker
	sahara_memory_read_worker_request memoryReadWorkerRequest;
	memoryReadWorkerRequest.address = address;
	memoryReadWorkerRequest.size = size;
	memoryReadWorkerRequest.outFilePath = fileName.toStdString();

	memoryReadQueue.push_front(memoryReadWorkerRequest);

	memoryReadStartThread();
}

void SaharaWindow::memoryReadChunkReadyHandler(sahara_memory_read_worker_request request)
{
	// update progress bar
	QString tmp;
	ui->progressBar->setValue(ui->progressBar->value() + request.lastChunkSize);	
	ui->progressBarTextLabel->setText(tmp.sprintf("%lu / %lu bytes", ui->progressBar->value(), request.size));
}

void SaharaWindow::memoryReadCompleteHandler(sahara_memory_read_worker_request request)
{
	QString tmp;
	
	if (memoryReadQueue.size()) { // in case it was cancelled, would have been emptied
		memoryReadQueue.pop_front();
	}
	
	log(tmp.sprintf("Memory read complete. Contents dumped to %s. Final size is %lu bytes", request.outFilePath.c_str(), request.outSize));
	
	memoryReadWorker = NULL;

	memoryReadStartThread();
}

void SaharaWindow::memoryReadChunkErrorHandler(sahara_memory_read_worker_request request, QString msg)
{
	if (memoryReadQueue.size()) { // in case it was cancelled, would have been emptied
		memoryReadQueue.pop_front();
	}

	log(msg);

	if (memoryReadQueue.size() == 0) {
		enableControls();
		return;
	}

	memoryReadWorker = NULL;

	memoryReadStartThread();
}

void SaharaWindow::memoryReadStartThread()
{

	if (memoryReadQueue.size() == 0) {		 
		enableControls();
		return;
	}
	
	sahara_memory_read_worker_request request = memoryReadQueue.front();
	QString tmp;

	// setup progress bar
	ui->progressBar->reset();
	ui->progressBar->setMaximum(request.size);
	ui->progressBar->setMinimum(0);
	ui->progressBar->setValue(0);

	ui->progressBarTextLabel2->setText(tmp.sprintf("%s", request.outFilePath.c_str()));
	ui->progressBarTextLabel->setText(tmp.sprintf("0 / %lu bytes", request.size));

	disableControls(); 
	memoryReadWorker = new SaharaMemoryReadWorker(port, request, this);
	connect(memoryReadWorker, &SaharaMemoryReadWorker::chunkReady, this, &SaharaWindow::memoryReadChunkReadyHandler, Qt::QueuedConnection);
	connect(memoryReadWorker, &SaharaMemoryReadWorker::complete,   this, &SaharaWindow::memoryReadCompleteHandler);
	connect(memoryReadWorker, &SaharaMemoryReadWorker::error,      this, &SaharaWindow::memoryReadChunkErrorHandler);
	connect(memoryReadWorker, &SaharaMemoryReadWorker::finished, memoryReadWorker, &QObject::deleteLater);
	memoryReadWorker->start();
	
}


void SaharaWindow::imageTransferChunkDoneHandler(sahara_image_transfer_worker_request request)
{
	// update progress bar
	QString tmp;
	ui->progressBar->setValue(ui->progressBar->value() + request.chunkSize);
	ui->progressBarTextLabel->setText(tmp.sprintf("%lu / %lu bytes", ui->progressBar->value(), request.fileSize));
}

/**
* @brief imageTransferCompleteHandler
*/
void SaharaWindow::imageTransferCompleteHandler(sahara_image_transfer_worker_request request)
{
	QString tmp;
	
	log(tmp.sprintf("Successfully transfered %s", request.imagePath.c_str()));

	memoryReadWorker = NULL;

	enableControls();

	QMessageBox::StandardButton userResponse = QMessageBox::question(this, "Confirmation", "Would you like to send the done command?");

	if (userResponse == QMessageBox::Yes) {
		return sendDone();
	}
}

/**
* @brief imageTransferErrorHandler
*/
void SaharaWindow::imageTransferErrorHandler(sahara_image_transfer_worker_request request, QString msg)
{
	log(msg);

	enableControls();
	
	imageTransferWorker = NULL;
}

void SaharaWindow::cancelOperation()
{

	if (NULL != memoryReadWorker && memoryReadWorker->isRunning()) {
		QMessageBox::StandardButton userResponse = QMessageBox::question(this, "Confirm", "Really cancel operation?");

		if (userResponse == QMessageBox::Yes) {
			memoryReadWorker->cancel();
			if (!memoryReadWorker->wait(5000)) {
				memoryReadWorker->terminate();
				memoryReadWorker->wait();
			}

			if (memoryReadQueue.size()) {
				memoryReadQueue.clear();
			}

			log("Memory read cancelled");
		}
	} else if (NULL != imageTransferWorker && imageTransferWorker->isRunning()) {
		QMessageBox::StandardButton userResponse = QMessageBox::question(this, "Confirm", "Really cancel operation?");

		if (userResponse == QMessageBox::Yes) {
			imageTransferWorker->cancel();
			if (!imageTransferWorker->wait(5000)) {
				imageTransferWorker->terminate();
				imageTransferWorker->wait();
			}
			log("Image transfer cancelled");
		}
	} else {
		log("No operation currently running");
	}


	enableControls();
}

void SaharaWindow::disableControls()
{
	ui->mainTabSet->setEnabled(false);
	ui->cancelOperationButton->setEnabled(true);
}

void SaharaWindow::enableControls()
{
	ui->mainTabSet->setEnabled(true);	
	ui->progressBarTextLabel2->setText("");
	ui->progressBarTextLabel->setText("");
	ui->progressBar->setValue(0);
	ui->cancelOperationButton->setEnabled(false);
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
	
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Log"), "", tr("Log Files (*.log)"));

	if (!fileName.length()) {
		return;
	}

#ifdef _WIN32
	FILE* fp;
	errno_t err = fopen_s(&fp, fileName.toStdString().c_str(), "a+");
#else
	FILE* fp = fopen(fileName.toStdString().c_str(), "a+");
#endif

	if (!fp) {
		log("Error opening file for writing");
	}

	QString content = ui->log->toPlainText();

	fwrite(content.toStdString().c_str(), sizeof(uint8_t), content.size(), fp);
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
