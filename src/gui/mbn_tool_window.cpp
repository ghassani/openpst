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

using namespace openpst;

MbnToolWindow::MbnToolWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MbnToolWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->fileBrowseButton, SIGNAL(clicked()), this, SLOT(BrowseForFile()));
    QObject::connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(LoadFile()));
	QObject::connect(ui->readCodeButton, SIGNAL(clicked()), this, SLOT(readCode()));
	QObject::connect(ui->readSignatureButton, SIGNAL(clicked()), this, SLOT(readSignature()));
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

	std::ifstream file(fileName.toStdString(), std::ios::in | std::ios::binary);     

    if (!file.is_open()) {
        log("Error Opening File");
        return;
    }    

	file.seekg(0, file.end);

    size_t fileSize = file.tellg();

	file.seekg(0, file.beg);


	eighty_byte_mbn_header_t sblHeader = {};

	file.read((char *)&sblHeader, sizeof(sblHeader));

	file.close();

    log(tmp.sprintf("File Size: %lu bytes", fileSize));

    // code_size = fileSize - sizeof(eighty_byte_mbn_header_t) - sblHeader.image_size - sblHeader.image_size
    // image_size = fileSize - sizeof(eighty_byte_mbn_header_t)
	bool asDec = ui->displayDataAsSelect->currentText().compare("DEC");

	const char* format = asDec ? "%04X" : "%lu";

    if (ui->flipEndianCheckbox->isChecked()) {
        log(tmp.sprintf("Image Size: %lu bytes", flip_endian32(sblHeader.image_size)));
        log(tmp.sprintf("Code Size: %lu bytes", flip_endian32(sblHeader.code_size)));
        log(tmp.sprintf("Signature Size: %lu bytes", flip_endian32(sblHeader.signature_size)));
        log(tmp.sprintf("Cert Chain Size: %lu bytes", flip_endian32(sblHeader.cert_chain_size)));

        ui->mbnHeaderCodewordValue->setText(tmp.sprintf(format, flip_endian32(sblHeader.codeword)));
        ui->mbnHeaderMagicValue->setText(tmp.sprintf(format, flip_endian32(sblHeader.magic)));
        ui->mbnHeaderImageIdValue->setText(tmp.sprintf(format, flip_endian32(sblHeader.image_id)));
        ui->mbnHeaderReserved1Value->setText(tmp.sprintf(format, flip_endian32(sblHeader.reserved1)));
        ui->mbnHeaderReserved2Value->setText(tmp.sprintf(format, flip_endian32(sblHeader.reserved2)));
        ui->mbnHeaderImageSrcValue->setText(tmp.sprintf(format, flip_endian32(sblHeader.image_src)));
        ui->mbnHeaderImageDestPtrValue->setText(tmp.sprintf(format, flip_endian32(sblHeader.image_dest_ptr)));
        ui->mbnHeaderImageSizeValue->setText(tmp.sprintf(format, flip_endian32(sblHeader.image_size)));
        ui->mbnHeaderCodeSizeValue->setText(tmp.sprintf(format, flip_endian32(sblHeader.code_size)));
		ui->mbnHeaderSignaturePtrValue->setText(tmp.sprintf(format, flip_endian32(sblHeader.signature_ptr)));
        ui->mbnHeaderSignatureSizeValue->setText(tmp.sprintf(format, flip_endian32(sblHeader.signature_size)));
		ui->mbnHeaderCertChainPtrValue->setText(tmp.sprintf(format, flip_endian32(sblHeader.cert_chain_ptr)));
        ui->mbnHeaderCertChainSizeValue->setText(tmp.sprintf(format, flip_endian32(sblHeader.cert_chain_size)));
        ui->mbnHeaderOemRootCertSelectionValue->setText(tmp.sprintf(format, flip_endian32(sblHeader.oem_root_cert_sel)));
        ui->mbnHeaderOemNumberRootCertsValue->setText(tmp.sprintf(format, flip_endian32(sblHeader.oem_num_root_certs)));
        ui->mbnHeaderReserved3Value->setText(tmp.sprintf(format, flip_endian32(sblHeader.reserved3)));
        ui->mbnHeaderReserved4Value->setText(tmp.sprintf(format, flip_endian32(sblHeader.reserved4)));
        ui->mbnHeaderReserved5Value->setText(tmp.sprintf(format, flip_endian32(sblHeader.reserved5)));
        ui->mbnHeaderReserved6Value->setText(tmp.sprintf(format, flip_endian32(sblHeader.reserved6)));
        ui->mbnHeaderReserved7Value->setText(tmp.sprintf(format, flip_endian32(sblHeader.reserved7)));
    } else {
        log(tmp.sprintf("Image Size: %lu bytes", sblHeader.image_size));
        log(tmp.sprintf("Code Size: %lu bytes", sblHeader.code_size));
        log(tmp.sprintf("Signature Size: %lu bytes", sblHeader.signature_size));
        log(tmp.sprintf("Cert Chain Size: %lu bytes", sblHeader.cert_chain_size));

        ui->mbnHeaderCodewordValue->setText(tmp.sprintf(format, sblHeader.codeword));
        ui->mbnHeaderMagicValue->setText(tmp.sprintf(format, sblHeader.magic));
        ui->mbnHeaderImageIdValue->setText(tmp.sprintf(format, sblHeader.image_id));
        ui->mbnHeaderReserved1Value->setText(tmp.sprintf(format, sblHeader.reserved1));
        ui->mbnHeaderReserved2Value->setText(tmp.sprintf(format, sblHeader.reserved2));
        ui->mbnHeaderImageSrcValue->setText(tmp.sprintf(format, sblHeader.image_src));
        ui->mbnHeaderImageDestPtrValue->setText(tmp.sprintf(format, sblHeader.image_dest_ptr));
        ui->mbnHeaderImageSizeValue->setText(tmp.sprintf(format, sblHeader.image_size));
        ui->mbnHeaderCodeSizeValue->setText(tmp.sprintf(format, sblHeader.code_size));
        ui->mbnHeaderSignaturePtrValue->setText(tmp.sprintf(format, sblHeader.signature_ptr));
        ui->mbnHeaderSignatureSizeValue->setText(tmp.sprintf(format, sblHeader.signature_size));
        ui->mbnHeaderCertChainPtrValue->setText(tmp.sprintf(format, sblHeader.cert_chain_ptr));
        ui->mbnHeaderCertChainSizeValue->setText(tmp.sprintf(format, sblHeader.cert_chain_size));
        ui->mbnHeaderOemRootCertSelectionValue->setText(tmp.sprintf(format, sblHeader.oem_root_cert_sel));
        ui->mbnHeaderOemNumberRootCertsValue->setText(tmp.sprintf(format, sblHeader.oem_num_root_certs));
        ui->mbnHeaderReserved3Value->setText(tmp.sprintf(format, sblHeader.reserved3));
        ui->mbnHeaderReserved4Value->setText(tmp.sprintf(format, sblHeader.reserved4));
        ui->mbnHeaderReserved5Value->setText(tmp.sprintf(format, sblHeader.reserved5));
        ui->mbnHeaderReserved6Value->setText(tmp.sprintf(format, sblHeader.reserved6));
        ui->mbnHeaderReserved7Value->setText(tmp.sprintf(format, sblHeader.reserved7));
    }

}

