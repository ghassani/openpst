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

using namespace openpst;

QcdmWindow::QcdmWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QcdmWindow),
    port("", 115200)
{
    ui->setupUi(this);

    DisableUiButtons();
    UpdatePortList();

    ui->decSpcValue->setInputMask("999999");
    ui->hexMeidValue->setInputMask("HHHHHHHHHHHHHH");
    ui->imeiValue->setInputMask("999999999999999");

    QObject::connect(ui->portListRefreshButton, SIGNAL(clicked()), this, SLOT(UpdatePortList()));
    QObject::connect(ui->portDisconnectButton, SIGNAL(clicked()), this, SLOT(DisconnectPort()));
    QObject::connect(ui->portConnectButton, SIGNAL(clicked()), this, SLOT(ConnectToPort()));
    QObject::connect(ui->sendSpcButton, SIGNAL(clicked()), this, SLOT(sendSpc()));
    QObject::connect(ui->sendPasswordButton, SIGNAL(clicked()), this, SLOT(sendPassword()));
    QObject::connect(ui->readMeidButton, SIGNAL(clicked()), this, SLOT(readMeid()));
    QObject::connect(ui->writeMeidButton, SIGNAL(clicked()), this, SLOT(writeMeid()));
    QObject::connect(ui->readImeiButton, SIGNAL(clicked()), this, SLOT(readImei()));
    QObject::connect(ui->readNamButton, SIGNAL(clicked()), this, SLOT(readNam()));
    QObject::connect(ui->readSpcButton, SIGNAL(clicked()), this, SLOT(readSpc()));
    QObject::connect(ui->writeSpcButton, SIGNAL(clicked()), this, SLOT(writeSpc()));
    QObject::connect(ui->readSubscriptionButton, SIGNAL(clicked()), this, SLOT(readSubscription()));
    QObject::connect(ui->writeSubscriptionButton, SIGNAL(clicked()), this, SLOT(writeSubscription()));

    QObject::connect(ui->sendPhoneModeButton, SIGNAL(clicked()), this, SLOT(sendPhoneMode()));

    QObject::connect(ui->decSpcValue, SIGNAL(textChanged(QString)), this, SLOT(spcTextChanged(QString)));
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
    std::vector<serial::PortInfo> devices = serial::list_ports();
    std::vector<serial::PortInfo>::iterator iter = devices.begin();

    ui->portListComboBox->clear();
    ui->portListComboBox->addItem("- Select a Port -", 0);

    log(LOGTYPE_INFO, "Scanning for Available Devices");

    while (iter != devices.end()) {
        serial::PortInfo device = *iter++;

        QString item = device.port.c_str();
        item.append(" - ").append(device.description.c_str());

        ui->portListComboBox->addItem(item, device.port.c_str());

        QString logMsg = "Found ";
        logMsg.append(device.hardware_id.c_str()).append(" on ").append(device.port.c_str());

        if (device.description.length()) {
            logMsg.append(" - ").append(device.description.c_str());
        }

        log(LOGTYPE_DEBUG, logMsg);
    }
}

/**
* @brief QcdmWindow::ConnectToPort
*/
void QcdmWindow::ConnectToPort()
{
    QString selected = ui->portListComboBox->currentData().toString();

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

            ui->portConnectButton->setEnabled(false);
            ui->portDisconnectButton->setEnabled(true);
            ui->portListRefreshButton->setEnabled(false);
            ui->portListComboBox->setEnabled(false);
            EnableUiButtons();

            QString connectedText = "Connected to ";
            connectedText.append(currentPort.port.c_str());
            log(LOGTYPE_INFO, connectedText);
        }
    }
    catch (serial::IOException e) {
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

        ui->portConnectButton->setEnabled(true);
        ui->portDisconnectButton->setEnabled(false);
        ui->portListRefreshButton->setEnabled(true);
        ui->portListComboBox->setEnabled(true);
        DisableUiButtons();
    }
}

/**
* @brief QcdmWindow::SecuritySendSpc
*/
void QcdmWindow::sendSpc()
{
    if (ui->sendSpcValue->text().length() != 6) {
        log(LOGTYPE_WARNING, "Enter a Valid 6 Digit SPC");
        return;
    }

    int result = port.sendSpc(ui->sendSpcValue->text().toStdString().c_str());

    if (result == DIAG_CMD_TX_FAIL || result == DIAG_CMD_RX_FAIL) {
        log(LOGTYPE_ERROR, "Error Sending SPC");
        return;
    }

    if (result == DIAG_SPC_REJECT) {
        log(LOGTYPE_ERROR, "SPC Not Accepted: " + ui->sendSpcValue->text());
        return;
    }

    log(LOGTYPE_INFO, "SPC Accepted: " + ui->sendSpcValue->text());
}

