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
    ui->mdnValue->setInputMask("9999999999");
    ui->minValue->setInputMask("9999999999");

    QObject::connect(ui->portListRefreshButton, SIGNAL(clicked()), this, SLOT(UpdatePortList()));
    QObject::connect(ui->portDisconnectButton, SIGNAL(clicked()), this, SLOT(DisconnectPort()));
    QObject::connect(ui->portConnectButton, SIGNAL(clicked()), this, SLOT(ConnectToPort()));
    QObject::connect(ui->sendSpcButton, SIGNAL(clicked()), this, SLOT(SendSpc()));
    QObject::connect(ui->sendPasswordButton, SIGNAL(clicked()), this, SLOT(SendPassword()));
    QObject::connect(ui->readMeidButton, SIGNAL(clicked()), this, SLOT(ReadMeid()));
    QObject::connect(ui->writeMeidButton, SIGNAL(clicked()), this, SLOT(WriteMeid()));
    QObject::connect(ui->readImeiButton, SIGNAL(clicked()), this, SLOT(ReadImei()));
    QObject::connect(ui->readNamButton, SIGNAL(clicked()), this, SLOT(ReadNam()));
    QObject::connect(ui->writeNamButton, SIGNAL(clicked()), this, SLOT(WriteNam()));
    QObject::connect(ui->readNvItemButton, SIGNAL(clicked()), this, SLOT(ReadNvItem()));
    QObject::connect(ui->readSpcButton, SIGNAL(clicked()), this, SLOT(ReadSpc()));
    QObject::connect(ui->writeSpcButton, SIGNAL(clicked()), this, SLOT(WriteSpc()));
    QObject::connect(ui->readSubscriptionButton, SIGNAL(clicked()), this, SLOT(ReadSubscription()));
    QObject::connect(ui->writeSubscriptionButton, SIGNAL(clicked()), this, SLOT(WriteSubscription()));

    QObject::connect(ui->clearLogButton, SIGNAL(clicked()), this, SLOT(ClearLog()));
    QObject::connect(ui->saveLogButton, SIGNAL(clicked()), this, SLOT(SaveLog()));

    QObject::connect(ui->sendPhoneModeButton, SIGNAL(clicked()), this, SLOT(SendPhoneMode()));

    QObject::connect(ui->decSpcValue, SIGNAL(textChanged(QString)), this, SLOT(SpcTextChanged(QString)));
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
* @brief QcdmWindow::SendSpc
*/
void QcdmWindow::SendSpc()
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
* @brief QcdmWindow::SendPassword
*/
void QcdmWindow::SendPassword()
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
* @brief QcdmWindow::SendPhoneMode
*/
void QcdmWindow::SendPhoneMode()
{
    int rx = port.sendPhoneMode((uint8_t)ui->phoneModeValue->currentIndex());

    if (rx == MODE_RESET_F) {
        DisconnectPort();
        UpdatePortList();
    }

    if (rx == (uint8_t)ui->phoneModeValue->currentIndex()){
        log(LOGTYPE_INFO, "Send Phone Mode Success: " + ui->phoneModeValue->currentText());
    }
    else {
        log(LOGTYPE_INFO, "Send Phone Mode Failure: " + ui->phoneModeValue->currentText());
    }
}

/**
* @brief QcdmWindow::ReadMeid
*/
void QcdmWindow::ReadMeid()
{
    if (ui->hexMeidValue->text().length() != 0) {
        ui->hexMeidValue->setText("");
    }

    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_MEID_I, &resp);

    if (rx == DIAG_NV_READ_F){
        qcdm_nv_rx_t* rxPacket = (qcdm_nv_rx_t*)resp;

        std::string tmp = bytesToHex((unsigned char *)rxPacket->data, 6, true);
        QString result = QString::fromStdString(tmp).toUpper();

        ui->hexMeidValue->setText(result);

        log(LOGTYPE_INFO, "Read Success - MEID: " + result);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - MEID");
    }
}

