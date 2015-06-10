#include "sahara_window.h"

#include <iostream>
#include <stdio.h>

SaharaWindow::SaharaWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SaharaWindow),
    port("", 115200)
{
    ui->setupUi(this);

    ui->saharaSayHelloSwitchCommandSelect->addItem("", -1);
    ui->saharaSayHelloSwitchCommandSelect->addItem("Image Transfer Pending",   SAHARA_MODE_IMAGE_TX_PENDING);
    ui->saharaSayHelloSwitchCommandSelect->addItem("Image Transfer Complete",  SAHARA_MODE_IMAGE_TX_COMPLETE);
    ui->saharaSayHelloSwitchCommandSelect->addItem("Memory Debug",             SAHARA_MODE_MEMORY_DEBUG);
    ui->saharaSayHelloSwitchCommandSelect->addItem("Client Command Mode",      SAHARA_MODE_COMMAND);

    ui->saharaSendClientCommandSelect->addItem("", -1);
    ui->saharaSendClientCommandSelect->addItem("NOP",                       SAHARA_EXEC_CMD_NOP);
    ui->saharaSendClientCommandSelect->addItem("Read Serial Number",        SAHARA_EXEC_CMD_SERIAL_NUM_READ);
    ui->saharaSendClientCommandSelect->addItem("Read MSM HW ID",            SAHARA_EXEC_CMD_MSM_HW_ID_READ);
    ui->saharaSendClientCommandSelect->addItem("Read OEM PK Hash",          SAHARA_EXEC_CMD_OEM_PK_HASH_READ);
    ui->saharaSendClientCommandSelect->addItem("Switch To DMSS DLOAD",      SAHARA_EXEC_CMD_SWITCH_TO_DMSS_DLOAD);
    ui->saharaSendClientCommandSelect->addItem("Switch To Streaming DLOAD", SAHARA_EXEC_CMD_SWITCH_TO_STREAM_DLOAD);
    ui->saharaSendClientCommandSelect->addItem("Read Debug Data",           SAHARA_EXEC_CMD_READ_DEBUG_DATA);
    ui->saharaSendClientCommandSelect->addItem("Get SBL SW Version",        SAHARA_EXEC_CMD_GET_SOFTWARE_VERSION_SBL);

    ui->saharaSwitchModeSelect->addItem("", -1);
    ui->saharaSwitchModeSelect->addItem("Image Transfer Pending",   SAHARA_MODE_IMAGE_TX_PENDING);
    ui->saharaSwitchModeSelect->addItem("Image Transfer Complete",  SAHARA_MODE_IMAGE_TX_COMPLETE);
    ui->saharaSwitchModeSelect->addItem("Memory Debug",             SAHARA_MODE_MEMORY_DEBUG);
    ui->saharaSwitchModeSelect->addItem("Client Command Mode",      SAHARA_MODE_COMMAND);

    ui->saharaSendCommandSelect->addItem("", -1);
    ui->saharaSendCommandSelect->addItem("Done",        SAHARA_DONE);
    ui->saharaSendCommandSelect->addItem("Reset",       SAHARA_RESET);

    UpdatePortList();

    QObject::connect(ui->portRefreshButton,             SIGNAL(clicked()), this, SLOT(UpdatePortList()));
    QObject::connect(ui->saharaReceiveHelloButton,      SIGNAL(clicked()), this, SLOT(ConnectToPort()));
    QObject::connect(ui->saharaSayHelloButton,          SIGNAL(clicked()), this, SLOT(SayHello()));
    QObject::connect(ui->portDisconnectButton,          SIGNAL(clicked()), this, SLOT(DisconnectPort()));
    QObject::connect(ui->saharaSwitchModeButton,        SIGNAL(clicked()), this, SLOT(SwitchMode()));
    QObject::connect(ui->saharaSendClientCommandButton, SIGNAL(clicked()), this, SLOT(SendClientCommand()));
    QObject::connect(ui->saharaReadPortButton,          SIGNAL(clicked()), this, SLOT(ReadSome()));
    QObject::connect(ui->saharaSendImageBrowse,         SIGNAL(clicked()), this, SLOT(BrowseForImage()));
    QObject::connect(ui->saharaSendImageButton,         SIGNAL(clicked()), this, SLOT(SendImage()));
    QObject::connect(ui->saharaSendCommandButton,       SIGNAL(clicked()), this, SLOT(SendCommandButtonAction()));
    QObject::connect(ui->streamingDloadHelloButton,     SIGNAL(clicked()), this, SLOT(SendStreamingDloadHello()));


}

SaharaWindow::~SaharaWindow()
{
    this->close();
    delete ui;
}

void SaharaWindow::UpdatePortList()
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

            if (device.description.length()) {
                logMsg.append(" - ").append(device.description.c_str());
            }

            log(logMsg);

        }
    }
}

