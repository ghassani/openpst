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

	/*STREAMING_DLOAD_SECURITY_MODE_UNTRUSTED

		ui->streamingDloadSecurityModeValue->set*/
	
	ui->streamingDloadHelloFeatureBitValue->addItem("0x01 - Uncompressed Download", STREAMING_DLOAD_FEATURE_BIT_UNCOMPRESSED_DOWNLOAD);
	ui->streamingDloadHelloFeatureBitValue->addItem("0x02 - NAND Bootable Image", STREAMING_DLOAD_FEATURE_BIT_NAND_BOOTABLE_IMAGE);
	ui->streamingDloadHelloFeatureBitValue->addItem("0x04 - NAND Bootloader", STREAMING_DLOAD_FEATURE_BIT_NAND_BOOT_LOADER);
	ui->streamingDloadHelloFeatureBitValue->addItem("0x08 - Multi Image", STREAMING_DLOAD_FEATURE_BIT_MULTI_IMAGE);
	ui->streamingDloadHelloFeatureBitValue->addItem("0x10 - Sector Address", STREAMING_DLOAD_FEATURE_BIT_SECTOR_ADDRESSES);
	
	ui->streamingDloadSecurityModeValue->addItem("0x01 - Trusted", STREAMING_DLOAD_SECURITY_MODE_TRUSTED);
	ui->streamingDloadSecurityModeValue->addItem("0x00 - Untrusted", STREAMING_DLOAD_SECURITY_MODE_UNTRUSTED);

	UpdatePortList();

	QObject::connect(ui->portRefreshButton, SIGNAL(clicked()), this, SLOT(UpdatePortList()));
	QObject::connect(ui->portDisconnectButton, SIGNAL(clicked()), this, SLOT(DisconnectPort()));
	QObject::connect(ui->portConnectButton, SIGNAL(clicked()), this, SLOT(ConnectToPort()));	
	QObject::connect(ui->streamingDloadHelloButton, SIGNAL(clicked()), this, SLOT(SendHello()));	
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
		log("Port is currently open.");
		return;
	}

	std::vector<serial::PortInfo> devices = serial::list_ports();
	std::vector<serial::PortInfo>::iterator iter = devices.begin();

	ui->portList->clear();
	ui->portList->addItem("- Select a Port -", 0);

	log("Rescanning for available devices..");

	QString logMsg;

	while (iter != devices.end()) {
		serial::PortInfo device = *iter++;
		if (!strstr("n/a", device.hardware_id.c_str())) {
			ui->portList->addItem(device.port.c_str(), device.port.c_str());

			logMsg = device.hardware_id.c_str();

			logMsg.append(" on port ").append(device.port.c_str());

			std::cout << device.port.c_str() << std::endl;
			std::cout << device.description.c_str() << std::endl;
			if (device.description.length()) {
				logMsg.append(" - ").append(device.description.c_str());
			}

			log(logMsg);

		}
	}
}

/**
* @brief StreamingDloadWindow::ConnectToPort
*/
void StreamingDloadWindow::ConnectToPort()
{
	QString selected = ui->portList->currentData().toString();

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

	QString connectionText = "Connecting to ";

	connectionText.append(currentPort.port.c_str()).append(" ...");

	log(connectionText);

	try {
		port.setPort(currentPort.port);

		if (!port.isOpen()){
			port.open();
		}
	}
	catch (serial::IOException e) {
		log("Error Connecting To Serial Port");
		log(e.getErrorNumber() == 13 ? "Permission Denied. Try Running With Elevated Privledges." : e.what());
		return;
	}

	log("Connected");


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

	if (!ui->streamingDloadHelloMagicValue->text().length()) {
		log("No Magic Set");
		return;
	}


	streaming_dload_hello_tx_t dloadHello;
	uint32_t outsize = 0;
	uint8_t* outbuf = NULL;

	uint8_t version = atoi(ui->streamingDloadHelloVersionValue->currentText().toStdString().c_str());
	uint8_t compatVersion = atoi(ui->streamingDloadHelloCompatVersionValue->currentText().toStdString().c_str());
	uint8_t feature = atoi(ui->streamingDloadHelloFeatureBitValue->currentText().toStdString().c_str());

	printf("Result: %02x %02x %02x\n", version, compatVersion, feature);
	/*
	std::string magic = ui->streamingDloadHelloMagicValue->text().toStdString().c_str();

	dloadHello.command = STREAMING_DLOAD_HELLO;
	memcpy(dloadHello.magic, magic.c_str(), magic.size());
	dloadHello.version			 = atoi(ui->streamingDloadHelloVersionValue->currentData());
	dloadHello.compatibleVersion = atoi(ui->streamingDloadHelloCompatVersionValue->text().toStdString().c_str());
	dloadHello.featureBits		 = atoi(ui->streamingDloadHelloFeatureBitsValue->text().toStdString().c_str());

	hdlc_request((uint8_t*)&dloadHello, sizeof(dloadHello), &outbuf, &outsize);

	size_t bytesWritten = port.write(outbuf, outsize);
	printf("Wrote %zd bytes\n", bytesWritten);
	hexdump(outbuf, bytesWritten);

	size_t bytesRead = port.read(port.buffer, port.bufferSize);
	printf("Read %zd bytes\n", bytesRead);
	hexdump(port.buffer, bytesRead);

	if (outbuf != NULL) {
		free(outbuf);
	}*/

	//ui->portDisconnectButton->setEnabled(false);
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
void StreamingDloadWindow::log(std::string message)
{
	ui->log->appendPlainText(message.c_str());
}

/**
* @brief StreamingDloadWindow::log
* @param message
*/
void StreamingDloadWindow::log(QString message)
{
	ui->log->appendPlainText(message);
}

/**
* @brief StreamingDloadWindow::logTxHex
* @param data
* @param amount
*/
void StreamingDloadWindow::logTxHex(uint8_t* data, size_t amount)
{

	QString tmp;
	log(tmp.sprintf("Dumping %zd bytes written", amount));
	printf(tmp.append("\n").toStdString().c_str());
	logHex(data, amount);
}

/**
* @brief StreamingDloadWindow::logRxHex
* @param data
* @param amount
*/
void StreamingDloadWindow::logRxHex(uint8_t* data, size_t amount)
{

	QString tmp;
	log(tmp.sprintf("Dumping %zd bytes read", amount));
	printf(tmp.append("\n").toStdString().c_str());
	logHex(data, amount);
}

/**
* @brief StreamingDloadWindow::logHex
* @param data
* @param amount
*/
void StreamingDloadWindow::logHex(uint8_t* data, size_t amount)
{

	QString out;
	hexdump(data, amount, out);
	log(out);
	return;
}