/**
* @brief QcdmWindow::WriteMeid
*/
void QcdmWindow::WriteMeid()
{
    if (ui->hexMeidValue->text().length() != 14) {
        log(LOGTYPE_WARNING, "Enter a Valid 14 Character MEID");
    }

    long data = HexToBytes(ui->hexMeidValue->text().toStdString());

    qcdm_nv_raw_tx_t packet;
    memcpy(&packet.data, &data, sizeof(data));

    int rx = port.setNvItem(NV_MEID_I, (const char *)&packet, sizeof(packet));

    if (rx == DIAG_NV_WRITE_F) {
        log(LOGTYPE_INFO, "Write Success - MEID: " + ui->hexMeidValue->text());
    } else {
        log(LOGTYPE_ERROR, "Write Failure - MEID");
    }
}

/**
* @brief QcdmWindow::ReadImei
*/
void QcdmWindow::ReadImei() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_UE_IMEI_I, &resp);

    if (rx == DIAG_NV_READ_F) {
        qcdm_nv_rx_t* rxPacket = (qcdm_nv_rx_t*)resp;

        std::string tmp = bytesToHex((unsigned char *)rxPacket->data, 8, true);
        QString result = QString::fromStdString(tmp).remove("a");

        ui->imeiValue->setText(result);

        log(LOGTYPE_INFO, "Read Success - IMEI: " + result);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - IMEI");
    }
}

/**
* @brief QcdmWindow::ReadNvItem
*/
void QcdmWindow::ReadNvItem() {
    if (ui->nvItemValue->text().length() == 0) {
        log(LOGTYPE_WARNING, "Input a Valid NV Item Number");
        return;
    }

    uint8_t* resp = nullptr;

    int rx = port.getNvItem(ui->nvItemValue->text().toInt(), &resp);

    if (rx == DIAG_NV_READ_F){
        QString result;

        hexdump(resp, sizeof(resp) * 16, result, true);

        log(LOGTYPE_INFO, "Read Success - Item Number: " + ui->nvItemValue->text() + "<br>" + result);
    }
}

/**
* @brief QcdmWindow::ReadNam
*/
void QcdmWindow::ReadNam() {
    // readMdn();
    // readMin();
    // readSid();
    // readSystemPref();
    // readPrefMode();
    // readPrefServ();
    // readRoamPref();

    ReadPapUserId();
    ReadPapPassword();
    ReadPppUserId();
    ReadPppPassword();
    ReadHdrAnUserId();
    ReadHdrAnPassword();
    ReadHdrAnLongUserId();
    ReadHdrAnLongPassword();
    ReadHdrAnPppUserId();
}

/**
* @brief QcdmWindow::WriteNam
*/
void QcdmWindow::WriteNam() {
    // writeMdn();
    // writeSystemPref();
    // writePrefServ();
    // writeRoamPref();
}

/**
* @brief QcdmWindow::ReadMdn
*/
void QcdmWindow::ReadMdn() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_DIR_NUMBER_I, &resp);

    if (rx == DIAG_NV_READ_F){
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)resp;

        std::string tmp = hexToString((char *)rxPacket->data, 9);
        QString mdnValue = QString::fromStdString(tmp);

        ui->mdnValue->setText(mdnValue);

        log(LOGTYPE_INFO, "Read Success - MDN: " + mdnValue);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - MDN");
    }
}

/**
* @brief QcdmWindow::WriteMdn
*/
void QcdmWindow::WriteMdn()
{
    if (ui->mdnValue->text().length() != 10) {
        log(LOGTYPE_WARNING, "Enter a Valid 10 Digit MDN");
        return;
    }

    qcdm_nv_alt_tx_t packet;
    memcpy(&packet.data, ui->mdnValue->text().toStdString().c_str(), 10);

    int rx = port.setNvItem(NV_DIR_NUMBER_I, (const char *)&packet, 11);

    if (rx == DIAG_NV_WRITE_F) {
        log(LOGTYPE_INFO, "Write Success - MDN: " + ui->mdnValue->text());
    } else {
        log(LOGTYPE_ERROR, "Write Failure - MDN");
    }
}