/**
* @brief QcdmWindow::SecuritySend16Password
*/
void QcdmWindow::sendPassword()
{
    if (ui->sendPasswordValue->text().length() != 16) {
        log(LOGTYPE_WARNING, "Enter a Valid 16 Digit Password");
        return;
    }

    int result = port.sendPassword(ui->sendPasswordValue->text().toStdString().c_str());

    if (result == DIAG_CMD_TX_FAIL || result == DIAG_CMD_RX_FAIL) {
        log(LOGTYPE_ERROR, "Error Sending Password");
        return;
    }

    if (result == DIAG_PASSWORD_REJECT) {
        log(LOGTYPE_ERROR, "Password Not Accepted: " + ui->sendPasswordValue->text());
        return;
    }

    log(LOGTYPE_INFO, "Password Accepted: " + ui->sendPasswordValue->text());
}

/**
* @brief QcdmWindow::sendQcdmPhoneMode
*/
void QcdmWindow::sendPhoneMode()
{
    int result = port.sendPhoneMode((uint8_t)ui->phoneModeValue->currentIndex());

    if (result == MODE_RESET_F) {
        DisconnectPort();
        UpdatePortList();
    }

    if (result == (uint8_t)ui->phoneModeValue->currentIndex()){
        log(LOGTYPE_INFO, "Send Phone Mode Success: " + ui->phoneModeValue->currentText());
    }
    else {
        log(LOGTYPE_INFO, "Send Phone Mode Failure: " + ui->phoneModeValue->currentText());
    }
}

/**
* @brief QcdmWindow::nvReadGetMeid
*/
void QcdmWindow::readMeid()
{
    if (ui->hexMeidValue->text().length() != 0) {
        ui->hexMeidValue->setText("");
    }

    uint8_t* response = NULL;

    int result = port.getNvItem(NV_MEID_I, &response);

    if (result == DIAG_NV_READ_F){
        QString meidValue, tmp;

        qcdm_nv_rx_t* rxPacket = (qcdm_nv_rx_t*)response;

        for (int p = 6; p >= 0; p--) {
            tmp.sprintf("%02x", rxPacket->data[p]);
            meidValue.append(tmp);
        }

        meidValue = meidValue.toUpper();

        ui->hexMeidValue->setText(meidValue);

        log(LOGTYPE_INFO, "Read Success - MEID: " + meidValue);
    }
    else {
        log(LOGTYPE_ERROR, "Read Failure - MEID");
    }
}

/**
* @brief QcdmWindow::nvWriteSetMeid
*/
void QcdmWindow::writeMeid()
{
    if (ui->hexMeidValue->text().length() != 14) {
        log(LOGTYPE_WARNING, "Enter a Valid 14 Character MEID");
    }

    uint8_t* response = NULL;

    int result = port.setNvItem(NV_MEID_I, ui->hexMeidValue->text().toStdString().c_str(), 6, &response);

    if (result == DIAG_NV_WRITE_F) {
        log(LOGTYPE_INFO, "Write Success - MEID: " + ui->hexMeidValue->text());
    }
    else {
        log(LOGTYPE_ERROR, "Write Failure - MEID");
    }
}

/**
* @brief QcdmWindow::nvReadGetImei
*/
void QcdmWindow::readImei()

{
    if (ui->imeiValue->text().length() != 0) {
        ui->imeiValue->setText("");
    }

    uint8_t* response = NULL;

    int result = port.getNvItem(NV_UE_IMEI_I, &response);

    if (result == DIAG_NV_READ_F) {
        QString imeiValue, tmp;

        qcdm_nv_rx_t* rxPacket = (qcdm_nv_rx_t*)response;

        for (int p = 1; p <= 8; p++) {
            tmp.sprintf("%02x", rxPacket->data[p]);
            imeiValue.append(tmp);
        }

        imeiValue = imeiValue.remove("a");

        if (imeiValue != "0000000000000000") {
            ui->imeiValue->setText(imeiValue);

            log(LOGTYPE_INFO, "Read Success - IMEI: " + imeiValue);
        }
        else {
            log(LOGTYPE_ERROR, "Read Failure - IMEI");
        }
    }
    else {
        log(LOGTYPE_ERROR, "Read Failure - IMEI");
    }
}

