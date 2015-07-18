/**
* LICENSE PLACEHOLDER
*
* @file mbn_tool_window.h
* @class MbnToolWindow
* @package OpenPST
* @brief MBN Tool GUI interface class definitions
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#ifndef _GUI_MBNTOOLWINDOW_H
#define _GUI_MBNTOOLWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "ui_mbn_tool_window.h"
#include "qc/mbn.h"
#include "util/hexdump.h"
#include "util/endian.h"
#include <fstream>
#include <ostream>

namespace Ui {
class MbnToolWindow;
}

namespace openpst{
	class MbnToolWindow : public QMainWindow
	{
		Q_OBJECT

		public:
			explicit MbnToolWindow(QWidget *parent = 0);
			~MbnToolWindow();

			public slots:
			void BrowseForFile();
			void LoadFile();
			void readX509Chain();
			void readSignature();
			void readCode();

		private:
			Ui::MbnToolWindow *ui;
			void log(const char* message);
			void log(std::string message);
			void log(QString message);

	};
}
#endif // _GUI_MBNTOOLWINDOW_H