/**
* @brief QcdmWindow::ReadMin
*/
void QcdmWindow::ReadMin() {
    unsigned char min1Chunk[3], min2Chunk[1];
    std::string min1, min2;

    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_MIN1_I, &resp);

    if (rx == DIAG_NV_READ_F){
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)resp;

        min1Chunk[0] = rxPacket->data[3];
        min1Chunk[1] = rxPacket->data[2];
        min1Chunk[2] = rxPacket->data[1];
        min1Chunk[3] = rxPacket->data[0];

        min1 = min1Decode(min1Chunk);
    }

    resp = nullptr;

    rx = port.getNvItem(NV_MIN2_I, &resp);

    if (rx == DIAG_NV_READ_F){
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)resp;

        min2Chunk[0] = rxPacket->data[3];
        min2Chunk[1] = rxPacket->data[2];

        min2 = min2Decode(min2Chunk);
    }

    QString decodedMin;
    decodedMin.append(QString::fromStdString(min2));
    decodedMin.append(QString::fromStdString(min1));

    if (decodedMin.length() == 10) {
        ui->minValue->setText(decodedMin);

        log(LOGTYPE_INFO, "Read Success - MIN: " + decodedMin);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - MIN");
    }
}

/**
* @brief QcdmWindow::ReadSid
*/
void QcdmWindow::ReadSid() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_HOME_SID_NID_I, &resp);

    if (rx == DIAG_NV_READ_F){
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)resp;

        unsigned char data[2];

        data[0] = rxPacket->data[1];
        data[1] = rxPacket->data[0];

        std::string tmp = sidDecode(data);
        QString result = QString::fromStdString(tmp);
        result = FixedEmptyTrim(result);

        ui->sidValue->setText(result);

        log(LOGTYPE_INFO, "Read Success - SID: " + result);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - SID");
    }
}

/**
* @brief QcdmWindow::ReadSystemPref
*/
void QcdmWindow::ReadSystemPref()
{
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_SYSTEM_PREF_I, &resp);

    if (rx == DIAG_NV_READ_F) {
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)resp;

        ui->systemPrefValue->setCurrentIndex(rxPacket->data[0] + 1);

        log(LOGTYPE_INFO, "Read Success - System Pref: " + ui->systemPrefValue->currentText());
    } else {
        log(LOGTYPE_ERROR, "Read Failure - System Pref");
    }
}

/**
* @brief QcdmWindow::WriteSystemPref
*/
void QcdmWindow::WriteSystemPref()
{
    int mode = ui->systemPrefValue->currentIndex() - 1;

    if (mode < 0) {
        log(LOGTYPE_WARNING, "Select a System Pref to Write");
        return;
    }

    uint8_t* resp = nullptr;

    qcdm_nv_alt_tx_t packet;
    memcpy(&packet.data, static_cast<const char *>(static_cast<void*>(&mode)), 1);

    int rx = port.setNvItem(NV_SYSTEM_PREF_I, (const char *)&packet, 2, &resp);

    if (rx == DIAG_NV_WRITE_F) {
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)resp;

        ui->systemPrefValue->setCurrentIndex(rxPacket->data[0] + 1);

        log(LOGTYPE_INFO, "Write Success - System Pref: " + ui->systemPrefValue->currentText());
    } else {
        log(LOGTYPE_ERROR, "Write Failure - System Pref");
    }
}

