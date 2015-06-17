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
        log("Port is currently open.");
        return;
    }

    std::vector<serial::PortInfo> devices = serial::list_ports();
    std::vector<serial::PortInfo>::iterator iter = devices.begin();

    ui->portList->clear();
    ui->portList->addItem("- Select a Port -", 0);

    log("Scanning for available devices...");

    QString logMsg;

    while (iter != devices.end()) {
        serial::PortInfo device = *iter++;
        if (!strstr("n/a", device.hardware_id.c_str())) {
            ui->portList->addItem(device.description.c_str(), device.port.c_str());

            logMsg = "Found ";
			logMsg.append(device.hardware_id.c_str());
            logMsg.append(" on port ").append(device.port.c_str());

            if (device.description.length()) {
                logMsg.append(" - ").append(device.description.c_str());
            }

            log(logMsg);

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

    connectionText.append(currentPort.port.c_str()).append("...");

    log(connectionText);

    try {
        port.setPort(currentPort.port);

        if (!port.isOpen()){
            port.open();
        }
    } catch(serial::IOException e) {
        log("Error Connecting To Serial Port");
        log(e.getErrorNumber() == 13 ? "Permission Denied. Try Running With Elevated Privledges." : e.what());
        return;
    }

    QString connectedText = "Connected to ";

	connectedText.append(currentPort.port.c_str());
	log(connectedText);

}

/**
 * @brief QcdmWindow::DisconnectPort
 */
void QcdmWindow::DisconnectPort()
{
    log("Closing Port..");

    if (port.isOpen()) {
        port.close();
    }

    //ui->portDisconnectButton->setEnabled(false);
}



/**
 * @brief QcdmWindow::UpdatePortList
 */
void QcdmWindow::SecuritySendSpc()
{

    if (!port.isOpen()) {
        log("Connect to a Port First");
        return;
    }

    if (ui->securitySpcValue->text().length() != 6) {
        log("Enter a Valid 6 Digit SPC. You only entered");
        return;
    }

    int result = port.sendSpc(ui->securitySpcValue->text().toStdString().c_str());

    if (result < 0) {
        log ("Error Sending SPC");
        return;
    } else if (result != 1) {
        log("SPC Not Accepted");
        return;
    }

    log("SPC Accepted");
}



/**
 * @brief QcdmWindow::log
 * @param message
 */
void QcdmWindow::log(const char* message)
{
    ui->log->appendPlainText(message);
}
/**
 * @brief QcdmWindow::log
 * @param message
 */
void QcdmWindow::log(std::string message)
{
    ui->log->appendPlainText(message.c_str());
}

/**
 * @brief QcdmWindow::log
 * @param message
 */
void QcdmWindow::log(QString message)
{
    ui->log->appendPlainText(message);
}