void SaharaWindow::ConnectToPort()
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
        if (selected.compare(device.port.c_str(), Qt::CaseInsensitive) == 0 && strstr(device.hardware_id.c_str(), "05c6:9008")) {
            currentPort = device;
            break;
        }
    }

    if (!currentPort.port.length()) {
        log("Invalid Port Type");
        return;
    }

    //ui->portDisconnectButton->setEnabled(true);

    QString connectionText = "Connecting to ";

    connectionText.append(currentPort.port.c_str()).append(" ...");

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

    if (!port.receiveHello()) {
        log("Did not receive hello. Not in sahara mode or requires restart.");
        return DisconnectPort();
    }

    ui->deviceStateModeValue->setText(port.getNamedMode(port.deviceState.mode));

    QString tmp;
    log(tmp.sprintf("Device In Mode: %s", port.getNamedMode(port.deviceState.mode)));
    ui->deviceStateVersionValue->setText(tmp.sprintf("%i", port.deviceState.version));
    ui->deviceStateMinVersionValue->setText(tmp.sprintf("%i", port.deviceState.minVersion));
    ui->deviceStatePreferredMaxSizeValue->setText(tmp.sprintf("%i", port.deviceState.maxCommandPacketSize));

    int index = ui->saharaSayHelloSwitchCommandSelect->findData(port.deviceState.mode);

    if ( index != -1 ) {
       ui->saharaSayHelloSwitchCommandSelect->setCurrentIndex(index);
    }
}

void SaharaWindow::SayHello()
{
    if (!port.isOpen()) {
        log("Select a port and receive hello first");
        return;
    }

    QString tmp;

    if (!port.sendHello(ui->saharaSayHelloSwitchCommandSelect->currentData().toUInt(), 0x00)) {
        log("Error Saying Hello");
        return DisconnectPort();
    }

    ui->deviceStateModeValue->setText(port.getNamedMode(port.deviceState.mode));

    if (port.deviceState.mode == SAHARA_MODE_IMAGE_TX_PENDING) {

        log(tmp.sprintf("Device Requesting %zd Bytes of Image 0x%02x - %s", port.readState.length, port.readState.imageId, port.getNamedRequestedImage(port.readState.imageId)));
        log(tmp.sprintf("Device Requesting %zd Bytes of Image (Possible Alternate) 0x%02x - %s", port.readState.length, port.readState.imageId, port.getNamedRequestedImageAlt(port.readState.imageId)));

    }
}

void SaharaWindow::BrowseForImage()
{

    QString fileName = QFileDialog::getOpenFileName(this, "Select Image To Send", "", "Image Files (*.mbn *.bin *.img)");

    if (fileName.length()) {
        ui->saharaSendImagePath->setText(fileName);
    }
}

void SaharaWindow::SendImage()
{
    if (!port.isOpen()) {
        log("Select a port and receive hello first.");
        return;
    }

    QString fileName = ui->saharaSendImagePath->text();

    if (!fileName.length()) {
        log("Please Enter or Browse For a Image File");
        return;
    }

    if (port.readState.imageId == SAHARA_IMAGE_NONE) {
        log("Device has not requested an image");
        return;
    }

    if (!port.sendImage(fileName.toStdString())) {
        log("Error Sending Image");
        return DisconnectPort();
    }


    log("Image Transfer Complete");
}

/**
 * @brief SaharaWindow::SwitchMode
 */
void SaharaWindow::SwitchMode()
{
    if (!port.isOpen()) {
        log("Select a port and receive hello first.");
        return;
    }

    QString tmp;
    uint16_t requestMode =  ui->saharaSwitchModeSelect->currentData().toUInt();

    log(tmp.sprintf("Requesting Mode Switch From 0x%02x - %s  to 0x%02x - %s",
        port.deviceState.mode,
        port.getNamedMode(port.deviceState.mode),
        requestMode,
        port.getNamedMode(requestMode)
    ));

    if (!port.switchMode(requestMode)) {
        log("Error Switching Modes");
        return DisconnectPort();
    }

    log(tmp.sprintf("Device In Mode: %s", port.getNamedMode(port.deviceState.mode)));
    ui->deviceStateModeValue->setText(port.getNamedMode(port.deviceState.mode));
}

/**
 * @brief SaharaWindow::SendClientCommand
 *
 */
void SaharaWindow::SendClientCommand()
{
    if (!port.isOpen()) {
        log("Select a port and receive hello first and switch to client command mode.");
        return;
    }

    uint16_t requestedCommand =  ui->saharaSendClientCommandSelect->currentData().toUInt();

    uint8_t* readData;
    size_t readDataSize;
    QString tmp;

    if (!port.sendClientCommand(requestedCommand, readData, readDataSize)) {
        log("Error Sending Client Comand");
        return DisconnectPort();
    }

    log(tmp.sprintf("========\nDumping Data For Command: 0x%02x - %s - %zd Bytes\n========\n\n",
        requestedCommand, port.getNamedClientCommand(requestedCommand), readDataSize
    ));

    logHex(readData, readDataSize);
}