void MbnToolWindow::readCode()
{
	QString tmp;
	QString fileName = ui->filePathInput->text();

	if (!fileName.length()) {
		log("Please Enter or Browse For a Image File");
		return;
	}

	std::ifstream file(fileName.toStdString(), std::ios::in | std::ios::binary);

	file.seekg(0, file.end);
	size_t fileSize = file.tellg();
	file.seekg(0, file.beg);

	if (MBN_HEADER_MAX_SIZE > fileSize) {
		log("File is not large enough to be an mbn image");
		file.close();
		return;
	}

	uint8_t headerBuffer[MBN_HEADER_MAX_SIZE];

	file.read((char*)headerBuffer, MBN_HEADER_MAX_SIZE);

	uint32_t magic = *((uint32_t*)&headerBuffer[sizeof(uint32_t)]);

	if (magic == MBN_EIGHTY_BYTE_MAGIC) {
		eighty_byte_mbn_header_t* header = (eighty_byte_mbn_header_t*)headerBuffer;
		
		size_t codeOffset = header->image_src;
		
		if (header->code_size > fileSize) {
			log("Error. Code size exceeds file size");
			file.close();
			return;
		}

		QString outFilePath = QFileDialog::getSaveFileName(this, tr("Save MBN Image Code Segment"), "", tr("Binary Files (*.bin)"));

		std::ofstream outFile(outFilePath.toStdString(), std::ios::out | std::ios::binary | std::ios::trunc);

		if (!outFile.is_open()) {
			log("Could not open file for writing");
			file.close();
		}

		uint8_t* outBuffer = new uint8_t[header->code_size];

		file.seekg(codeOffset, file.beg);
		
		file.read((char *)outBuffer, header->code_size);

		outFile.write((char *)outBuffer, header->code_size);

		outFile.close();

		free(outBuffer);

	} else {
		log("40 Byte and check not implemented");
	}


	file.close();
}


