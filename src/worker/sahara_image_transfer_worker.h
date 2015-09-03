/**
* LICENSE PLACEHOLDER
*
* @file sahara_image_transfer_worker.h
* @class SaharaImageTransferWorker
* @package OpenPST
* @brief Handles background processing of image transfers in sahara mode
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#ifndef _WORKER_SAHARA_IMAGE_TRANSFER_WORKER_H
#define _WORKER_SAHARA_IMAGE_TRANSFER_WORKER_H

#include <QThread>
#include "serial/sahara_serial.h"
#include "qc/sahara.h"

using namespace serial;

namespace OpenPST {


	struct SaharaImageTransferWorkerRequest {
		size_t			fileSize;
		size_t			chunkSize;
		uint32_t		offset;
		size_t			sent;
		uint32_t		imageType;
		std::string		imagePath;
	};
	
	class SaharaImageTransferWorker : public QThread
	{
		Q_OBJECT

		public:
			SaharaImageTransferWorker(SaharaSerial& port, SaharaImageTransferWorkerRequest request, QObject *parent = 0);
			~SaharaImageTransferWorker();
			void cancel();
		protected:
			SaharaSerial&  port;
			SaharaImageTransferWorkerRequest request;

			void run() Q_DECL_OVERRIDE;			
			bool cancelled;
		signals:
			void chunkDone(SaharaImageTransferWorkerRequest request);
			void complete(SaharaImageTransferWorkerRequest request);
			void error(SaharaImageTransferWorkerRequest request, QString msg);
	};
}

#endif // _WORKER_SAHARA_IMAGE_TRANSFER_WORKER_H