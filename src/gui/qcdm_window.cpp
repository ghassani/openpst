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

using namespace OpenPST;

QcdmWindow::QcdmWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QcdmWindow),
    port("", 115200),
	efsManager(port)
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
	
	QObject::connect(ui->probeCommandsButton, SIGNAL(clicked()), this, SLOT(ProbeCommands()));

	

	// EFS Browse Sub Tab
	QObject::connect(ui->efsListDirectoriesButton, SIGNAL(clicked()), this, SLOT(EfsListDirectories()));
	QObject::connect(ui->efsReadAllButton, SIGNAL(clicked()), this, SLOT(EfsReadAll()));
	QObject::connect(ui->efsSubsysCommandValue, SIGNAL(editingFinished()), this, SLOT(onEfsSubsysCommandCodeChange()));
	QObject::connect(ui->efsSubsysIdValue, SIGNAL(editingFinished()), this, SLOT(onEfsSubsysIdChange()));


	ui->efsDirectoryTree->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->efsDirectoryTree, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onEfsTreeNodeRightClick(const QPoint &)));

	// EFS Manage Sub Tab
	QObject::connect(ui->efsFileRetrieveButton, SIGNAL(clicked()), this, SLOT(EfsGetFile()));
	QObject::connect(ui->efsFileGetInfoButton, SIGNAL(clicked()), this, SLOT(EfsGetFileInfo()));
	QObject::connect(ui->efsFileDeleteButton, SIGNAL(clicked()), this, SLOT(EfsDeleteFile()));

	// File Action Menu
	//QObject::connect(ui->actionExit, SIGNAL(triggered()), this, SLOT());

	// EFS Action Menu 
	QObject::connect(ui->actionEfsHello, SIGNAL(triggered()), this, SLOT(EfsHello()));
	QObject::connect(ui->actionEfsGetDeviceInfo, SIGNAL(triggered()), this, SLOT(EfsGetDeviceInfo()));
	QObject::connect(ui->actionEfsQuery, SIGNAL(triggered()), this, SLOT(EfsQuery()));
	QObject::connect(ui->actionEfsExtractFactoryImage, SIGNAL(triggered()), this, SLOT(EfsExtractFactoryImage()));
	QObject::connect(ui->actionEfsMakeGoldenCopy, SIGNAL(triggered()), this, SLOT(EfsMakeGoldenCopy()));
	QObject::connect(ui->actionEfsFilesystemImage, SIGNAL(triggered()), this, SLOT(EfsFilesystemImage()));

	
	// Help Action Menu
	QObject::connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(About()));


	// TODO: remov eme
	QObject::connect(ui->sendCommandButton, SIGNAL(clicked()), this, SLOT(SendCommand()));
	QObject::connect(ui->readButton, SIGNAL(clicked()), this, SLOT(ReadSome()));

	
	
	qRegisterMetaType<QcdmEfsDirectoryTreeWorkerRequest>("QcdmEfsDirectoryTreeWorkerRequest");
	qRegisterMetaType<QcdmEfsFileReadWorkerRequest>("QcdmEfsFileReadWorkerRequest");
	qRegisterMetaType<QcdmEfsFileWriteWorkerRequest>("QcdmEfsFileWriteWorkerRequest");
	qRegisterMetaType<QcdmMemoryReadWorkerRequest>("QcdmMemoryReadWorkerRequest");
	qRegisterMetaType<QcdmPrlWriteWorkerRequest>("QcdmPrlWriteWorkerRequest");
	

}

/**
* @brief QcdmWindow::~QcdmWindow
*/
QcdmWindow::~QcdmWindow()
{
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

        log(kLogTypeDebug, logMsg);
    }
}

/**
* @brief QcdmWindow::ConnectToPort
*/
void QcdmWindow::ConnectToPort()
{
    QString selected = ui->portListComboBox->currentData().toString();

    if (selected.compare("0") == 0) {
        log(kLogTypeWarning, "Select a Port First");
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
        log(kLogTypeError, "Invalid Port Type");
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
            log(kLogTypeInfo, connectedText);
        }
    }
    catch (serial::IOException e) {
        log(kLogTypeError, "Error Connecting To Serial Port");
        log(kLogTypeError, e.getErrorNumber() == 13 ? "Permission Denied. Try Running With Elevated Privledges." : e.what());
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
        log(kLogTypeInfo, closeText);

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
        log(kLogTypeWarning, "Enter a Valid 6 Digit SPC");
        return;
    }

    int result = port.sendSpc(ui->sendSpcValue->text().toStdString().c_str());

    if (result == DIAG_CMD_TX_FAIL || result == DIAG_CMD_RX_FAIL) {
        log(kLogTypeError, "Error Sending SPC");
        return;
    }

    if (result == DIAG_SPC_REJECT) {
        log(kLogTypeError, "SPC Not Accepted: " + ui->sendSpcValue->text());
        return;
    }

    log(kLogTypeInfo, "SPC Accepted: " + ui->sendSpcValue->text());
}

/**
* @brief QcdmWindow::SendPassword
*/
void QcdmWindow::SendPassword()
{
    if (ui->sendPasswordValue->text().length() != 16) {
        log(kLogTypeWarning, "Enter a Valid 16 Digit Password");
        return;
    }

    int result = port.sendPassword(ui->sendPasswordValue->text().toStdString().c_str());

    if (result == DIAG_CMD_TX_FAIL || result == DIAG_CMD_RX_FAIL) {
        log(kLogTypeError, "Error Sending Password");
        return;
    }

    if (result == DIAG_PASSWORD_REJECT) {
        log(kLogTypeError, "Password Not Accepted: " + ui->sendPasswordValue->text());
        return;
    }

    log(kLogTypeInfo, "Password Accepted: " + ui->sendPasswordValue->text());
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
        log(kLogTypeInfo, "Send Phone Mode Success: " + ui->phoneModeValue->currentText());
    }
    else {
        log(kLogTypeInfo, "Send Phone Mode Failure: " + ui->phoneModeValue->currentText());
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
        QcdmNvResponse* rxPacket = (QcdmNvResponse*)resp;

        std::string tmp = bytesToHex((unsigned char *)rxPacket->data, 6, true);
        QString result = QString::fromStdString(tmp).toUpper();

        ui->hexMeidValue->setText(result);

        log(kLogTypeInfo, "Read Success - MEID: " + result);
    } else {
        log(kLogTypeError, "Read Failure - MEID");
    }
}

/**
* @brief QcdmWindow::WriteMeid
*/
void QcdmWindow::WriteMeid()
{
    if (ui->hexMeidValue->text().length() != 14) {
        log(kLogTypeWarning, "Enter a Valid 14 Character MEID");
    }

    long data = HexToBytes(ui->hexMeidValue->text().toStdString());

    QcdmNvRawRequest packet;
    memcpy(&packet.data, &data, sizeof(data));

    int rx = port.setNvItem(NV_MEID_I, (const char *)&packet, sizeof(packet));

    if (rx == DIAG_NV_WRITE_F) {
        log(kLogTypeInfo, "Write Success - MEID: " + ui->hexMeidValue->text());
    } else {
        log(kLogTypeError, "Write Failure - MEID");
    }
}