/**
 * @brief SaharaWindow::SendReset
 */
void SaharaWindow::SendReset()
{
    if (!port.isOpen()) {
        log("Select a port and receive hello first.");
        return;
    }

    log("Sending Reset Command");

    if (!port.sendReset()) {
        log("Error Sending Reset");
        return DisconnectPort();
    }

}



void SaharaWindow::SendDone()
{
    if (!port.isOpen()) {
        log("Select a port and receive hello first.");
        return;
    }

    log("Sending Done Command");

    if (!port.sendDone()) {
        log("Error Sending Done");
        return DisconnectPort();
    }

}

/**
 * @brief SaharaWindow::SendCommandButtonAction
 */
void SaharaWindow::SendCommandButtonAction()
{
    if (!port.isOpen()) {
        log("Select a port and receive hello first.");
        return;
    }

    uint16_t requestedCommand =  ui->saharaSendCommandSelect->currentData().toUInt();

    switch(requestedCommand){
        case SAHARA_DONE:
            return SendDone();
            break;
        case SAHARA_RESET:
            return SendReset();
            break;
        default:
            log("Unknown Command");
            break;
    }
}

/**
 * @brief SaharaWindow::ReadSome
 */
void SaharaWindow::ReadSome()
{
    if (!port.isOpen()) {
        log("Select a port and receive hello first.");
        return;
    }

    size_t bytesRead = port.read(port.buffer, port.bufferSize);

    QString tmp;

    log(tmp.sprintf("Read %zd Bytes", bytesRead));

    if (bytesRead) {
        logHex(port.buffer, bytesRead);
    }
}

/**
 * @brief SaharaWindow::DisconnectPort
 */
void SaharaWindow::DisconnectPort()
{
    log("Closing Port..");

    if (port.isOpen()) {
        port.close();
    }

    //ui->portDisconnectButton->setEnabled(false);
}


/**
 * @brief SaharaWindow::SendStreamingDloadHello
 */
void SaharaWindow::SendStreamingDloadHello()
{

    if (!port.isOpen()) {
        log("Port Not Open");
        return;
    }

    if (!ui->streamingDloadHelloMagiInput->text().length()) {
        log("No Magic Set");
        return;
    }

    streaming_dload_hello_tx_t dloadHello;
    uint32_t outsize = 0;
    uint8_t* outbuf = NULL;
    std::string magic = ui->streamingDloadHelloMagiInput->text().toStdString();

    dloadHello.command = STREAMING_DLOAD_HELLO;
    memcpy(dloadHello.magic, magic.c_str(), magic.size());
    dloadHello.version = 0x04;
    dloadHello.compatibleVersion = 0x04;
    dloadHello.featureBits = 0x08;

    dload_request((uint8_t*)&dloadHello, sizeof(dloadHello), &outbuf, &outsize);

    size_t bytesWritten = port.write(outbuf, outsize);
    printf("Wrote %zd bytes\n", bytesWritten);
    hexdump(outbuf, bytesWritten);

    size_t bytesRead = port.read(port.buffer, port.bufferSize);
    printf("Read %zd bytes\n", bytesRead);
    hexdump(port.buffer, bytesRead);

    //ui->portDisconnectButton->setEnabled(false);
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
    log(tmp.sprintf("Dumping %zd bytes written", amount));
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
    log(tmp.sprintf("Dumping %zd bytes read", amount));
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
    unsigned int    dp, p;  /* data pointer */
    const char      trans[] =
        "................................ !\"#$%&'()*+,-./0123456789"
        ":;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklm"
        "nopqrstuvwxyz{|}~...................................."
        "....................................................."
        "........................................";

    hexdump(data, amount);

    QString lineBuff;
    QString tmp;

    for (dp = 1; dp <= amount; dp++) {
        tmp.sprintf("%02x ", data[dp - 1]);
        lineBuff.append(tmp);

        if ((dp % 8) == 0) {
            lineBuff.append(" ");
        }

        if ((dp % 16) == 0) {
            lineBuff.append("| ");
            p = dp;
            for (dp -= 16; dp < p; dp++) {
                tmp.sprintf("%c ", trans[data[dp]]);
                lineBuff.append(tmp);
            }
            lineBuff.append("\n");
        }
    }

    // tail
    if ((amount % 16) != 0) {
        p = dp = 16 - (amount % 16);
        for (dp = p; dp > 0; dp--) {
            lineBuff.append("   ");
            if (((dp % 8) == 0) && (p != 8))
                lineBuff.append(" ");
        }
        lineBuff.append(" | ");
        for (dp = (amount - (16 - p)); dp < amount; dp++)
            tmp.sprintf("%c ", trans[data[dp]]);
            lineBuff.append(tmp);
    }

    lineBuff.append("\n");

    log(lineBuff);

    return;
}
