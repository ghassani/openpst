/**
* LICENSE PLACEHOLDER
*
* @file sahara_window.h
* @class SaharaWindow
* @package OpenPST
* @brief Sahara GUI interface class definitions
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#ifndef _GUI_SAHARA_WINDOW_H
#define _GUI_SAHARA_WINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegExp>
#include <iostream>
#include <stdio.h>
#include <deque>
#include "ui_sahara_window.h"
#include "serial/sahara_serial.h"
#include "qc/sahara.h"
#include "util/hexdump.h"
#include "util/sleep.h"
#include "util/endian.h"
#include "worker/sahara_memory_read_worker.h"
#include "worker/sahara_image_transfer_worker.h"

using namespace serial;

namespace Ui {
	class SaharaWindow;
}

namespace openpst {

	class SaharaWindow : public QMainWindow
	{
		Q_OBJECT

		public:
			explicit SaharaWindow(QWidget *parent = 0);
			~SaharaWindow();

		public slots:
			/**
			* @brief updatePortList 
			*/
			void updatePortList();

			/**
			* @brief connectToPort
			*/
			void connectToPort();

			/**
			* @brief disconnectPort
			*/
			void disconnectPort();

			/**
			* @brief readHello
			*/
			void readHello();

			/**
			* @brief writeHello
			*/
			void writeHello();

			/**
			* @brief switchMode
			*/
			void switchMode();

			/**
			* @brief sendClientCommand
			*/
			void sendClientCommand();

			/**
			* @brief sendReset
			*/			
			void sendReset();

			/**
			* @brief browseForImage
			*/
			void browseForImage();

			/**
			* @brief sendImage
			*/
			void sendImage();

			/**
			* @brief sendDone
			*/
			void sendDone();

			/**
			* @brief memoryRead
			*/
			void memoryRead();

			/**
			* @brief cancelOperation
			*/
			void cancelOperation();

			/**
			* @brief clearLog
			*/
			void clearLog();

			/**
			* @brief saveLog
			*/
			void saveLog();

			/**
			* @brief memoryReadChunkReadyHandler
			*/
			void memoryReadChunkReadyHandler(sahara_memory_read_worker_request request);

			/**
			* @brief memoryReadCompleteHandler
			*/
			void memoryReadCompleteHandler(sahara_memory_read_worker_request request);

			/**
			* @brief memoryReadChunkErrorHandler
			*/
			void memoryReadChunkErrorHandler(sahara_memory_read_worker_request request, QString msg);

			/**
			* @brief imageTransferChunkDoneHandler
			*/
			void imageTransferChunkDoneHandler(sahara_image_transfer_worker_request request);

			/**
			* @brief imageTransferCompleteHandler
			*/
			void imageTransferCompleteHandler(sahara_image_transfer_worker_request request);

			/**
			* @brief imageTransferErrorHandler
			*/
			void imageTransferErrorHandler(sahara_image_transfer_worker_request request, QString msg);

		private:

			/**
			* @brief memoryReadStartThread
			*/
			void memoryReadStartThread();

			/**
			* @brief disableControls
			*/
			void disableControls();

			/**
			* @brief enableControls
			*/
			void enableControls();

			/**
			* @brief log
			*/
			void log(const char* message);
			
			/**
			* @brief log
			*/
			void log(std::string message);

			/**
			* @brief log
			*/
			void log(QString message);

			Ui::SaharaWindow *ui;
			SaharaSerial port;
			PortInfo currentPort;
			SaharaMemoryReadWorker* memoryReadWorker;
			SaharaImageTransferWorker* imageTransferWorker;
			std::deque<sahara_memory_read_worker_request> memoryReadQueue;
	};
}
#endif // _GUI_SAHARA_WINDOW_H
