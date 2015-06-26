/**
* LICENSE PLACEHOLDER
*
* @file qcdm_window.cpp
* @class QcdmWindow
* @package OpenPST
* @brief QCDM GUI interface class
*
* @author Gassan Idriss <ghassani@gmail.com>
* @author Matteson Raab <mraabhimself@gmail.com>
*/

#include "qcdm_window.h"

QcdmWindow::QcdmWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QcdmWindow),
    port("", 115200)
{
    ui->setupUi(this);

    UpdatePortList();

	ui->readSpcMethod->addItem("Default", 0);
	ui->readSpcMethod->addItem("EFS", 1);
	ui->readSpcMethod->addItem("HTC", 2);
	ui->readSpcMethod->addItem("LG", 3);
	ui->readSpcMethod->addItem("Samsung", 4);

    QObject::connect(ui->portListRefreshButton,        SIGNAL(clicked()), this, SLOT(UpdatePortList()));
    QObject::connect(ui->portDisconnectButton,         SIGNAL(clicked()), this, SLOT(DisconnectPort()));
    QObject::connect(ui->portConnectButton,            SIGNAL(clicked()), this, SLOT(ConnectToPort()));
    QObject::connect(ui->securitySendSpcButton,        SIGNAL(clicked()), this, SLOT(SecuritySendSpc()));
	QObject::connect(ui->securitySend16PasswordButton, SIGNAL(clicked()), this, SLOT(SecuritySend16Password()));
	QObject::connect(ui->readMeidButton,			   SIGNAL(clicked()), this, SLOT(nvReadGetMeid()));
	QObject::connect(ui->readImeiButton,			   SIGNAL(clicked()), this, SLOT(nvReadGetImei()));
	QObject::connect(ui->readSpcButton,				   SIGNAL(clicked()), this, SLOT(nvReadGetSpc()));
	QObject::connect(ui->writeSpcButton,			   SIGNAL(clicked()), this, SLOT(nvWriteSetSpc()));
}

/**
 * @brief QcdmWindow::~QcdmWindow
 */
QcdmWindow::~QcdmWindow()
{
    this->close();
    delete ui;
}


/**
 * @brief QcdmWindow::UpdatePortList
 */
void QcdmWindow::UpdatePortList()
{
    if (port.isOpen()) {
        log(LOGTYPE_WARNING, "Port is currently open. Disconnect before Refresh...");
        return;
    }

    std::vector<serial::PortInfo> devices = serial::list_ports();
    std::vector<serial::PortInfo>::iterator iter = devices.begin();

    ui->portList->clear();
    ui->portList->addItem("- Select a Port -", 0);

    log(LOGTYPE_INFO, "Scanning for Available Devices");

    while (iter != devices.end()) {
        serial::PortInfo device = *iter++;
        if (!strstr("n/a", device.hardware_id.c_str())) {
            ui->portList->addItem(device.description.c_str(), device.port.c_str());

			QString logMsg = "Found ";
			logMsg.append(device.hardware_id.c_str()).append(" on port ").append(device.port.c_str());

            if (device.description.length()) {
                logMsg.append(" - ").append(device.description.c_str());
            }

            log(LOGTYPE_DEBUG, logMsg);

        }
    }
}

/**
 * @brief QcdmWindow::ConnectToPort
 */
void QcdmWindow::ConnectToPort()
{
    QString selected = ui->portList->currentData().toString();

    if (selected.compare("0") == 0) {
        log(LOGTYPE_WARNING, "Select a Port First");
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
        log(LOGTYPE_ERROR, "Invalid Port Type");
        return;
    }

    QString connectionText = "Connecting to ";
    connectionText.append(currentPort.port.c_str()).append("...");
    log(LOGTYPE_INFO, connectionText);

    try {
        port.setPort(currentPort.port);

        if (!port.isOpen()){
            port.open();

			QString connectedText = "Connected to ";
			connectedText.append(currentPort.port.c_str());
			log(LOGTYPE_INFO, connectedText);
        }
    } catch(serial::IOException e) {
        log(LOGTYPE_ERROR, "Error Connecting To Serial Port");
		log(LOGTYPE_ERROR, e.getErrorNumber() == 13 ? "Permission Denied. Try Running With Elevated Privledges." : e.what());
        return;
    }
}