/**
* @brief QcdmWindow::ReadImei
*/
void QcdmWindow::ReadImei() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_UE_IMEI_I, &resp);

    if (rx == DIAG_NV_READ_F) {
        QcdmNvResponse* rxPacket = (QcdmNvResponse*)resp;

        std::string tmp = bytesToHex((unsigned char *)rxPacket->data, 8, true);
        QString result = QString::fromStdString(tmp).remove("a");

        ui->imeiValue->setText(result);

        log(kLogTypeInfo, "Read Success - IMEI: " + result);
    } else {
        log(kLogTypeError, "Read Failure - IMEI");
    }
}

/**
* @brief QcdmWindow::ReadNvItem
*/
void QcdmWindow::ReadNvItem() {
    if (ui->nvItemValue->text().length() == 0) {
        log(kLogTypeWarning, "Input a Valid NV Item Number");
        return;
    }

    uint8_t* resp = nullptr;

    int rx = port.getNvItem(ui->nvItemValue->text().toInt(), &resp);

    if (rx == DIAG_NV_READ_F){
        QString result;

        hexdump(resp, sizeof(resp) * 16, result, true);

        log(kLogTypeInfo, "Read Success - Item Number: " + ui->nvItemValue->text() + "<br>" + result);
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
        QcdmNvAltResponse* rxPacket = (QcdmNvAltResponse*)resp;

        std::string tmp = hexToString((char *)rxPacket->data, 9);
        QString mdnValue = QString::fromStdString(tmp);

        ui->mdnValue->setText(mdnValue);

        log(kLogTypeInfo, "Read Success - MDN: " + mdnValue);
    } else {
        log(kLogTypeError, "Read Failure - MDN");
    }
}

/**
* @brief QcdmWindow::WriteMdn
*/
void QcdmWindow::WriteMdn()
{
    if (ui->mdnValue->text().length() != 10) {
        log(kLogTypeWarning, "Enter a Valid 10 Digit MDN");
        return;
    }

    QcdmNvAltRequest packet;
    memcpy(&packet.data, ui->mdnValue->text().toStdString().c_str(), 10);

    int rx = port.setNvItem(NV_DIR_NUMBER_I, (const char *)&packet, 11);

    if (rx == DIAG_NV_WRITE_F) {
        log(kLogTypeInfo, "Write Success - MDN: " + ui->mdnValue->text());
    } else {
        log(kLogTypeError, "Write Failure - MDN");
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
        QcdmNvAltResponse* rxPacket = (QcdmNvAltResponse*)resp;

        min1Chunk[0] = rxPacket->data[3];
        min1Chunk[1] = rxPacket->data[2];
        min1Chunk[2] = rxPacket->data[1];
        min1Chunk[3] = rxPacket->data[0];

        min1 = min1Decode(min1Chunk);
    }

    resp = nullptr;

    rx = port.getNvItem(NV_MIN2_I, &resp);

    if (rx == DIAG_NV_READ_F){
        QcdmNvAltResponse* rxPacket = (QcdmNvAltResponse*)resp;

        min2Chunk[0] = rxPacket->data[3];
        min2Chunk[1] = rxPacket->data[2];

        min2 = min2Decode(min2Chunk);
    }

    QString decodedMin;
    decodedMin.append(QString::fromStdString(min2));
    decodedMin.append(QString::fromStdString(min1));

    if (decodedMin.length() == 10) {
        ui->minValue->setText(decodedMin);

        log(kLogTypeInfo, "Read Success - MIN: " + decodedMin);
    } else {
        log(kLogTypeError, "Read Failure - MIN");
    }
}

/**
* @brief QcdmWindow::ReadSid
*/
void QcdmWindow::ReadSid() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_HOME_SID_NID_I, &resp);

    if (rx == DIAG_NV_READ_F){
        QcdmNvAltResponse* rxPacket = (QcdmNvAltResponse*)resp;

        unsigned char data[2];

        data[0] = rxPacket->data[1];
        data[1] = rxPacket->data[0];

        std::string tmp = sidDecode(data);
        QString result = QString::fromStdString(tmp);
        result = FixedEmptyTrim(result);

        ui->sidValue->setText(result);

        log(kLogTypeInfo, "Read Success - SID: " + result);
    } else {
        log(kLogTypeError, "Read Failure - SID");
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
        QcdmNvAltResponse* rxPacket = (QcdmNvAltResponse*)resp;

        ui->systemPrefValue->setCurrentIndex(rxPacket->data[0] + 1);

        log(kLogTypeInfo, "Read Success - System Pref: " + ui->systemPrefValue->currentText());
    } else {
        log(kLogTypeError, "Read Failure - System Pref");
    }
}

/**
* @brief QcdmWindow::WriteSystemPref
*/
void QcdmWindow::WriteSystemPref()
{
    int mode = ui->systemPrefValue->currentIndex() - 1;

    if (mode < 0) {
        log(kLogTypeWarning, "Select a System Pref to Write");
        return;
    }

    uint8_t* resp = nullptr;

    QcdmNvAltRequest packet;
    memcpy(&packet.data, static_cast<const char *>(static_cast<void*>(&mode)), 1);

    int rx = port.setNvItem(NV_SYSTEM_PREF_I, (const char *)&packet, 2, &resp);

    if (rx == DIAG_NV_WRITE_F) {
        QcdmNvAltResponse* rxPacket = (QcdmNvAltResponse*)resp;

        ui->systemPrefValue->setCurrentIndex(rxPacket->data[0] + 1);

        log(kLogTypeInfo, "Write Success - System Pref: " + ui->systemPrefValue->currentText());
    } else {
        log(kLogTypeError, "Write Failure - System Pref");
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

        QcdmNvAltResponse* rxPacket = (QcdmNvAltResponse*)resp;

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

        log(kLogTypeInfo, "Read Success - Pref Mode: " + result);
    } else {
        log(kLogTypeError, "Read Failure - Pref Mode");
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
        QcdmNvAltResponse* rxPacket = (QcdmNvAltResponse*)resp;

        ui->prefServValue->setCurrentIndex(rxPacket->data[0] + 1);

        log(kLogTypeInfo, "Read Success - Pref Serv: " + ui->prefServValue->currentText());
    } else {
        log(kLogTypeError, "Read Failure - Pref Serv");
    }
}

/**
* @brief QcdmWindow::WritePrefServ
*/
void QcdmWindow::WritePrefServ()
{
    int mode = ui->prefServValue->currentIndex() - 1;

    if (mode < 0) {
        log(kLogTypeWarning, "Select a System Pref to Write");
        return;
    }

    uint8_t* resp = nullptr;

    QcdmNvAltRequest packet;
    memcpy(&packet.data, static_cast<const char *>(static_cast<void*>(&mode)), 1);

    int rx = port.setNvItem(NV_CDMA_PREF_SERV_I, (const char *)&packet, 2, &resp);

    if (rx == DIAG_NV_WRITE_F) {
        QcdmNvAltResponse* rxPacket = (QcdmNvAltResponse*)resp;

        ui->prefServValue->setCurrentIndex(rxPacket->data[0] + 1);

        log(kLogTypeInfo, "Write Success - System Pref: " + ui->prefServValue->currentText());
    } else {
        log(kLogTypeError, "Write Failure - System Pref");
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
        QcdmNvAltResponse* rxPacket = (QcdmNvAltResponse*)resp;

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

        log(kLogTypeInfo, "Read Success - Roam Pref: " + ui->roamPrefValue->currentText());
    } else {
        log(kLogTypeError, "Read Failure - Roam Pref");
    }
}