/**
* @brief QcdmWindow::ReadPrefMode
*/
void QcdmWindow::ReadPrefMode()
{
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_PREF_MODE_I, &resp);

    if (rx == DIAG_NV_READ_F) {
        QString result = "NOT_IMPLEMENTED";

        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)resp;

        int newIndex = 0;

        switch (rxPacket->data[0]) {

        case PREF_MODE_AUTOMATIC:
            result = "AUTOMATIC";
            newIndex = 1;
            break;
        case PREF_MODE_CDMA_GSM_WCDMA:
            result = "CDMA_GSM_WCDMA";
            newIndex = 2;
            break;
        case PREF_MODE_CDMA_HDR:
            result = "CDMA_HDR";
            newIndex = 3;
            break;
        case PREF_MODE_CDMA_HDR_GSM_WCDMA:
            result = "CDMA_HDR_GSM_WCDMA";
            newIndex = 4;
            break;
        case PREF_MODE_CDMA_IS2000:
            result = "CDMA_IS2000";
            newIndex = 5;
            break;
        case PREF_MODE_CDMA_IS95:
            result = "CDMA_IS95";
            newIndex = 6;
            break;
        case PREF_MODE_GSM_GPRS_EDGE:
            result = "GSM_GPRS_EDGE";
            newIndex = 7;
            break;
        case PREF_MODE_GSM_WCDMA:
            result = "GSM_WCDMA";
            newIndex = 8;
            break;
        case PREF_MODE_HDR:
            result = "HDR";
            newIndex = 9;
            break;
        case PREF_MODE_LTE:
            result = "LTE";
            newIndex = 10;
            break;
        case PREF_MODE_LTE_CDMA:
            result = "LTE_CDMA";
            newIndex = 11;
            break;
        case PREF_MODE_LTE_CDMA_GSM:
            result = "LTE_CDMA_GSM";
            newIndex = 12;
            break;
        case PREF_MODE_LTE_CDMA_HDR:
            result = "LTE_CDMA_HDR";
            newIndex = 13;
            break;
        case PREF_MODE_LTE_CDMA_WCDMA:
            result = "LTE_CDMA_WCDMA";
            newIndex = 14;
            break;
        case PREF_MODE_LTE_GSM:
            result = "LTE_GSM";
            newIndex = 15;
            break;
        case PREF_MODE_LTE_GSM_WCDMA:
            result = "LTE_GSM_WCDMA";
            newIndex = 16;
            break;
        case PREF_MODE_LTE_HDR:
            result = "LTE_HDR";
            newIndex = 17;
            break;
        case PREF_MODE_LTE_HDR_GSM:
            result = "LTE_HDR_GSM";
            newIndex = 18;
            break;
        case PREF_MODE_LTE_HDR_WCDMA:
            result = "LTE_HDR_WCDMA";
            newIndex = 19;
            break;
        case PREF_MODE_LTE_WCDMA:
            result = "LTE_WCDMA";
            newIndex = 20;
            break;
        case PREF_MODE_WCDMA_HSDPA:
            result = "WCDMA_HSPDA";
            newIndex = 20;
            break;
        }

        ui->prefModeValue->setCurrentIndex(newIndex);

        log(LOGTYPE_INFO, "Read Success - Pref Mode: " + result);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - Pref Mode");
    }
}

/**
* @brief QcdmWindow::ReadPrefServ
*/
void QcdmWindow::ReadPrefServ()
{
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_CDMA_PREF_SERV_I, &resp);

    if (rx == DIAG_NV_READ_F) {
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)resp;

        ui->prefServValue->setCurrentIndex(rxPacket->data[0] + 1);

        log(LOGTYPE_INFO, "Read Success - Pref Serv: " + ui->prefServValue->currentText());
    } else {
        log(LOGTYPE_ERROR, "Read Failure - Pref Serv");
    }
}

/**
* @brief QcdmWindow::WritePrefServ
*/
void QcdmWindow::WritePrefServ()
{
    int mode = ui->prefServValue->currentIndex() - 1;

    if (mode < 0) {
        log(LOGTYPE_WARNING, "Select a System Pref to Write");
        return;
    }

    uint8_t* resp = nullptr;

    qcdm_nv_alt_tx_t packet;
    memcpy(&packet.data, static_cast<const char *>(static_cast<void*>(&mode)), 1);

    int rx = port.setNvItem(NV_CDMA_PREF_SERV_I, (const char *)&packet, 2, &resp);

    if (rx == DIAG_NV_WRITE_F) {
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)resp;

        ui->prefServValue->setCurrentIndex(rxPacket->data[0] + 1);

        log(LOGTYPE_INFO, "Write Success - System Pref: " + ui->prefServValue->currentText());
    } else {
        log(LOGTYPE_ERROR, "Write Failure - System Pref");
    }
}