void MbnToolWindow::readSignature()
{
	QString tmp;
	QString fileName = ui->filePathInput->text();

	if (!fileName.length()) {
		log("Please Enter or Browse For a Image File");
		return;
	}

	std::ifstream file(fileName.toStdString(), std::ios::in | std::ios::binary);

	file.seekg(0, file.end);
	size_t fileSize = file.tellg();
	file.seekg(0, file.beg);

	if (MBN_HEADER_MAX_SIZE > fileSize) {
		log("File is not large enough to be an mbn image");
		file.close();
		return;
	}

	uint8_t headerBuffer[MBN_HEADER_MAX_SIZE];

	file.read((char*)headerBuffer, MBN_HEADER_MAX_SIZE);

	uint32_t magic = *((uint32_t*)&headerBuffer[sizeof(uint32_t)]);

	if (magic == MBN_EIGHTY_BYTE_MAGIC) {
		eighty_byte_mbn_header_t* header = (eighty_byte_mbn_header_t*)headerBuffer;

		size_t signatureOffset = header->image_src + header->code_size;

		if (header->signature_size > fileSize) {
			log("Error. Signature size exceeds file size");
			file.close();
			return;
		}

		QString outFilePath = QFileDialog::getSaveFileName(this, tr("Save MBN Image Signature Segment"), "", tr("Binary Files (*.bin)"));

		std::ofstream outFile(outFilePath.toStdString(), std::ios::out | std::ios::binary | std::ios::trunc);

		if (!outFile.is_open()) {
			log("Could not open file for writing");
			file.close();
		}

		uint8_t* outBuffer = new uint8_t[header->signature_size];

		file.seekg(signatureOffset, file.beg);

		file.read((char *)outBuffer, header->signature_size);

		outFile.write((char *)outBuffer, header->signature_size);

		outFile.close();

		free(outBuffer);

	} else {
		log("40 Byte and check not implemented");
	}


	file.close();
}


void MbnToolWindow::readX509Chain()
{
	QString tmp;
	QString fileName = ui->filePathInput->text();

	if (!fileName.length()) {
		log("Please Enter or Browse For a Image File");
		return;
	}

	std::ifstream file(fileName.toStdString(), std::ios::in | std::ios::binary);

	file.seekg(0, file.end);
	size_t fileSize = file.tellg();
	file.seekg(0, file.beg);

	if (MBN_HEADER_MAX_SIZE > fileSize) {
		log("File is not large enough to be an mbn image");
		file.close();
		return;
	}

	uint8_t headerBuffer[MBN_HEADER_MAX_SIZE];

	file.read((char*)headerBuffer, MBN_HEADER_MAX_SIZE);

	uint32_t magic = *((uint32_t*)&headerBuffer[sizeof(uint32_t)]);

	if (magic == MBN_EIGHTY_BYTE_MAGIC) {
		eighty_byte_mbn_header_t* header = (eighty_byte_mbn_header_t*)headerBuffer;

		size_t x509Offset = header->image_src + header->code_size + header->signature_size;

		if (header->cert_chain_size > fileSize) {
			log("Error. Certificate size exceeds file size");
			file.close();
			return;
		}

		QString outFilePath = QFileDialog::getSaveFileName(this, tr("Save MBN Image X509 Certificate Segment"), "", tr("Binary Files (*.bin)"));

		std::ofstream outFile(outFilePath.toStdString(), std::ios::out | std::ios::binary | std::ios::trunc);

		if (!outFile.is_open()) {
			log("Could not open file for writing");
			file.close();
		}

		uint8_t* outBuffer = new uint8_t[header->cert_chain_size];

		file.seekg(x509Offset, file.beg);

		file.read((char *)outBuffer, header->cert_chain_size);

		outFile.write((char *)outBuffer, header->cert_chain_size);

		outFile.close();

		free(outBuffer);

	} else {
		log("40 Byte and check not implemented");
	}


	file.close();
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