/**
* @brief QcdmWindow::WriteRoamPref
*/
void QcdmWindow::WriteRoamPref()
{
    int mode = ui->roamPrefValue->currentIndex() - 1;

    if (mode < 0) {
        log(kLogTypeWarning, "Select a Roam Pref to Write");
        return;
    }

    uint8_t* resp = nullptr;

    QcdmNvAltRequest packet;

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
        QcdmNvAltResponse* rxPacket = (QcdmNvAltResponse*)resp;

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

        log(kLogTypeInfo, "Write Success - Roam Pref: " + ui->roamPrefValue->currentText());
    } else {
        log(kLogTypeError, "Write Failure - Roam Pref");
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
        QcdmNvResponse* rxPacket = (QcdmNvResponse*)resp;

        std::string result = hexToString((char *)rxPacket->data, 5);

        ui->decSpcValue->setText(QString::fromStdString(result));

        log(kLogTypeInfo, "Read Success - SPC: " + result);
    } else {
        log(kLogTypeError, "Read Failure - SPC");
    }
}

/**
* @brief QcdmWindow::WriteSpc
*/
void QcdmWindow::WriteSpc()
{
    if (ui->decSpcValue->text().length() != 6) {
        log(kLogTypeWarning, "Enter a Valid 6 Digit SPC");
        return;
    }

    int rx = port.setNvItem(NV_SEC_CODE_I, ui->decSpcValue->text().toStdString().c_str(), 6);

    if (rx == DIAG_NV_WRITE_F) {
        log(kLogTypeInfo, "Write Success - SPC: " + ui->decSpcValue->text());
    } else {
        log(kLogTypeError, "Write Failure - SPC");
    }
}

/**
* @brief QcdmWindow::ReadSubscription
*/
void QcdmWindow::ReadSubscription() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_RTRE_CONFIG_I, &resp);

    if (rx == DIAG_NV_READ_F) {
        QcdmNvResponse* rxPacket = (QcdmNvResponse*)resp;

        ui->subscriptionValue->setCurrentIndex(rxPacket->data[0] + 1);

        log(kLogTypeInfo, "Read Success - Subscription Mode: " + ui->subscriptionValue->currentText());
    } else {
        log(kLogTypeError, "Read Failure - Subscription Mode");
    }
}

/**
* @brief QcdmWindow::WriteSubscription
*/
void QcdmWindow::WriteSubscription()
{
    int mode = ui->subscriptionValue->currentIndex() - 1;

    if (mode < 0) {
        log(kLogTypeWarning, "Select a Subsciption Mode to Write");
        return;
    }

    uint8_t* resp = nullptr;

    const char* data = static_cast<const char *>(static_cast<void*>(&mode));

    int rx = port.setNvItem(NV_RTRE_CONFIG_I, data, 1, &resp);

    if (rx == DIAG_NV_WRITE_F) {
        QcdmNvResponse* rxPacket = (QcdmNvResponse*)resp;

        ui->subscriptionValue->setCurrentIndex(rxPacket->data[0] + 1);

        log(kLogTypeInfo, "Write Success - Subscription Mode: " + ui->subscriptionValue->currentText());
    } else {
        log(kLogTypeError, "Write Failure - Subscription Mode");
    }
}

/**
* @brief QcdmWindow::ReadPapUserId
*/
void QcdmWindow::ReadPapUserId() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_PAP_USER_ID_I, &resp);

    if (rx == DIAG_NV_READ_F){
        QcdmNvAlt2Response* rxPacket = (QcdmNvAlt2Response*)resp;

        std::string tmp = hexToString((char *)rxPacket->data, DIAG_NV_ITEM_SIZE);
        QString result = QString::fromStdString(tmp);
        result = FixedEmptyTrim(result);

        ui->papUserIdValue->setText(result);

        log(kLogTypeInfo, "Read Success - PAP User ID: " + result);
    } else {
        log(kLogTypeError, "Read Failure - PAP User ID");
    }
}

/**
* @brief QcdmWindow::ReadPapPassword
*/
void QcdmWindow::ReadPapPassword() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_PAP_PASSWORD_I, &resp);

    if (rx == DIAG_NV_READ_F){
        QcdmNvAltResponse* rxPacket = (QcdmNvAltResponse*)resp;

        std::string tmp = bytesToHex((unsigned char *)rxPacket->data, 16, false);
        tmp = tmp.erase(tmp.find_last_not_of("0") + 1);
        QString result = QString::fromStdString(tmp).toUpper();

        ui->papPasswordValue->setText(result);

        log(kLogTypeInfo, "Read Success - PAP Password: " + result);
    } else {
        log(kLogTypeError, "Read Failure - PAP Password");
    }
}

/**
* @brief QcdmWindow::ReadPppUserId
*/
void QcdmWindow::ReadPppUserId() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_PPP_USER_ID_I, &resp);

    if (rx == DIAG_NV_READ_F){
        QcdmNvAlt2Response* rxPacket = (QcdmNvAlt2Response*)resp;

        std::string tmp = hexToString((char *)rxPacket->data, DIAG_NV_ITEM_SIZE);
        QString result = QString::fromStdString(tmp);
        result = FixedEmptyTrim(result);

        ui->pppUserIdValue->setText(result);

        log(kLogTypeInfo, "Read Success - PPP User ID: " + result);
    } else {
        log(kLogTypeError, "Read Failure - PPP User ID");
    }
}

/**
* @brief QcdmWindow::ReadPppPassword
*/
void QcdmWindow::ReadPppPassword() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_PPP_PASSWORD_I, &resp);

    if (rx == DIAG_NV_READ_F){
        QcdmNvAltResponse* rxPacket = (QcdmNvAltResponse*)resp;

        std::string tmp = bytesToHex((unsigned char *)rxPacket->data, 16, false);
        tmp = tmp.erase(tmp.find_last_not_of("0") + 1);
        QString result = QString::fromStdString(tmp).toUpper();

        ui->pppPasswordValue->setText(result);

        log(kLogTypeInfo, "Read Success - PPP Password: " + result);
    } else {
        log(kLogTypeError, "Read Failure - PPP Password");
    }
}

