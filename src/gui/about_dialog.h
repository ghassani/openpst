/**
* LICENSE PLACEHOLDER
*
* @file about_dialog.h
* @class AboutDialog
* @package OpenPST
* @brief Shared program about dialog
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#ifndef _GUI_ABOUT_DIALOG_H
#define _GUI_ABOUT_DIALOG_H

#include <QMainWindow>
#include "ui_about_dialog.h"

namespace Ui {
	class AboutDialog;
}

namespace OpenPST{
	class AboutDialog : public QDialog
	{

		Q_OBJECT

		public:
			explicit AboutDialog(QWidget *parent = 0);
			~AboutDialog();


		public slots:

		private:
			Ui::AboutDialog *ui;

		};
}
#endif // _GUI_ABOUT_DIALOG_H