/**
* @brief QcdmWindow::ReadRoamPref
*/
void QcdmWindow::ReadRoamPref()
{
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_ROAM_PREF_I, &resp);

    if (rx == DIAG_NV_READ_F) {
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)resp;

        int i = 0;

        switch (rxPacket->data[0]) {
        case ROAM_PREF_HOME:
            i = 1;
            break;
        case ROAM_PREF_AFFILIATED:
            i = 2;
            break;
        case ROAM_PREF_AUTOMATIC:
            i = 3;
            break;
        }

        ui->roamPrefValue->setCurrentIndex(i);

        log(LOGTYPE_INFO, "Read Success - Roam Pref: " + ui->roamPrefValue->currentText());
    } else {
        log(LOGTYPE_ERROR, "Read Failure - Roam Pref");
    }
}

/**
* @brief QcdmWindow::WriteRoamPref
*/
void QcdmWindow::WriteRoamPref()
{
    int mode = ui->roamPrefValue->currentIndex() - 1;

    if (mode < 0) {
        log(LOGTYPE_WARNING, "Select a Roam Pref to Write");
        return;
    }

    uint8_t* resp = nullptr;

    qcdm_nv_alt_tx_t packet;

    switch(mode) {
    case 0:
        packet.data[0] = ROAM_PREF_HOME;
        break;
    case 1:
        packet.data[0] = ROAM_PREF_AFFILIATED;
        break;
    case 2:
        packet.data[0] = ROAM_PREF_AUTOMATIC;
        break;
    }

    int rx = port.setNvItem(NV_ROAM_PREF_I, (const char *)&packet, 2, &resp);

    if (rx == DIAG_NV_WRITE_F) {
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)resp;

        int i = 0;

        switch (rxPacket->data[0]) {
        case ROAM_PREF_HOME:
            i = 1;
            break;
        case ROAM_PREF_AFFILIATED:
            i = 2;
            break;
        case ROAM_PREF_AUTOMATIC:
            i = 3;
            break;
        }

        ui->roamPrefValue->setCurrentIndex(i);

        log(LOGTYPE_INFO, "Write Success - Roam Pref: " + ui->roamPrefValue->currentText());
    } else {
        log(LOGTYPE_ERROR, "Write Failure - Roam Pref");
    }
}

/**
* @brief QcdmWindow::ReadSpc
*/
void QcdmWindow::ReadSpc()
{
    uint8_t* resp = nullptr;

    int rx = 0;

    switch (ui->readSpcMethod->currentIndex()) {
    case 0:
        rx = port.getNvItem(NV_SEC_CODE_I, &resp);
        break;
    case 1:
        // EFS Method

        break;
    case 2:
        port.sendHtcNvUnlock(&resp); // HTC Method
        rx = port.getNvItem(NV_SEC_CODE_I, &resp);
        break;
    case 3:
        port.sendLgNvUnlock(&resp); // LG Method
        rx = port.getLgSpc(&resp);
        break;
    case 4:
        // Samsung Method

        break;
    }

    if (rx == DIAG_NV_READ_F){
        qcdm_nv_rx_t* rxPacket = (qcdm_nv_rx_t*)resp;

        std::string result = hexToString((char *)rxPacket->data, 5);

        ui->decSpcValue->setText(QString::fromStdString(result));

        log(LOGTYPE_INFO, "Read Success - SPC: " + result);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - SPC");
    }
}

/**
* @brief QcdmWindow::WriteSpc
*/
void QcdmWindow::WriteSpc()
{
    if (ui->decSpcValue->text().length() != 6) {
        log(LOGTYPE_WARNING, "Enter a Valid 6 Digit SPC");
        return;
    }

    int rx = port.setNvItem(NV_SEC_CODE_I, ui->decSpcValue->text().toStdString().c_str(), 6);

    if (rx == DIAG_NV_WRITE_F) {
        log(LOGTYPE_INFO, "Write Success - SPC: " + ui->decSpcValue->text());
    } else {
        log(LOGTYPE_ERROR, "Write Failure - SPC");
    }
}

/**
* @brief QcdmWindow::ReadSubscription
*/
void QcdmWindow::ReadSubscription() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_RTRE_CONFIG_I, &resp);

    if (rx == DIAG_NV_READ_F) {
        qcdm_nv_rx_t* rxPacket = (qcdm_nv_rx_t*)resp;

        ui->subscriptionValue->setCurrentIndex(rxPacket->data[0] + 1);

        log(LOGTYPE_INFO, "Read Success - Subscription Mode: " + ui->subscriptionValue->currentText());
    } else {
        log(LOGTYPE_ERROR, "Read Failure - Subscription Mode");
    }
}