/**
* @brief QcdmWindow::ReadHdrAnUserId
*/
void QcdmWindow::ReadHdrAnUserId() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_HDR_AN_AUTH_NAI_I, &resp);

    if (rx == DIAG_NV_READ_F){
        QcdmNvAlt2Response* rxPacket = (QcdmNvAlt2Response*)resp;

        std::string tmp = hexToString((char *)rxPacket->data, DIAG_NV_ITEM_SIZE);
        QString result = QString::fromStdString(tmp);
        result = FixedEmptyTrim(result);

        ui->hdrAnUserIdValue->setText(result);

        log(kLogTypeInfo, "Read Success - HDR AN User ID: " + result);
    } else {
        log(kLogTypeError, "Read Failure - HDR AN User ID");
    }
}

/**
* @brief QcdmWindow::ReadHdrAnPassword
*/
void QcdmWindow::ReadHdrAnPassword() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_HDR_AN_AUTH_PASSWORD_I, &resp);

    if (rx == DIAG_NV_READ_F){
        QcdmNvAltResponse* rxPacket = (QcdmNvAltResponse*)resp;

        std::string tmp = bytesToHex((unsigned char *)rxPacket->data, 16, false);
        tmp = tmp.erase(tmp.find_last_not_of("0") + 1);
        QString result = QString::fromStdString(tmp).toUpper();

        ui->hdrAnPasswordValue->setText(result);

        log(kLogTypeInfo, "Read Success - HDR AN Password: " + result);
    } else {
        log(kLogTypeError, "Read Failure - HDR AN Password");
    }
}

/**
* @brief QcdmWindow::ReadHdrAnLongUserId
*/
void QcdmWindow::ReadHdrAnLongUserId() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_HDR_AN_AUTH_USER_ID_LONG_I, &resp);

    if (rx == DIAG_NV_READ_F){
        QcdmNvAlt2Response* rxPacket = (QcdmNvAlt2Response*)resp;

        std::string tmp = hexToString((char *)rxPacket->data, DIAG_NV_ITEM_SIZE);
        QString result = QString::fromStdString(tmp);
        result = FixedEmptyTrim(result);

        ui->hdrAnLongUserIdValue->setText(result);

        log(kLogTypeInfo, "Read Success - HDR AN LONG User ID: " + result);
    } else {
        log(kLogTypeError, "Read Failure - HDR AN LONG User ID");
    }
}

/**
* @brief QcdmWindow::ReadHdrAnLongPassword
*/
void QcdmWindow::ReadHdrAnLongPassword() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_HDR_AN_AUTH_PASSWORD_LONG_I, &resp);

    if (rx == DIAG_NV_READ_F){
        QcdmNvAltResponse* rxPacket = (QcdmNvAltResponse*)resp;

        std::string tmp = bytesToHex((unsigned char *)rxPacket->data, 16, false);
        tmp = tmp.erase(tmp.find_last_not_of("0") + 1);
        QString result = QString::fromStdString(tmp).toUpper();

        ui->hdrAnLongPasswordValue->setText(result);

        log(kLogTypeInfo, "Read Success - HDR AN LONG Password: " + result);
    } else {
        log(kLogTypeError, "Read Failure - HDR AN LONG Password");
    }
}

/**
* @brief QcdmWindow::ReadHdrAnPppUserId
*/
void QcdmWindow::ReadHdrAnPppUserId() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_HDR_AN_AUTH_USER_ID_PPP_I, &resp);

    if (rx == DIAG_NV_READ_F){
        QcdmNvAlt2Response* rxPacket = (QcdmNvAlt2Response*)resp;

        std::string tmp = hexToString((char *)rxPacket->data, DIAG_NV_ITEM_SIZE);
        QString result = QString::fromStdString(tmp);
        result = FixedEmptyTrim(result);

        ui->hdrAnPppUserIdValue->setText(result);

        log(kLogTypeInfo, "Read Success - HDR AN PPP User ID: " + result);
    } else {
        log(kLogTypeError, "Read Failure - HDR AN PPP User ID");
    }
}

