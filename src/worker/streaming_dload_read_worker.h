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

namespace openpst {


	struct streaming_dload_read_worker_request {
		uint32_t		address;
		uint32_t		lastAddress;
		size_t			size;
		size_t			outSize;
		size_t			lastChunkSize;
		std::string		outFilePath;		
	};
	
	class StreamingDloadReadWorker : public QThread
	{
		Q_OBJECT

		public:
			StreamingDloadReadWorker(StreamingDloadSerial& port, streaming_dload_read_worker_request request, QObject *parent = 0);
			~StreamingDloadReadWorker();
			void cancel();
		protected:
			StreamingDloadSerial&  port;
			streaming_dload_read_worker_request request;

			void run() Q_DECL_OVERRIDE;			
			bool cancelled;
		signals:
			void chunkReady(streaming_dload_read_worker_request request);
			void complete(streaming_dload_read_worker_request request);
			void error(streaming_dload_read_worker_request request, QString msg);
	};
}

#endif // _WORKER_STREAMING_DLOAD_READ_WORKER_H