/**
* @brief QcdmWindow::WriteSubscription
*/
void QcdmWindow::WriteSubscription()
{
    int mode = ui->subscriptionValue->currentIndex() - 1;

    if (mode < 0) {
        log(LOGTYPE_WARNING, "Select a Subsciption Mode to Write");
        return;
    }

    uint8_t* resp = nullptr;

    const char* data = static_cast<const char *>(static_cast<void*>(&mode));

    int rx = port.setNvItem(NV_RTRE_CONFIG_I, data, 1, &resp);

    if (rx == DIAG_NV_WRITE_F) {
        qcdm_nv_rx_t* rxPacket = (qcdm_nv_rx_t*)resp;

        ui->subscriptionValue->setCurrentIndex(rxPacket->data[0] + 1);

        log(LOGTYPE_INFO, "Write Success - Subscription Mode: " + ui->subscriptionValue->currentText());
    } else {
        log(LOGTYPE_ERROR, "Write Failure - Subscription Mode");
    }
}

/**
* @brief QcdmWindow::ReadPapUserId
*/
void QcdmWindow::ReadPapUserId() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_PAP_USER_ID_I, &resp);

    if (rx == DIAG_NV_READ_F){
        qcdm_nv_alt2_rx_t* rxPacket = (qcdm_nv_alt2_rx_t*)resp;

        std::string tmp = hexToString((char *)rxPacket->data, DIAG_NV_ITEM_SIZE);
        QString result = QString::fromStdString(tmp);
        result = FixedEmptyTrim(result);

        ui->papUserIdValue->setText(result);

        log(LOGTYPE_INFO, "Read Success - PAP User ID: " + result);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - PAP User ID");
    }
}

/**
* @brief QcdmWindow::ReadPapPassword
*/
void QcdmWindow::ReadPapPassword() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_PAP_PASSWORD_I, &resp);

    if (rx == DIAG_NV_READ_F){
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)resp;

        std::string tmp = bytesToHex((unsigned char *)rxPacket->data, 16, false);
        tmp = tmp.erase(tmp.find_last_not_of("0") + 1);
        QString result = QString::fromStdString(tmp).toUpper();

        ui->papPasswordValue->setText(result);

        log(LOGTYPE_INFO, "Read Success - PAP Password: " + result);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - PAP Password");
    }
}

/**
* @brief QcdmWindow::ReadPppUserId
*/
void QcdmWindow::ReadPppUserId() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_PPP_USER_ID_I, &resp);

    if (rx == DIAG_NV_READ_F){
        qcdm_nv_alt2_rx_t* rxPacket = (qcdm_nv_alt2_rx_t*)resp;

        std::string tmp = hexToString((char *)rxPacket->data, DIAG_NV_ITEM_SIZE);
        QString result = QString::fromStdString(tmp);
        result = FixedEmptyTrim(result);

        ui->pppUserIdValue->setText(result);

        log(LOGTYPE_INFO, "Read Success - PPP User ID: " + result);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - PPP User ID");
    }
}

/**
* @brief QcdmWindow::ReadPppPassword
*/
void QcdmWindow::ReadPppPassword() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_PPP_PASSWORD_I, &resp);

    if (rx == DIAG_NV_READ_F){
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)resp;

        std::string tmp = bytesToHex((unsigned char *)rxPacket->data, 16, false);
        tmp = tmp.erase(tmp.find_last_not_of("0") + 1);
        QString result = QString::fromStdString(tmp).toUpper();

        ui->pppPasswordValue->setText(result);

        log(LOGTYPE_INFO, "Read Success - PPP Password: " + result);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - PPP Password");
    }
}