/**
* @brief QcdmWindow::ReadHdrAnPppPassword
*/
void QcdmWindow::ReadHdrAnPppPassword() {
    uint8_t* resp = nullptr;

    int rx = port.getNvItem(NV_HDR_AN_AUTH_PASSWORD_PPP_I, &resp);

    if (rx == DIAG_NV_READ_F){
        QcdmNvAltResponse* rxPacket = (QcdmNvAltResponse*)resp;

        std::string tmp = bytesToHex((unsigned char *)rxPacket->data, 16, false);
        tmp = tmp.erase(tmp.find_last_not_of("0") + 1);
        QString result = QString::fromStdString(tmp).toUpper();

        ui->hdrAnLongPasswordValue->setText(result);

        log(kLogTypeInfo, "Read Success - HDR AN PPP Password: " + result);
    } else {
        log(kLogTypeError, "Read Failure - HDR AN PPP Password");
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

	ui->securityGroupBox->setEnabled(false);
	ui->modeGroupBox->setEnabled(false);
	ui->qcdmTabWidget->setEnabled(false);
}

/**
* @brief QcdmWindow::EnableUiButtons
*/
void QcdmWindow::EnableUiButtons() {
	ui->securityGroupBox->setEnabled(true);
	ui->modeGroupBox->setEnabled(true);
	ui->qcdmTabWidget->setEnabled(true);
}

/**
* @brief QcdmWindow::EfsHello
*/
void QcdmWindow::EfsHello() {
	
	if (!port.isOpen()) {
		log(kLogTypeError, "Connect to a port first");
		return;
	}
	
	QString tmp; 
	diag_subsys_efs_hello_rx_t response;

	if (!efsManager.hello(response)) {
		log(kLogTypeError, "Error sending hello");
	}
	
	log(kLogTypeInfo, tmp.sprintf("Command: 0x%02X - %d", response.header.command, response.header.command));
	log(kLogTypeInfo, tmp.sprintf("Subsys ID: 0x%02X - %d", response.header.subsysId, response.header.subsysId));
	log(kLogTypeInfo, tmp.sprintf("Subsys Command: 0x%02X - %d", response.header.subsysCommand, response.header.subsysCommand));
	log(kLogTypeInfo, tmp.sprintf("Target Packet Window Size: %d", response.targetPacketWindowSize));
	log(kLogTypeInfo, tmp.sprintf("Target Packet Window Size in Bytes: %d", response.targetPacketWindowByteSize));
	log(kLogTypeInfo, tmp.sprintf("Host Packet Window Size: %d", response.hostPacketWindowSize));
	log(kLogTypeInfo, tmp.sprintf("Host Packet Window Size in Bytes: %d", response.hostPacketWindowByteSize));
	log(kLogTypeInfo, tmp.sprintf("Directory Iterator Window Size: %d", response.dirIteratorWindowSize));
	log(kLogTypeInfo, tmp.sprintf("Directory Iterator Window Size in Bytes: %d", response.dirIteratorWindowByteSize));
	log(kLogTypeInfo, tmp.sprintf("Version: %d", response.version));
	log(kLogTypeInfo, tmp.sprintf("Min Version: %d", response.minVersion));
	log(kLogTypeInfo, tmp.sprintf("Feature Bits: 0x%04X", response.featureBits));
}

/**
* @brief QcdmWindow::EfsGetDeviceInfo
*/
void QcdmWindow::EfsGetDeviceInfo() {

	if (!port.isOpen()) {
		log(kLogTypeError, "Connect to a port first");
		return;
	}

	diag_subsys_efs_device_info_rx_t response = {};

	if (!efsManager.getDeviceInfo(response)) {
		log(kLogTypeError, "Error getting device info");
		return;
	}

	QString tmp;
	log(kLogTypeInfo, tmp.sprintf("Command: 0x%02X - %d", response.header.command, response.header.command));
	log(kLogTypeInfo, tmp.sprintf("Subsys ID: 0x%02X - %d", response.header.subsysId, response.header.subsysId));
	log(kLogTypeInfo, tmp.sprintf("Subsys Command: 0x%02X - %d", response.header.subsysCommand, response.header.subsysCommand));
	log(kLogTypeInfo, tmp.sprintf("Error: %d", response.error));
	log(kLogTypeInfo, tmp.sprintf("Total Number of Blocks: %d", response.totalNumberOfBlocks));
	log(kLogTypeInfo, tmp.sprintf("Pages Per Block: %d", response.pagesPerBlock));
	log(kLogTypeInfo, tmp.sprintf("Page Size: %d bytes", response.pageSize));
	log(kLogTypeInfo, tmp.sprintf("Total Page Size: %d", response.totalPageSize));
	log(kLogTypeInfo, tmp.sprintf("Maker ID: %d", response.makerId));
	log(kLogTypeInfo, tmp.sprintf("Device ID: %d", response.deviceId));
	log(kLogTypeInfo, tmp.sprintf("Device Type: %d", response.deviceType));
	log(kLogTypeInfo, tmp.sprintf("Device Name: %s", response.name));
}

/**
* @brief QcdmWindow::EfsQuery
*/
void QcdmWindow::EfsQuery() {
	//efsQueryButton
	if (!port.isOpen()) {
		log(kLogTypeError, "Connect to a port first");
		return;
	}

	diag_subsys_efs_query_rx_t response = {};

	if (!efsManager.query(response)) {
		log(kLogTypeError, "Error querying for efs settings");
		return;
	}

	QString tmp;
	log(kLogTypeInfo, tmp.sprintf("Command: 0x%02X - %d", response.header.command, response.header.command));
	log(kLogTypeInfo, tmp.sprintf("Subsys ID: 0x%02X - %d", response.header.subsysId, response.header.subsysId));
	log(kLogTypeInfo, tmp.sprintf("Subsys Command: 0x%02X - %d", response.header.subsysCommand, response.header.subsysCommand));
	log(kLogTypeInfo, tmp.sprintf("Max File Name Length: %d", response.maxFilenameLength));
	log(kLogTypeInfo, tmp.sprintf("Max Path Name Length: %d", response.mapPathnameLength));
	log(kLogTypeInfo, tmp.sprintf("Max Symlink Depth: %d", response.maxSymlinkDepth));
	log(kLogTypeInfo, tmp.sprintf("Max File Size: %d", response.maxFileSize));
	log(kLogTypeInfo, tmp.sprintf("Max Directories: %d", response.maxDirectories));
	log(kLogTypeInfo, tmp.sprintf("Max Mounts: %d", response.maxMounts));
}



/**
* @brief QcdmWindow::EfsListDirectories
*/
void QcdmWindow::EfsListDirectories() {

	if (!port.isOpen()) {
		log(kLogTypeError, "Connect to a port first");
		return;
	}

	DisableUiButtons();

	QString tmp;

	std::string rootDir = "/";
	std::vector<DmEfsNode> contents;

	

	QcdmEfsDirectoryTreeWorkerRequest request = {};
	QcdmEfsDirectoryTreeWorker* worker = new QcdmEfsDirectoryTreeWorker(efsManager, request, this);
	
	
	workers.insert(workers.end(), reinterpret_cast<QThread*>(worker));

	/*connect(readWorker, &StreamingDloadReadWorker::chunkReady, this, &StreamingDloadWindow::readChunkReadyHandler, Qt::QueuedConnection);
	connect(readWorker, &StreamingDloadReadWorker::complete, this, &StreamingDloadWindow::readCompleteHandler);
	connect(readWorker, &StreamingDloadReadWorker::error, this, &StreamingDloadWindow::readChunkErrorHandler);
	connect(readWorker, &StreamingDloadReadWorker::finished, readWorker, &QObject::deleteLater);*/

	if (!efsManager.readDir(rootDir, contents, true)){
		EnableUiButtons();
		log(kLogTypeError, "Error Reading From Directory /");
		return;
	}
	
	ui->efsDirectoryTree->clear();

	log(kLogTypeInfo, tmp.sprintf("Directory / containts %d files", contents.size()));
	
	QTreeWidgetItem *rootNode = new QTreeWidgetItem(ui->efsDirectoryTree);
	rootNode->setText(kEfsBrowserColumnName, "/");
	rootNode->setIcon(kEfsBrowserColumnName, QIcon(":/images/folder-2x.png"));
	rootNode->setExpanded(true);

	EfsAddTreeNodes(rootNode, contents);

	EnableUiButtons();
}
/**
* @brief QcdmWindow::EfsReadAll
*/
void QcdmWindow::EfsReadAll()
{
	if (!port.isOpen()) {
		log(kLogTypeError, "Connect to a port first");
		return;
	}

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("*.*"));

	if (!fileName.length()) {
		log(kLogTypeInfo, "Operation Cancelled");
		return;
	}

    log(kLogTypeInfo, "Operation Cancelled");

    /*if (efsManager.read("/SUPL/Cert0", fileName.toStdString()) != efsManager.kDmEfsSuccess) {
		log(kLogTypeError, "Error reading file");
    }*/
}

/**
* @brief QcdmWindow::EfsExtractFactoryImage
*/
void QcdmWindow::EfsExtractFactoryImage()
{	
	
	if (!port.isOpen()) {
		log(kLogTypeError, "Connect to a port first");
		return;
	}

	std::vector<uint8_t> data;

	if (efsManager.factoryImagePrepare() == efsManager.kDmEfsSuccess) {
		
		if (efsManager.factoryImageStart() == efsManager.kDmEfsSuccess) {
			
			if (efsManager.factoryImageRead(data) == efsManager.kDmEfsSuccess) {
				
			} else {
				log(kLogTypeError, "Error Reading");
			}

		} else {
			log(kLogTypeError, "Error Starting");
		}

	} else {
		log(kLogTypeError, "Error Preparing");
	}


}
void QcdmWindow::EfsMakeGoldenCopy()
{
	if (!port.isOpen()) {
		log(kLogTypeError, "Connect to a port first");
		return;
	}
	
	if (efsManager.makeGoldenCopy("/") != efsManager.kDmEfsSuccess) {
		log(kLogTypeError, "Error Making Golden Copy Request");
		return;
	}

	log(kLogTypeInfo, "Golden Copy Generating. Device should restart upon successfuly generation.");
}