/**
* @brief QcdmWindow::readNam
*/
void QcdmWindow::readNam() {
    // Move to readMdn() function...
    if (ui->mdnValue->text().length() != 0) {
        ui->mdnValue->setText("");
    }

    uint8_t* response = NULL;

    int result = port.getNvItem(NV_DIR_NUMBER_I, &response);

    if (result == DIAG_NV_READ_F){
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)response;

        QString mdnValue = QString::fromStdString(port.hexToString((char *)rxPacket->data, 9));

        ui->mdnValue->setText(mdnValue);

        log(LOGTYPE_INFO, "Read Success - MDN: " + mdnValue);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - MDN");
    }

    if (ui->minValue->text().length() != 0) {
        ui->minValue->setText("");
    }

    char minChunk1[3];
    char minChunk2[1];

    int32_t iMin1, iMin2, min1a, min1b, min1c, min2;

    QString decodedMin, tmp;

    std::string sMin1;
    std::string sMin2;

    response = NULL;

    result = port.getNvItem(NV_MIN1_I, &response);

    if (result == DIAG_NV_READ_F){
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)response;

        minChunk1[0] = rxPacket->data[3];
        minChunk1[1] = rxPacket->data[6];
        minChunk1[2] = rxPacket->data[5];
        minChunk1[3] = rxPacket->data[4];

        sMin1 = port.bytesToHex(minChunk1, 4);

        iMin1 = strtoul(sMin1.c_str(), nullptr, 16);

        min1a = (iMin1 & 0xFFC000) >> 14;
        min1a = ((min1a + 1) % 10) + (((((min1a % 100) / 10) + 1) % 10) * 10) + ((((min1a / 100) + 1) % 10) * 100);

        min1b = ((iMin1 & 0x3C00) >> 10) % 10;

        min1c = (iMin1 & 0x3FF);
        min1c = ((min1c + 1) % 10) + (((((min1c % 100) / 10) + 1) % 10) * 10) + ((((min1c / 100) + 1) % 10) * 100);
    }

    response = NULL;

    result = port.getNvItem(NV_MIN2_I, &response);

    if (result == DIAG_NV_READ_F){
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)response;

        minChunk2[0] = rxPacket->data[3];
        minChunk2[1] = rxPacket->data[2];

        sMin2 = port.bytesToHex(minChunk2, 2);

        iMin2 = strtoul(sMin2.c_str(), nullptr, 16);

        min2 = ((iMin2+1) % 10) + (((((iMin2 % 100) / 10) + 1) % 10) * 10) + ((((iMin2 / 100) + 1) % 10) * 100);
    }

    tmp.sprintf("%03i%03i%i%i", min2, min1a, min1b, min1c);
    decodedMin.append(tmp);

    if (decodedMin.length() == 10) {
        ui->minValue->setText(decodedMin);

        log(LOGTYPE_INFO, "Read Success - MIN: " + decodedMin);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - MIN");
    }
}

/**
* @brief QcdmWindow::nvReadGetSpc
*/
void QcdmWindow::readSpc()
{
    if (ui->hexSpcValue->text().length() != 0 || ui->decSpcValue->text().length() != 0) {
        ui->hexSpcValue->setText("");
        ui->decSpcValue->setText("");
    }

    uint8_t* response = NULL;

    int result = 0;

    switch (ui->readSpcMethod->currentData().toInt()) {
    case 0:
        result = port.getNvItem(NV_SEC_CODE_I, &response);
        break;
    case 1:
        // EFS Method

        break;
    case 2:
        port.sendHtcNvUnlock(&response); // HTC Method
        result = port.getNvItem(NV_SEC_CODE_I, &response);
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
        qcdm_nv_rx_t* rxPacket = (qcdm_nv_rx_t*)response;

        QString rxSpcValue = QString::fromStdString(port.hexToString((char *)rxPacket->data, 5));

        ui->decSpcValue->setText(rxSpcValue);

        log(LOGTYPE_INFO, "Read Success - SPC: " + rxSpcValue);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - SPC");
    }
}

/**
* @brief QcdmWindow::nvWriteSetSpc
*/
void QcdmWindow::writeSpc()
{
    if (ui->decSpcValue->text().length() != 6) {
        log(LOGTYPE_WARNING, "Enter a Valid 6 Digit SPC");
        return;
    }

    uint8_t* response = NULL;

    int result = port.setNvItem(NV_SEC_CODE_I, ui->decSpcValue->text().toStdString().c_str(), 6, &response);

    if (result == DIAG_NV_WRITE_F) {
        log(LOGTYPE_INFO, "Write Success - SPC: " + ui->decSpcValue->text());
    }
    else {
        log(LOGTYPE_ERROR, "Write Failure - SPC");
    }
}

