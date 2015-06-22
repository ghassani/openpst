/**
* LICENSE PLACEHOLDER
*
* @file mbn_tool_window.cpp
* @class MbnToolWindow
* @package OpenPST
* @brief MBN Tool GUI interface class
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "mbn_tool_window.h"

MbnToolWindow::MbnToolWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MbnToolWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->fileBrowseButton, SIGNAL(clicked()), this, SLOT(BrowseForFile()));
    QObject::connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(LoadFile()));
    QObject::connect(ui->readx509ChainButton, SIGNAL(clicked()), this, SLOT(readX509Chain()));
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

void MbnToolWindow::LoadFile()
{
    QString tmp;
    QString fileName = ui->filePathInput->text();

    if (!fileName.length()) {
        log("Please Enter or Browse For a Image File");
        return;
    }

    FILE* fp = fopen(fileName.toStdString().c_str(), "rb");

    if (!fp) {
        log("Error Opening File");
        return;
    }    

    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);
    rewind(fp);


    ui->savePathInput->setText(fileName.replace("\.[A-Za-z]{1,}$", "_edited$1"));

    log(tmp.sprintf("SBL Header Size: %lu", sizeof(eighty_byte_mbn_header_t)));

    eighty_byte_mbn_header_t sblHeader;

    size_t bytesRead = fread((uint8_t*)&sblHeader, 1, sizeof(eighty_byte_mbn_header_t), fp);

    //logHex((uint8_t*)&sblHeader, sizeof(eighty_byte_mbn_header_t));

    fclose(fp);

    log(tmp.sprintf("File Size: %lu bytes", fileSize));

    // code_size = fileSize - sizeof(eighty_byte_mbn_header_t) - sblHeader.image_size - sblHeader.image_size
    // image_size = fileSize - sizeof(eighty_byte_mbn_header_t)

    if (ui->flipEndianCheckbox->isChecked()) {
        log(tmp.sprintf("Image Size: %lu bytes", flip_endian32(sblHeader.image_size)));
        log(tmp.sprintf("Code Size: %lu bytes", flip_endian32(sblHeader.code_size)));
        log(tmp.sprintf("Signature Size: %lu bytes", flip_endian32(sblHeader.signature_size)));
        log(tmp.sprintf("Cert Chain Size: %lu bytes", flip_endian32(sblHeader.cert_chain_size)));

        ui->mbnHeaderCodewordValue->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.codeword)));
        ui->mbnHeaderMagicValue->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.magic)));
        ui->mbnHeaderImageIdValue->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.image_id)));
        ui->mbnHeaderReserved1Value->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.reserved1)));
        ui->mbnHeaderReserved2Value->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.reserved2)));
        ui->mbnHeaderImageSrcValue->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.image_src)));
        ui->mbnHeaderImageDestPtrValue->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.image_dest_ptr)));
        ui->mbnHeaderImageSizeValue->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.image_size)));
        ui->mbnHeaderCodeSizeValue->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.code_size)));
        ui->mbnHeaderSignaturePtrValue->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.signature_ptr)));
        ui->mbnHeaderSignatureSizeValue->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.signature_size)));
        ui->mbnHeaderCertChainPtrValue->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.cert_chain_ptr)));
        ui->mbnHeaderCertChainSizeValue->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.cert_chain_size)));
        ui->mbnHeaderOemRootCertSelectionValue->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.oem_root_cert_sel)));
        ui->mbnHeaderOemNumberRootCertsValue->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.oem_num_root_certs)));
        ui->mbnHeaderReserved3Value->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.reserved3)));
        ui->mbnHeaderReserved4Value->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.reserved4)));
        ui->mbnHeaderReserved5Value->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.reserved5)));
        ui->mbnHeaderReserved6Value->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.reserved6)));
        ui->mbnHeaderReserved7Value->setText(tmp.sprintf("%04X", flip_endian32(sblHeader.reserved7)));
    } else {
        log(tmp.sprintf("Image Size: %lu bytes", sblHeader.image_size));
        log(tmp.sprintf("Code Size: %lu bytes", sblHeader.code_size));
        log(tmp.sprintf("Signature Size: %lu bytes", sblHeader.signature_size));
        log(tmp.sprintf("Cert Chain Size: %lu bytes", sblHeader.cert_chain_size));

        ui->mbnHeaderCodewordValue->setText(tmp.sprintf("%04X", sblHeader.codeword));
        ui->mbnHeaderMagicValue->setText(tmp.sprintf("%04X", sblHeader.magic));
        ui->mbnHeaderImageIdValue->setText(tmp.sprintf("%04X", sblHeader.image_id));
        ui->mbnHeaderReserved1Value->setText(tmp.sprintf("%04X", sblHeader.reserved1));
        ui->mbnHeaderReserved2Value->setText(tmp.sprintf("%04X", sblHeader.reserved2));
        ui->mbnHeaderImageSrcValue->setText(tmp.sprintf("%04X", sblHeader.image_src));
        ui->mbnHeaderImageDestPtrValue->setText(tmp.sprintf("%04X", sblHeader.image_dest_ptr));
        ui->mbnHeaderImageSizeValue->setText(tmp.sprintf("%04X", sblHeader.image_size));
        ui->mbnHeaderCodeSizeValue->setText(tmp.sprintf("%04X", sblHeader.code_size));
        ui->mbnHeaderSignaturePtrValue->setText(tmp.sprintf("%04X", sblHeader.signature_ptr));
        ui->mbnHeaderSignatureSizeValue->setText(tmp.sprintf("%04X", sblHeader.signature_size));
        ui->mbnHeaderCertChainPtrValue->setText(tmp.sprintf("%04X", sblHeader.cert_chain_ptr));
        ui->mbnHeaderCertChainSizeValue->setText(tmp.sprintf("%04X", sblHeader.cert_chain_size));
        ui->mbnHeaderOemRootCertSelectionValue->setText(tmp.sprintf("%04X", sblHeader.oem_root_cert_sel));
        ui->mbnHeaderOemNumberRootCertsValue->setText(tmp.sprintf("%04X", sblHeader.oem_num_root_certs));
        ui->mbnHeaderReserved3Value->setText(tmp.sprintf("%04X", sblHeader.reserved3));
        ui->mbnHeaderReserved4Value->setText(tmp.sprintf("%04X", sblHeader.reserved4));
        ui->mbnHeaderReserved5Value->setText(tmp.sprintf("%04X", sblHeader.reserved5));
        ui->mbnHeaderReserved6Value->setText(tmp.sprintf("%04X", sblHeader.reserved6));
        ui->mbnHeaderReserved7Value->setText(tmp.sprintf("%04X", sblHeader.reserved7));
    }

}

void MbnToolWindow::readX509Chain()
{
    QString tmp;
    QString fileName = ui->filePathInput->text();

    if (!fileName.length()) {
        log("Please Enter or Browse For a Image File");
        return;
    }

    FILE* fp = fopen(fileName.toStdString().c_str(), "rb");

    if (!fp) {
        log("Error Opening File");
        return;
    }

    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);
    rewind(fp);

    eighty_byte_mbn_header_t sblHeader;

    fread((uint8_t*)&sblHeader, 1, sizeof(eighty_byte_mbn_header_t), fp);

    rewind(fp);

    uint32_t codeSize     = sblHeader.code_size;
    uint32_t imgSize       = sblHeader.image_size;
    uint32_t imgSrc       = sblHeader.image_src;
    uint32_t certChainPtr = flip_endian32(sblHeader.cert_chain_ptr);
    uint32_t certSize     = sblHeader.cert_chain_size;
    uint32_t sigSize     = sblHeader.signature_size;
    uint32_t offset       = sizeof(eighty_byte_mbn_header_t) + imgSrc + codeSize;

    if (offset > fileSize) {
        log(tmp.sprintf("Offset Exceeds File Size: %u | %u %u %u %u", fileSize, offset, sizeof(eighty_byte_mbn_header_t), imgSrc, codeSize));
        return;
    }

    uint8_t* fileBuffer = new uint8_t[fileSize];

    fread(fileBuffer, 1, fileSize, fp);

    uint8_t* sig = &fileBuffer[offset];

    hexdump(sig, sigSize);

    uint8_t* cert = &fileBuffer[offset + sigSize];

    hexdump(cert, certSize);



    free(fileBuffer);

    fclose(fp);
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