void QcdmWindow::EfsFilesystemImage()
{
	if (!port.isOpen()) {
		log(kLogTypeError, "Connect to a port first");
		return;
	}

	int32_t handle;
	if (efsManager.openFilesystemImage("/", DIAG_EFS_FILESYSTEM_IMAGE_TAR, handle) != efsManager.kDmEfsSuccess) {
		log(kLogTypeError, "Error Opening For Filesystem Image Request");
		return;
	}

	log(kLogTypeError, "Operation Success. Unimplemented functionality required to continue, stopping.");
}

void QcdmWindow::EfsGetFileInfo()
{
	if (!port.isOpen()) {
		log(kLogTypeError, "Connect to a port first");
		return;
	}

	QString tmp;
	QString path = ui->efsFilePathValue->text();
	std::string md5;

	if (!path.length()) {
		log(kLogTypeError, "Please enter a path");
		return;
	}

	diag_subsys_efs_stat_rx_t statResponse;
	diag_subsys_efs_md5_sum_rx_t md5Response;

	log(kLogTypeInfo, "---------------------------");
	log(kLogTypeInfo, tmp.sprintf("Info For File: %s", path.toStdString().c_str()));
	log(kLogTypeInfo, "---------------------------");

	if (efsManager.stat(path.toStdString().c_str(), statResponse) != efsManager.kDmEfsSuccess) {
		log(kLogTypeError, "Stat Error");
	} else {
		log(kLogTypeInfo, tmp.sprintf("Size: %lu", statResponse.size));
		log(kLogTypeInfo, tmp.sprintf("Mode: %08X", statResponse.mode));
		log(kLogTypeInfo, tmp.sprintf("Link Count: %d", statResponse.linkCount));
		log(kLogTypeInfo, tmp.sprintf("Last Access: %lu", statResponse.atime));
		log(kLogTypeInfo, tmp.sprintf("Last Modified: %lu", statResponse.mtime));
		log(kLogTypeInfo, tmp.sprintf("Creation Time: %lu", statResponse.ctime));
	}

	if (efsManager.md5sum(path.toStdString().c_str(), md5) != efsManager.kDmEfsSuccess) {
		log(kLogTypeError, "MD5 Error");
	} else {
		log(kLogTypeInfo, tmp.sprintf("MD5: %s", md5.c_str()));
	}
	
	log(kLogTypeInfo, "---------------------------");
}

void QcdmWindow::EfsGetFile()
{
	if (!port.isOpen()) {
		log(kLogTypeError, "Connect to a port first");
		return;
	}

	QString tmp;
	QString path = ui->efsFilePathValue->text();

	if (!path.length()) {
		log(kLogTypeError, "Please enter a path");
		return;
	}
		
	QStringList pathParts = path.split("/");
	QString pathFileName = pathParts.value(pathParts.length() - 1);

	QString outFile = QFileDialog::getSaveFileName(this, tr("Save File"), pathFileName, tr("*.*"));

	if (!outFile.length()) {
		log(kLogTypeInfo, "Operation Cancelled");
		return;
	}

	if (efsManager.read(path.toStdString(), outFile.toStdString()) != efsManager.kDmEfsSuccess) {
		log(kLogTypeError, tmp.sprintf("Error reading file %s", path.toStdString().c_str()));
		return;
	}

	log(kLogTypeInfo, tmp.sprintf("File %s saved to %s", path.toStdString().c_str(), outFile.toStdString().c_str()));
}

void QcdmWindow::EfsDeleteFile()
{
	if (!port.isOpen()) {
		log(kLogTypeError, "Connect to a port first");
		return;
	}

	QString tmp;
	QString path = ui->efsFilePathValue->text();

	if (!path.length()) {
		log(kLogTypeError, "Please enter a path");
		return;
	}

	if (efsManager.unlink(path.toStdString()) != efsManager.kDmEfsSuccess) {
		log(kLogTypeError, tmp.sprintf("Error removing file %s", path.toStdString().c_str()));
		return;
	}

	log(kLogTypeInfo, tmp.sprintf("File %s deleted", path.toStdString().c_str()));
}

void QcdmWindow::EfsAddTreeNodes(QTreeWidgetItem* parent, std::vector<DmEfsNode>& entries)
{
	QString tmp;

	for (auto entry : entries) {
		QTreeWidgetItem *treeNode = new QTreeWidgetItem(parent);
		
		treeNode->setText(kEfsBrowserColumnName, entry.name.c_str());
			
		if (entry.isFile()) {
			treeNode->setText(kEfsBrowserColumnType, tr("File"));
			treeNode->setIcon(kEfsBrowserColumnName, QIcon(entry.error ? ":/images/file-protected-2x.png" : ":/images/file-2x.png"));
		} else if (entry.isDir()) {
			treeNode->setText(kEfsBrowserColumnType, tr("Directory"));
			treeNode->setIcon(kEfsBrowserColumnName, QIcon(entry.error ? ":/images/folder-protected-2x.png" : ":/images/folder-2x.png"));
		} else if (entry.isLink()) {
			treeNode->setText(kEfsBrowserColumnType, tr("Link"));
		} else if (entry.isImmovable()) {
			treeNode->setText(kEfsBrowserColumnType, tr("Immovable"));
		} else {
			treeNode->setText(kEfsBrowserColumnType, tr("Unknown"));
		}

		treeNode->setText(kEfsBrowserColumnSize, tmp.sprintf("%lu", entry.size));
		treeNode->setText(kEfsBrowserColumnMode, tmp.sprintf("%08X", entry.mode));
		treeNode->setText(kEfsBrowserColumnATime, tmp.sprintf("%lu", entry.atime));
		treeNode->setText(kEfsBrowserColumnMTime, tmp.sprintf("%lu", entry.mtime));
		treeNode->setText(kEfsBrowserColumnCTime, tmp.sprintf("%lu", entry.ctime));

		if (entry.path.compare("/") == 0) {
			treeNode->setText(kEfsBrowserColumnFullPath, tmp.sprintf("/%s", entry.name.c_str()));
		} else {
			treeNode->setText(kEfsBrowserColumnFullPath, tmp.sprintf("%s%s", entry.path.c_str(), entry.name.c_str()));
		}

		if (entry.children.size()) {
			EfsAddTreeNodes(treeNode, entry.children);
		}
	}
}

void QcdmWindow::onEfsSubsysCommandCodeChange()
{
	if (!ui->efsSubsysCommandValue->isModified()) {
		return;
	}

	uint32_t code = std::stoul(ui->efsSubsysCommandValue->text().toStdString().c_str(), nullptr, 10);

	efsManager.setSubsystemCommand(code);

	log(kLogTypeInfo, "Subsystem Command Code Changed");
}

void QcdmWindow::onEfsSubsysIdChange()
{
	if (!ui->efsSubsysIdValue->isModified()) {
		return;
	}

	uint32_t id = std::stoul(ui->efsSubsysIdValue->text().toStdString().c_str(), nullptr, 10);

	efsManager.setSubsystemId(id);

	log(kLogTypeInfo, "Subsystem ID Changed");

}

void QcdmWindow::About()
{
	aboutDialog = new AboutDialog(this);
	
	aboutDialog->show();
	aboutDialog->raise();
	aboutDialog->activateWindow();

	//AboutDialog dialog(this);
	//dialog.exec(); 
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
    log(kLogTypeWarning, "Not Implemented Yet");
}


