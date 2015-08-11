/**
* LICENSE PLACEHOLDER
*
* @file about_dialog.cpp
* @class AboutDialog
* @package OpenPST
* @brief Shared program about dialog
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#include "about_dialog.h"

using namespace OpenPST;

AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent)
{
	ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
	delete ui;
}