/**
* @brief QcdmWindow::ReadHdrAnUserId
*/
void QcdmWindow::ReadHdrAnUserId() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_HDR_AN_AUTH_NAI_I, &resp);

    if (rx == DIAG_NV_READ_F){
        qcdm_nv_alt2_rx_t* rxPacket = (qcdm_nv_alt2_rx_t*)resp;

        std::string tmp = hexToString((char *)rxPacket->data, DIAG_NV_ITEM_SIZE);
        QString result = QString::fromStdString(tmp);
        result = FixedEmptyTrim(result);

        ui->hdrAnUserIdValue->setText(result);

        log(LOGTYPE_INFO, "Read Success - HDR AN User ID: " + result);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - HDR AN User ID");
    }
}

/**
* @brief QcdmWindow::ReadHdrAnPassword
*/
void QcdmWindow::ReadHdrAnPassword() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_HDR_AN_AUTH_PASSWORD_I, &resp);

    if (rx == DIAG_NV_READ_F){
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)resp;

        std::string tmp = bytesToHex((unsigned char *)rxPacket->data, 16, false);
        tmp = tmp.erase(tmp.find_last_not_of("0") + 1);
        QString result = QString::fromStdString(tmp).toUpper();

        ui->hdrAnPasswordValue->setText(result);

        log(LOGTYPE_INFO, "Read Success - HDR AN Password: " + result);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - HDR AN Password");
    }
}

/**
* @brief QcdmWindow::ReadHdrAnLongUserId
*/
void QcdmWindow::ReadHdrAnLongUserId() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_HDR_AN_AUTH_USER_ID_LONG_I, &resp);

    if (rx == DIAG_NV_READ_F){
        qcdm_nv_alt2_rx_t* rxPacket = (qcdm_nv_alt2_rx_t*)resp;

        std::string tmp = hexToString((char *)rxPacket->data, DIAG_NV_ITEM_SIZE);
        QString result = QString::fromStdString(tmp);
        result = FixedEmptyTrim(result);

        ui->hdrAnLongUserIdValue->setText(result);

        log(LOGTYPE_INFO, "Read Success - HDR AN LONG User ID: " + result);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - HDR AN LONG User ID");
    }
}

/**
* @brief QcdmWindow::ReadHdrAnLongPassword
*/
void QcdmWindow::ReadHdrAnLongPassword() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_HDR_AN_AUTH_PASSWORD_LONG_I, &resp);

    if (rx == DIAG_NV_READ_F){
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)resp;

        std::string tmp = bytesToHex((unsigned char *)rxPacket->data, 16, false);
        tmp = tmp.erase(tmp.find_last_not_of("0") + 1);
        QString result = QString::fromStdString(tmp).toUpper();

        ui->hdrAnLongPasswordValue->setText(result);

        log(LOGTYPE_INFO, "Read Success - HDR AN LONG Password: " + result);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - HDR AN LONG Password");
    }
}

/**
* @brief QcdmWindow::ReadHdrAnPppUserId
*/
void QcdmWindow::ReadHdrAnPppUserId() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_HDR_AN_AUTH_USER_ID_PPP_I, &resp);

    if (rx == DIAG_NV_READ_F){
        qcdm_nv_alt2_rx_t* rxPacket = (qcdm_nv_alt2_rx_t*)resp;

        std::string tmp = hexToString((char *)rxPacket->data, DIAG_NV_ITEM_SIZE);
        QString result = QString::fromStdString(tmp);
        result = FixedEmptyTrim(result);

        ui->hdrAnPppUserIdValue->setText(result);

        log(LOGTYPE_INFO, "Read Success - HDR AN PPP User ID: " + result);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - HDR AN PPP User ID");
    }
}

/**
* @brief QcdmWindow::ReadHdrAnPppPassword
*/
void QcdmWindow::ReadHdrAnPppPassword() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_HDR_AN_AUTH_PASSWORD_PPP_I, &resp);

    if (rx == DIAG_NV_READ_F){
        qcdm_nv_alt_rx_t* rxPacket = (qcdm_nv_alt_rx_t*)resp;

        std::string tmp = bytesToHex((unsigned char *)rxPacket->data, 16, false);
        tmp = tmp.erase(tmp.find_last_not_of("0") + 1);
        QString result = QString::fromStdString(tmp).toUpper();

        ui->hdrAnLongPasswordValue->setText(result);

        log(LOGTYPE_INFO, "Read Success - HDR AN PPP Password: " + result);
    } else {
        log(LOGTYPE_ERROR, "Read Failure - HDR AN PPP Password");
    }
}