/**
* @brief QcdmWindow::log
* @param message
*/
void QcdmWindow::log(int type, const char* message) {
	QString tmp = "gray";

	switch (type) {
	case kLogTypeDebug:		tmp = "gray";	break;
	case kLogTypeError:		tmp = "red";	break;
	case kLogTypeWarning:	tmp = "orange";	break;
	case kLogTypeInfo:		tmp = "green";	break;
	}

	ui->log->appendHtml(tmp.sprintf("<font color=%s><pre>%s</pre></font>", tmp.toStdString().c_str(), message));
}

/**
* @brief QcdmWindow::log
* @param message
*/
void QcdmWindow::log(int type, std::string message) 
{
	QString tmp = "gray";

	switch (type) {
	case kLogTypeDebug:		tmp = "gray";	break;
	case kLogTypeError:		tmp = "red";	break;
	case kLogTypeWarning:	tmp = "orange";	break;
	case kLogTypeInfo:		tmp = "green";	break;
	}

	ui->log->appendHtml(tmp.sprintf("<font color=%s><pre>%s</pre></font>", tmp.toStdString().c_str(), message.c_str())); 
}

/**
* @brief QcdmWindow::log
* @param type
* @param message
*/
void QcdmWindow::log(int type, QString message) 
{
    
	QString tmp = "gray";

    switch (type) {
		case kLogTypeDebug:		tmp = "gray";	break;
		case kLogTypeError:		tmp = "red";	break;		
		case kLogTypeWarning:	tmp = "orange";	break;
		case kLogTypeInfo:		tmp = "green";	break;
    }

	ui->log->appendHtml(tmp.sprintf("<font color=%s><pre>%s</pre></font>", tmp.toStdString().c_str(), message.toStdString().c_str()));
}

void QcdmWindow::onEfsTreeNodeRightClick(const QPoint& point)
{

	if (point.isNull()) {
		return;
	}

	QTreeWidgetItem* node = ui->efsDirectoryTree->itemAt(point);

	QMenu menu(this);

	if (node != nullptr) {
		if (node->text(kEfsBrowserColumnType).compare("file", Qt::CaseInsensitive) == 0) {
			menu.addAction(QIcon(":/images/data-transfer-download-2x.png"), "Save file", this, SLOT(EfsContextMenuSaveFile()))
				->setData(node->text(kEfsBrowserColumnFullPath));
		
			menu.addAction(QIcon(":/images/trash-2.png"), "Delete", this, SLOT(EfsContextMenuDeleteFile()))
				->setData(node->text(kEfsBrowserColumnFullPath));

			menu.addAction(QIcon(":/images/clipboard-2.png"), "Copy path to clipboard", this, SLOT(EfsContextMenuCopyPathToClipboard()))
				->setData(node->text(kEfsBrowserColumnFullPath));

			menu.exec(ui->efsDirectoryTree->mapToGlobal(point));

		} else if (node->text(kEfsBrowserColumnType).compare("directory", Qt::CaseInsensitive)== 0) {
						
			menu.addAction(QIcon(":/images/data-transfer-download-2x.png"), "Save directory", this, SLOT(EfsContextMenuSaveDirectory()))
				->setData(node->text(kEfsBrowserColumnFullPath));

			menu.addAction(QIcon(":/images/data-transfer-download-2x.png"), "Save directory as compressed zip", this, SLOT(EfsContextMenuSaveDirectoryCompressed()))
				->setData(node->text(kEfsBrowserColumnFullPath));

			menu.addAction(QIcon(":/images/data-transfer-upload-2x.png"), "Upload file", this, SLOT(EfsContextMenuUploadFile()))
				->setData(node->text(kEfsBrowserColumnFullPath));
			
			menu.addAction(QIcon(":/images/plus-2x.png"), "Create Directory", this, SLOT(EfsContextMenuCreateDirectory()))
				->setData(node->text(kEfsBrowserColumnFullPath));
			
			menu.addAction(QIcon(":/images/link-2x.png"), "Create Link", this, SLOT(EfsContextMenuCreateLink()))
				->setData(node->text(kEfsBrowserColumnFullPath));

			menu.addAction(QIcon(":/images/clipboard-2.png"), "Copy path to clipboard", this, SLOT(EfsContextMenuCopyPathToClipboard()))
				->setData(node->text(kEfsBrowserColumnFullPath));

			menu.addAction(QIcon(":/images/trash-2x.png"), "Delete", this, SLOT(EfsContextMenuDeleteDirectory()))
				->setData(node->text(kEfsBrowserColumnFullPath));

			menu.exec(ui->efsDirectoryTree->mapToGlobal(point));
		}
	}
	
}

void QcdmWindow::EfsContextMenuSaveFile()
{
	QString tmp;
	QAction* sendingAction = (QAction*)sender();
	QString path = sendingAction->data().toString();

	if (!path.length()) {
		log(kLogTypeError, "Invalid Path");
		return;
	}

	QStringList pathParts = path.split("/");
	QString filename = pathParts.value(pathParts.length() - 1);

	QString outPath = QFileDialog::getSaveFileName(this, tr("Save File"), filename, tr("*.*"));

	if (!outPath.length()) {
		log(kLogTypeInfo, "Operation Cancelled");
		return;
	}

	if (efsManager.read(path.toStdString(), outPath.toStdString()) != efsManager.kDmEfsSuccess) {
		log(kLogTypeError, "Error reading file");
		return;
	}

	log(kLogTypeInfo, tmp.sprintf("File %s saved to %s", path.toStdString().c_str(), outPath.toStdString().c_str()));

}

void QcdmWindow::EfsContextMenuSaveDirectory()
{
	QString tmp;
	QAction* sendingAction = (QAction*)sender();
	QString path = sendingAction->data().toString();

	if (!path.length()) {
		log(kLogTypeError, "Invalid path");
		return;
	}

}

void QcdmWindow::EfsContextMenuSaveDirectoryCompressed()
{
	QString tmp;
	QAction* sendingAction = (QAction*)sender();
	QString path = sendingAction->data().toString();

	if (!path.length()) {
		log(kLogTypeError, "Invalid path");
		return;
	}


}

void QcdmWindow::EfsContextMenuUploadFile()
{
	QString tmp;
	QAction* sendingAction = (QAction*)sender();
	QString path = sendingAction->data().toString();

	if (!path.length()) {
		log(kLogTypeError, "Invalid path");
		return;
	}
}

void QcdmWindow::EfsContextMenuCopyPathToClipboard()
{
	QAction* sendingAction = (QAction*)sender();
	QString path = sendingAction->data().toString();

	if (path.length()) {
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setText(path);
		log(kLogTypeInfo, "Path copied to clipboard");
	}

}


