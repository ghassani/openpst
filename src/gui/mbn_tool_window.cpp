#include "mbn_tool_window.h"

MbnToolWindow::MbnToolWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MbnToolWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->fileBrowseButton, SIGNAL(clicked()), this, SLOT(BrowseForFile()));
    QObject::connect(ui->inspectButton, SIGNAL(clicked()), this, SLOT(InspectFile()));

}

MbnToolWindow::~MbnToolWindow()
{
    delete ui;
}


void MbnToolWindow::BrowseForFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select Image To Inspect", "", "Image Files (*.mbn *.bin *.img)");

    if (fileName.length()) {
        ui->filePathInput->setText(fileName);
    }
}

void MbnToolWindow::InspectFile()
{
    QString msgTmp;
    QString fileName = ui->filePathInput->text();

    log(msgTmp.sprintf("Size: %zd", sizeof(mbn_header_t)));

    if (!fileName.length()) {
        log("Please Enter or Browse For a Image File");
        return;
    }

    FILE* fp = fopen(fileName.toStdString().c_str(), "rb");

    if (!fp) {
        log("Error Opening File");
        return;
    }

    fseek(fp, 8, SEEK_SET);

    mbn_header_t header;
    size_t bytesRead = fread((uint8_t*)&header, 1, sizeof(mbn_header_t), fp);

    logHex((uint8_t*)&header, sizeof(mbn_header_t));


    log(msgTmp.sprintf("Image ID: 0x%04X", header.image_id));
    log(msgTmp.sprintf("Header Version: 0x%04X", header.header_vsn_num));
    log(msgTmp.sprintf("Image Src Offset: 0x%04X", header.image_src));
    log(msgTmp.sprintf("Image Dest Ptr: 0x%08X", header.image_dest_ptr));
    log(msgTmp.sprintf("Image Size: 0x%04X - %zd Bytes", header.image_size, header.image_size));
    log(msgTmp.sprintf("Code Size: 0x%04X - %zd Bytes", header.code_size, header.code_size));
    log(msgTmp.sprintf("Signature Ptr: 0x%08X", header.signature_ptr));
    log(msgTmp.sprintf("Signature Size: 0x%04X - %zd Bytes", header.signature_size, header.signature_size));
    log(msgTmp.sprintf("Cert Chain Ptr: 0x%08X", header.cert_chain_ptr));
    log(msgTmp.sprintf("Cert Chain Size: 0x%04X - %zd Bytes", header.cert_chain_size, header.cert_chain_size));


    rewind(fp);


    printf("SBL Header:\n");

    sbl_mbn_header_t sblHeader;

    bytesRead = fread((uint8_t*)&sblHeader, 1, sizeof(sbl_mbn_header_t), fp);

    logHex((uint8_t*)&sblHeader, sizeof(sbl_mbn_header_t));

    fclose(fp);

    log(msgTmp.sprintf("Image ID: 0x%04X", sblHeader.image_id));
   // log(msgTmp.sprintf("Header Version: 0x%04X", sblHeader.header_vsn_num));
    log(msgTmp.sprintf("Image Src Offset: 0x%04X", sblHeader.image_src));
    log(msgTmp.sprintf("Image Dest Ptr: 0x%08X", sblHeader.image_dest_ptr));
    log(msgTmp.sprintf("Image Size: 0x%04X - %zd Bytes", sblHeader.image_size, sblHeader.image_size));
    log(msgTmp.sprintf("Code Size: 0x%04X - %zd Bytes", sblHeader.code_size, sblHeader.code_size));
    log(msgTmp.sprintf("Signature Ptr: 0x%08X", sblHeader.signature_ptr));
    log(msgTmp.sprintf("Signature Size: 0x%04X - %zd Bytes", sblHeader.signature_size, sblHeader.signature_size));
    log(msgTmp.sprintf("Cert Chain Ptr: 0x%08X", sblHeader.cert_chain_ptr));
    log(msgTmp.sprintf("Cert Chain Size: 0x%04X - %zd Bytes", sblHeader.cert_chain_size, sblHeader.cert_chain_size));
}


/**
 * @brief MbnToolWindow::log
 * @param message
 */
void MbnToolWindow::log(const char* message)
{
    ui->log->appendPlainText(message);
}
/**
 * @brief MbnToolWindow::log
 * @param message
 */
void MbnToolWindow::log(std::string message)
{
    ui->log->appendPlainText(message.c_str());
}

/**
 * @brief MbnToolWindow::log
 * @param message
 */
void MbnToolWindow::log(QString message)
{
    ui->log->appendPlainText(message);
}

/**
 * @brief MbnToolWindow::logHex
 * @param data
 * @param amount
 */
void MbnToolWindow::logHex(uint8_t* data, size_t amount)
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
