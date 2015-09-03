/**
* LICENSE PLACEHOLDER
*
* @file streaming_dload_read_worker.h
* @class StreamingDloadReadWorker
* @package OpenPST
* @brief Handles background processing of open/open-multi mode reading
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#ifndef _WORKER_STREAMING_DLOAD_READ_WORKER_H
#define _WORKER_STREAMING_DLOAD_READ_WORKER_H

#include <QThread>
#include "serial/streaming_dload_serial.h"
#include "qc/streaming_dload.h"

using namespace serial;

namespace OpenPST {

	struct StreamingDloadReadWorkerRequest {
		uint32_t		address;
		size_t			size;
		size_t			stepSize;
		size_t			outSize;
		std::string		outFilePath;		
	};
	
	class StreamingDloadReadWorker : public QThread
	{
		Q_OBJECT

		public:
			StreamingDloadReadWorker(StreamingDloadSerial& port, StreamingDloadReadWorkerRequest request, QObject *parent = 0);
			~StreamingDloadReadWorker();
			void cancel();
		protected:
			StreamingDloadSerial&  port;
			StreamingDloadReadWorkerRequest request;

			void run() Q_DECL_OVERRIDE;			
			bool cancelled;
		signals:
			void chunkReady(StreamingDloadReadWorkerRequest request);
			void complete(StreamingDloadReadWorkerRequest request);
			void error(StreamingDloadReadWorkerRequest request, QString msg);
	};
}

#endif // _WORKER_STREAMING_DLOAD_READ_WORKER_H