void QcdmWindow::EfsContextMenuDeleteDirectory()
{
	QString tmp;
	QAction* sendingAction = (QAction*)sender();
	QString path = sendingAction->data().toString();

	if (!path.length()) {
		log(kLogTypeError, "Invalid Path");
		return;
	}

	QMessageBox::StandardButton confirmDelete = QMessageBox::question(this, tr("Confirm Directory Deletion"), tmp.sprintf("Really remove directory %s and all its contents?", path.toStdString().c_str()));

	if (confirmDelete != QMessageBox::Yes) {
		return;
	}

	if (efsManager.deltree(path.toStdString()) != efsManager.kDmEfsSuccess) {
		log(kLogTypeError, tmp.sprintf("Error deleting %s", path.toStdString().c_str()));
		return;
	}

	log(kLogTypeInfo, tmp.sprintf("Directory %s deleted", path.toStdString().c_str()));

}


void QcdmWindow::EfsContextMenuDeleteFile()
{

	QString tmp;
	QAction* sendingAction = (QAction*)sender();
	QString path = sendingAction->data().toString();

	if (!path.length()) {
		log(kLogTypeError, "Invalid Path");
		return;
	}

	QMessageBox::StandardButton confirmDelete = QMessageBox::question(this, tr("Confirm File Deletion"), tmp.sprintf("Really delete %s?", path.toStdString().c_str()));

	if (confirmDelete != QMessageBox::Yes) {
		return;
	}

	if (efsManager.unlink(path.toStdString()) != efsManager.kDmEfsSuccess) {
		log(kLogTypeError, tmp.sprintf("Error deleting %s", path.toStdString().c_str()));
		return;
	}

	log(kLogTypeInfo, tmp.sprintf("File %s deleted", path.toStdString().c_str()));
}


void QcdmWindow::EfsContextMenuCreateDirectory()
{

	QString tmp;
	QAction* sendingAction = (QAction*)sender();
	QString basePath = sendingAction->data().toString();
	QString path;

	if (!basePath.length()) {
		log(kLogTypeError, "Invalid Path");
		return;
	}

	QString directoryName;
	int mode;
	bool collectedName;
	bool collectedMode;
	
	directoryName = QInputDialog::getText(this, tr("Directory Name"), tr("Directory name:"), QLineEdit::Normal, NULL, &collectedName);
	
	if (collectedName && !directoryName.isEmpty()) {
		
		mode = QInputDialog::getInt(this, tr("Directory Mode"), tr("Mode:"), 644, 0, 1000, 1, &collectedMode);
		
		if (collectedMode && mode > 0) {
			path.sprintf("%s%s", basePath.toStdString().c_str(), directoryName.toStdString().c_str());

			if (efsManager.mkdir(path.toStdString(), mode) != efsManager.kDmEfsSuccess) {
				log(kLogTypeError, "Error creating directory");
				return;
			}

			log(kLogTypeInfo, tmp.sprintf("Directory %s created", path.toStdString().c_str()));
		}
	}		
}


void QcdmWindow::EfsContextMenuCreateLink()
{
	QString tmp;
	QAction* sendingAction = (QAction*)sender();
	QString path = sendingAction->data().toString();

	if (!path.length()) {
		log(kLogTypeError, "Invalid Path");
		return;
	}

	bool collectedTarget, collectedName;

	QString targetPath = QInputDialog::getText(this, tr("Target Path"), tr("Target Path:"), QLineEdit::Normal, NULL, &collectedTarget);

	if (collectedTarget && !targetPath.isEmpty()) {
		QString name = QInputDialog::getText(this, tr("Link Name"), tr("Link name:"), QLineEdit::Normal, NULL, &collectedName);
		if (collectedName && !name.isEmpty()) {
			path.append(name);
			if (efsManager.symlink(targetPath.toStdString(), path.toStdString()) != efsManager.kDmEfsSuccess) {
				log(kLogTypeError, tmp.sprintf("Error creating link from %s to %s", path.toStdString().c_str(), targetPath.toStdString().c_str()));
			} else {
				log(kLogTypeInfo, tmp.sprintf("Created link from %s to %s", path.toStdString().c_str(), targetPath.toStdString().c_str()));
			}
		}
	}

}


void QcdmWindow::ProbeCommands()
{

	//port.getLog();


	/*QcdmVersionResponse version = {};
	
	port.getVersion(version);

	printf("Version: %d\n", version.version);
	*/

	/*
	QcdmQueryQpstCookieRequest r = {};
	r.header.command = DIAG_SUBSYS_CMD_F;
	r.header.subsysId = DIAG_SUBSYS_DIAG_SERV;
	r.header.subsysCommand = 0x0054;
	r.version = 1;
	*/
	QcdmTestStruct r = {};
	r.header.command = DIAG_SUBSYS_CMD_F;
	r.header.subsysId = DIAG_SUBSYS_DIAG_SERV;
	r.header.subsysCommand = 0x080A;
	r.processorId = 0x00;

	port.write((uint8_t*)&r, sizeof(r));

	uint8_t b[DIAG_MAX_PACKET_SIZE];
	port.read(b, DIAG_MAX_PACKET_SIZE);

	return;


	QString tmp;
	uint8_t buf[DIAG_MAX_PACKET_SIZE];

	QcdmGenericRequest packet = {};

	size_t rxSize, txSize;

	for (int i = 0; i <= DIAG_MAX_F; i++) {
		packet.command = i;

		if (i == DIAG_DLOAD_F) {
			continue;
		}
		
		
		
		try {
			txSize = port.write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
			
			if (!txSize) {
				continue;
			}

			rxSize = port.read(buf, DIAG_MAX_PACKET_SIZE);

			if (!rxSize) {
				continue;
			}

		}
		catch (...) {
			log(kLogTypeError, "Encountered Exception, Possible Reset");
			return;
		}

		tmp.sprintf("Command %d - 0x%02X - Response %d - 0x%02X - Resp Size: %lu\n", i, i, buf[0], buf[0], rxSize);

		if (buf[0] == i) {
			printf(tmp.toStdString().c_str());
			log(kLogTypeInfo, tmp);
			tmp.clear();
			hexdump(buf, rxSize, tmp, true);
			tmp.append("<br />");
			log(kLogTypeDebug, tmp);
		}
		else if (buf[0] != DIAG_BAD_CMD_F) {
			tmp.prepend("ERROR - ");
			log(kLogTypeError, tmp);
			if (rxSize > 2) {
				tmp.clear();
				hexdump(buf, rxSize, tmp, true);
				tmp.append("<br />");
				log(kLogTypeDebug, tmp);

			}
		}

	}
}

void QcdmWindow::ReadSome()
{
	uint8_t buf[DIAG_MAX_PACKET_SIZE];

	size_t readSize = 1;

	while (readSize > 0) {
		readSize = port.read(buf, DIAG_MAX_PACKET_SIZE, false);
		hexdump(buf, readSize);
		printf("\n");
	}
}

void QcdmWindow::SendCommand()
{
	uint8_t command = std::stoul(ui->sendCommandValue->text().toStdString().c_str(), nullptr, 16);

	QString tmp;
	uint8_t buf[DIAG_MAX_PACKET_SIZE];

	QcdmGenericRequest packet = { command };
	size_t txSize = port.write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
	hexdump(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));


}