/**
* @brief QcdmWindow::SpcTextChanged
*/
void QcdmWindow::SpcTextChanged(QString value) {
    if (value.length() == 6) {
        std::string tmp = bytesToHex((unsigned char *)value.toStdString().c_str(), 6, false);
        QString result = QString::fromStdString(tmp);

        ui->hexSpcValue->setText(result);
    }
}

/**
* @brief QcdmWindow::WriteSubscription
* Fix odd QString::trimmed() behavior
*/
QString QcdmWindow::FixedEmptyTrim(QString input) {
    return input.trimmed() == "." ? "" : input.trimmed();
}

/**
* @brief QcdmWindow::DisableUiButtons
*/
void QcdmWindow::DisableUiButtons() {
    ui->readSpcButton->setEnabled(false);
    ui->readMeidButton->setEnabled(false);
    ui->readImeiButton->setEnabled(false);
    ui->writeSpcButton->setEnabled(false);
    ui->writeMeidButton->setEnabled(false);
    ui->writeImeiButton->setEnabled(false);
    ui->readSubscriptionButton->setEnabled(false);
    ui->writeSubscriptionButton->setEnabled(false);
    ui->readNvItemButton->setEnabled(false);
    ui->readNamButton->setEnabled(false);
    ui->writeNamButton->setEnabled(false);
    ui->readPrlButton->setEnabled(false);
    ui->writePrlButton->setEnabled(false);

    ui->sendPhoneModeButton->setEnabled(false);
    ui->sendSpcButton->setEnabled(false);
    ui->sendPasswordButton->setEnabled(false);
}

/**
* @brief QcdmWindow::EnableUiButtons
*/
void QcdmWindow::EnableUiButtons() {
    ui->readSpcButton->setEnabled(true);
    ui->readMeidButton->setEnabled(true);
    ui->readImeiButton->setEnabled(true);
    ui->writeSpcButton->setEnabled(true);
    ui->writeMeidButton->setEnabled(true);
    ui->writeImeiButton->setEnabled(true);
    ui->readSubscriptionButton->setEnabled(true);
    ui->writeSubscriptionButton->setEnabled(true);
    ui->readNvItemButton->setEnabled(true);
    ui->readNamButton->setEnabled(true);
    ui->writeNamButton->setEnabled(true);
    ui->readPrlButton->setEnabled(true);
    ui->writePrlButton->setEnabled(true);

    ui->sendPhoneModeButton->setEnabled(true);
    ui->sendSpcButton->setEnabled(true);
    ui->sendPasswordButton->setEnabled(true);
}

/**
* @brief QcdmWindow::ClearLog
*/
void QcdmWindow::ClearLog() {
    ui->log->clear();
}

/**
* @brief QcdmWindow::SaveLog
*/
void QcdmWindow::SaveLog() {
    log(LOGTYPE_WARNING, "Not Implemented Yet");
}

/**
* @brief QcdmWindow::log
* @param message
*/
void QcdmWindow::log(int type, const char* message) {
    QString newMessage = message;
    log(type, newMessage);
}

/**
* @brief QcdmWindow::log
* @param message
*/
void QcdmWindow::log(int type, std::string message) {
    QString newMessage = message.c_str();
    log(type, newMessage);
}

/**
* @brief QcdmWindow::log
* @param type
* @param message
*/
void QcdmWindow::log(int type, QString message) {
    QString suffix = "</font></pre>";

    switch (type) {
    case 0:
        message = message.prepend("<font color=\"gray\">");
        message = message.prepend("<pre>");
        message = message.append(suffix);
        break;
    case -1:
        message = message.prepend("<font color=\"red\">");
        message = message.prepend("<pre>");
        message = message.append(suffix);
        break;
    case 1:
        message = message.prepend("<font color=\"green\">");
        message = message.prepend("<pre>");
        message = message.append(suffix);
        break;
    case 2:
        message = message.prepend("<font color=\"orange\">");
        message = message.prepend("<pre>");
        message = message.append(suffix);
        break;
    }

    ui->log->appendHtml(message);
}