/**
 * @brief QcdmWindow::DisconnectPort
 */
void QcdmWindow::DisconnectPort()
{
    if (port.isOpen()) {
		QString closeText = "Disconnected from ";
		closeText.append(currentPort.port.c_str());
		log(LOGTYPE_INFO, closeText);

        port.close();
    }

    //ui->portDisconnectButton->setEnabled(false);
}



/**
 * @brief QcdmWindow::SecuritySendSpc
 */
void QcdmWindow::SecuritySendSpc()
{
    if (!port.isOpen()) {
        log(LOGTYPE_WARNING, "Connect to a Port First");
        return;
    }

    if (ui->securitySpcValue->text().length() != 6) {
        log(LOGTYPE_WARNING, "Enter a Valid 6 Digit SPC");
        return;
    }

    int result = port.sendSpc(ui->securitySpcValue->text().toStdString().c_str());

	if (result == DIAG_CMD_NO_RESPONSE || result == DIAG_CMD_WRITE_FAIL) {
        log(LOGTYPE_ERROR, "Error Sending SPC");
        return;
    }
	
	if (result == DIAG_SPC_REJECT) {
        log(LOGTYPE_ERROR, "SPC Not Accepted");
        return;
    }

    log(LOGTYPE_INFO, "SPC Accepted");
}

/**
* @brief QcdmWindow::SecuritySend16Password
*/
void QcdmWindow::SecuritySend16Password()
{
	if (!port.isOpen()) {
		log(LOGTYPE_WARNING, "Connect to a Port First");
		return;
	}

	if (ui->security16PasswordValue->text().length() != 16) {
		log(LOGTYPE_WARNING, "Enter a Valid 16 Digit Password");
		return;
	}

	int result = port.send16Password(ui->security16PasswordValue->text().toStdString().c_str());

	if (result == DIAG_CMD_NO_RESPONSE || result == DIAG_CMD_WRITE_FAIL) {
		log(LOGTYPE_ERROR, "Error Sending 16 Digit Password");
		return;
	}

	if (result == DIAG_PASSWORD_REJECT) {
		log(LOGTYPE_ERROR, "16 Digit Password Not Accepted");
		return;
	}

	log(LOGTYPE_INFO, "16 Digit Password Accepted");
}

/**
* @brief QcdmWindow::nvReadGetMeid
*/
void QcdmWindow::nvReadGetMeid() 
{
	if (!port.isOpen()) {
		log(LOGTYPE_WARNING, "Connect to a Port First");
		return;
	}

	if (ui->hexMeidValue->text().length() != 0) {
		ui->hexMeidValue->setText("");
	}

	uint8_t* response = NULL;

	int result = port.getNvItem(1943, &response);

	if (result == DIAG_NV_READ_F){
		int p;
		QString meidValue, tmp;

		qcdm_nv_rx_t* rxPacket = (qcdm_nv_rx_t*)response;

		for (p = 6; p >= 0; p--) {
			tmp.sprintf("%02x", rxPacket->data[p]);
			meidValue.append(tmp);
		}

		meidValue = meidValue.toUpper();

		ui->hexMeidValue->setText(meidValue);

		log(LOGTYPE_INFO, "Read Success - MEID: " + meidValue);
	} else {
		log(LOGTYPE_ERROR, "Read Failure - MEID");
	}
}

/**
* @brief QcdmWindow::nvReadGetImei
*/
void QcdmWindow::nvReadGetImei() 

{
	if (!port.isOpen()) {
		log(LOGTYPE_WARNING, "Connect to a Port First");
		return;
	}

	if (ui->imeiValue->text().length() != 0) {
		ui->imeiValue->setText("");
	}

	uint8_t* response = NULL;

	int result = port.getNvItem(550, &response);

	if (result == DIAG_NV_READ_F) {
		int p;
		QString imeiValue, tmp;

		qcdm_nv_rx_t* rxPacket = (qcdm_nv_rx_t*)response;

		for (p = 1; p <= 8; p++) {
			tmp.sprintf("%02x", rxPacket->data[p]);
			imeiValue.append(tmp);
		}

		imeiValue = imeiValue.remove("a");

		if (imeiValue != "0000000000000000") {
			ui->imeiValue->setText(imeiValue);

			log(LOGTYPE_INFO, "Read Success - IMEI: " + imeiValue);
		} else {
			log(LOGTYPE_ERROR, "Read Failure - IMEI");
		}
	} else {
		log(LOGTYPE_ERROR, "Read Failure - IMEI");
	}
}

