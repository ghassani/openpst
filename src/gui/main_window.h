/**
* LICENSE PLACEHOLDER
*
* @file main_window.cpp
* @class MainWindow
* @package OpenPST
* @brief Main Window GUI interface class
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#ifndef _GUI_MAINWINDOW_H
#define _GUI_MAINWINDOW_H

#include <QMainWindow>
#include "ui_main_window.h"
#include "sahara_window.h"
#include "send_raw_window.h"
#include "mbn_tool_window.h"
#include "qcdm_window.h"
#include "streaming_dload_window.h"

namespace Ui {
class MainWindow;
}

namespace openpst {
	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

		public slots:
		void startSaharaMode();
		void startPortRawWriteRead();
		void startMbnTool();
		void startQcdm();
		void startStreamingDload();

	private:
		SaharaWindow		 *saharaWindow;
		SendRawWindow		 *sendRawWindow;
		MbnToolWindow		 *mbnToolWindow;
		QcdmWindow			 *qcdmWindow;
		StreamingDloadWindow *streamingDloadWindow;
		Ui::MainWindow		 *ui;
	};
}
#endif // _GUI_MAINWINDOW_H
