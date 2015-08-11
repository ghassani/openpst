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


	struct sahara_image_transfer_worker_request {
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
			SaharaImageTransferWorker(SaharaSerial& port, sahara_image_transfer_worker_request request, QObject *parent = 0);
			~SaharaImageTransferWorker();
			void cancel();
		protected:
			SaharaSerial&  port;
			sahara_image_transfer_worker_request request;

			void run() Q_DECL_OVERRIDE;			
			bool cancelled;
		signals:
			void chunkDone(sahara_image_transfer_worker_request request);
			void complete(sahara_image_transfer_worker_request request);
			void error(sahara_image_transfer_worker_request request, QString msg);
	};
}

#endif // _WORKER_SAHARA_IMAGE_TRANSFER_WORKER_H