/**
* @brief QcdmWindow::nvReadGetSpc
*/
void QcdmWindow::nvReadGetSpc()
{
	if (!port.isOpen()) {
		log(LOGTYPE_WARNING, "Connect to a Port First");
		return;
	}

	if (ui->hexSpcValue->text().length() != 0 || ui->readSpcValue->text().length() != 0) {
		ui->hexSpcValue->setText("");
		ui->readSpcValue->setText("");
	}

	uint8_t* response = NULL;

	int result = 0;

	switch (ui->readSpcMethod->currentData().toInt()) {
	case 0:
		result = port.getNvItem(85, &response);
		break;
	case 1:
		// EFS Method

		break;
	case 2:
		port.sendHtcNvUnlock(&response); // HTC Method
		result = port.getNvItem(85, &response);
		break;
	case 3:
		port.sendLgNvUnlock(&response); // LG Method
		result = port.getLgSpc(&response);
		break;
	case 4:
		// Samsung Method

		break;
	}

	if (result == DIAG_NV_READ_F){
		int p;
		QString hexSpcValue, tmp;

		qcdm_nv_rx_t* rxPacket = (qcdm_nv_rx_t*)response;

		for (p = 0; p <= 5; p++) {
			tmp.sprintf("%02x", rxPacket->data[p]);
			hexSpcValue.append(tmp);
		}

		QString readSpcValue = QString::fromStdString(port.transformHexToString((const char *)rxPacket->data, 5));

		ui->hexSpcValue->setText(hexSpcValue);
		ui->readSpcValue->setText(readSpcValue);

		log(LOGTYPE_INFO, "Read Success - SPC: " + readSpcValue);
	} else {
		log(LOGTYPE_ERROR, "Read Failure - SPC");
	}
}

/**
* @brief QcdmWindow::nvWriteSetSpc
*/
void QcdmWindow::nvWriteSetSpc()
{
	if (!port.isOpen()) {
		log(LOGTYPE_WARNING, "Connect to a Port First");
		return;
	}

	if (ui->readSpcValue->text().length() != 6) {
		log(LOGTYPE_WARNING, "Enter a Valid 6 Digit SPC");
		return;
	}

	uint8_t* response = NULL;

	int result = port.setNvItem(85, ui->readSpcValue->text().toStdString().c_str(), 6, &response);

	if (result == DIAG_NV_WRITE_F) {
		log(LOGTYPE_INFO, "Write Success - SPC: " + ui->readSpcValue->text());

		nvReadGetSpc();
	} else {
		log(LOGTYPE_ERROR, "Write Failure - SPC");
	}
}


/**
 * @brief QcdmWindow::log
 * @param message
 */
void QcdmWindow::log(int type, const char* message)
{
	//ui->log->appendPlainText(message);
	QString newMessage = message;
	log(type, newMessage);
}
/**
 * @brief QcdmWindow::log
 * @param message
 */
void QcdmWindow::log(int type, std::string message)
{
    //ui->log->appendPlainText(message.c_str());
	QString newMessage = message.c_str();
	log(type, newMessage);
}

/**
 * @brief QcdmWindow::log
 * @param type
 * @param message
 */
void QcdmWindow::log(int type, QString message)
{
	QString logColorDebug = "<font color=\"gray\">";
	QString logColorError = "<font color=\"red\">";
	QString logColorInfo = "<font color=\"green\">";
	QString logColorWarning = "<font color=\"orange\">";
	QString logColorSuffix = "</font>";

	switch (type) {
	case 0:
		message = logColorDebug.append(message).append(logColorSuffix);
		break;
	case -1:
		message = logColorError.append(message).append(logColorSuffix);
		break;
	case 1:
		message = logColorInfo.append(message).append(logColorSuffix);
		break;
	case 2:
		message = logColorWarning.append(message).append(logColorSuffix);
		break;
	}

	ui->log->appendHtml(message);
}

