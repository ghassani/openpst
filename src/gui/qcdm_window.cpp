/**
* LICENSE PLACEHOLDER
*/

#include "qcdm_window.h"

QcdmWindow::QcdmWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QcdmWindow),
    port("", 115200)
{
    ui->setupUi(this);

    UpdatePortList();

    QObject::connect(ui->portListRefreshButton,        SIGNAL(clicked()), this, SLOT(UpdatePortList()));
    QObject::connect(ui->portDisconnectButton,         SIGNAL(clicked()), this, SLOT(DisconnectPort()));
    QObject::connect(ui->portConnectButton,            SIGNAL(clicked()), this, SLOT(ConnectToPort()));
    QObject::connect(ui->securitySendSpcButton,        SIGNAL(clicked()), this, SLOT(SecuritySendSpc()));
	QObject::connect(ui->securitySend16PasswordButton, SIGNAL(clicked()), this, SLOT(SecuritySend16Password()));

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

    if (result < 0) {
        log(LOGTYPE_ERROR, "Error Sending SPC");
        return;
    } else if (result != 1) {
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

	if (result < 0) {
		log(LOGTYPE_ERROR, "Error Sending 16 Digit Password");
		return;
	}
	else if (result != 1) {
		log(LOGTYPE_ERROR, "16 Digit Password Not Accepted");
		return;
	}

	log(LOGTYPE_INFO, "16 Digit Password Accepted");
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