void QcdmWindow::readSubscription()
{
    uint8_t* response = NULL;

    int result = port.getNvItem(NV_RTRE_CONFIG_I, &response);

    if (result == DIAG_NV_READ_F) {
        QString result;

        qcdm_nv_rx_t* rxPacket = (qcdm_nv_rx_t*)response;

        ui->subscriptionValue->setCurrentIndex(rxPacket->data[0]);

        switch (rxPacket->data[0]) {
        case RTRE_MODE_RUIM_ONLY:
            result = "RUIM_ONLY";
            break;
        case RTRE_MODE_NV_ONLY:
            result = "NV_ONLY";
            break;
        case RTRE_MODE_RUIM_PREF:
            result = "RUIM_PREF";
            break;
        case RTRE_MODE_GSM_1X:
            result = "GSM_1X";
            break;
        }

        log(LOGTYPE_INFO, "Read Success - Subscription Mode: " + result);
    }
    else {
        log(LOGTYPE_ERROR, "Read Failure - Subscription Mode");
    }
}

void QcdmWindow::writeSubscription()
{
    uint8_t* response = NULL;

    int index = ui->subscriptionValue->currentIndex();

    int result = port.setNvItem(NV_RTRE_CONFIG_I, static_cast<const char *>(static_cast<void*>(&index)), 1, &response);

    if (result == DIAG_NV_WRITE_F) {
        QString result;

        qcdm_nv_rx_t* rxPacket = (qcdm_nv_rx_t*)response;

        ui->subscriptionValue->setCurrentIndex(rxPacket->data[0]);

        switch (rxPacket->data[0]) {
        case RTRE_MODE_RUIM_ONLY:
            result = "RUIM_ONLY";
            break;
        case RTRE_MODE_NV_ONLY:
            result = "NV_ONLY";
            break;
        case RTRE_MODE_RUIM_PREF:
            result = "RUIM_PREF";
            break;
        case RTRE_MODE_GSM_1X:
            result = "GSM_1X";
            break;
        }

        log(LOGTYPE_INFO, "Write Success - Subscription Mode: " + result);
    }
    else {
        log(LOGTYPE_ERROR, "Write Failure - Subscription Mode");
    }
}

void QcdmWindow::spcTextChanged(QString value)
{
    if (value.length() == 6) {
        QString result, tmp;

        for (int i = 0; i < value.length(); i++) {
            tmp.sprintf("%02x", value.toStdString().c_str()[i]);
            result.append(tmp);
        }

        ui->hexSpcValue->setText(result);
    }
}

void QcdmWindow::DisableUiButtons() {
    ui->readSpcButton->setEnabled(false);
    ui->readMeidButton->setEnabled(false);
    ui->readImeiButton->setEnabled(false);
    ui->writeSpcButton->setEnabled(false);
    ui->writeMeidButton->setEnabled(false);
    ui->writeImeiButton->setEnabled(false);
    ui->readSubscriptionButton->setEnabled(false);
    ui->writeSubscriptionButton->setEnabled(false);
    ui->sendPhoneModeButton->setEnabled(false);
    ui->sendSpcButton->setEnabled(false);
    ui->sendPasswordButton->setEnabled(false);
}

void QcdmWindow::EnableUiButtons() {
    ui->readSpcButton->setEnabled(true);
    ui->readMeidButton->setEnabled(true);
    ui->readImeiButton->setEnabled(true);
    ui->writeSpcButton->setEnabled(true);
    ui->writeMeidButton->setEnabled(true);
    ui->writeImeiButton->setEnabled(true);
    ui->readSubscriptionButton->setEnabled(true);
    ui->writeSubscriptionButton->setEnabled(true);
    ui->sendPhoneModeButton->setEnabled(true);
    ui->sendSpcButton->setEnabled(true);
    ui->sendPasswordButton->setEnabled(true);
}

/**
* @brief QcdmWindow::log
* @param message
*/
void QcdmWindow::log(int type, const char* message)
{
    QString newMessage = message;
    log(type, newMessage);
}
/**
* @brief QcdmWindow::log
* @param message
*/
void QcdmWindow::log(int type, std::string message)
{
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
    QString suffix = "</font>";

    switch (type) {
    case 0:
        message = message.prepend("<font color=\"gray\">");
        message = message.append(suffix);
        break;
    case -1:
        message = message.prepend("<font color=\"red\">");
        message = message.append(suffix);
        break;
    case 1:
        message = message.prepend("<font color=\"green\">");
        message = message.append(suffix);
        break;
    case 2:
        message = message.prepend("<font color=\"orange\">");
        message = message.append(suffix);
        break;
    }

    ui->log->appendHtml(message);
}

