/**
* LICENSE PLACEHOLDER
*
* @file streaming_dload_stream_write_worker.h
* @class StreamingDloadStreamWriteWorker
* @package OpenPST
* @brief Handles background processing of writing data to flash using streaming write 
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#ifndef _WORKER_STREAMING_DLOAD_STREAM_WRITE_WORKER_H
#define _WORKER_STREAMING_DLOAD_STREAM_WRITE_WORKER_H

#include <QThread>
#include "serial/streaming_dload_serial.h"
#include "qc/streaming_dload.h"

using namespace serial;

namespace OpenPST {

	struct StreamingDloadStreamWriteWorkerRequest {
		uint32_t		address;
		std::string		filePath;
		size_t			outSize;
		bool			unframed;
	};

	class StreamingDloadStreamWriteWorker : public QThread
	{
		Q_OBJECT

	public:
		StreamingDloadStreamWriteWorker(StreamingDloadSerial& port, StreamingDloadStreamWriteWorkerRequest request, QObject *parent = 0);
		~StreamingDloadStreamWriteWorker();
		void cancel();
	protected:
		StreamingDloadSerial&  port;
		StreamingDloadStreamWriteWorkerRequest request;

		void run() Q_DECL_OVERRIDE;
		bool cancelled;
	signals:
		void chunkComplete(StreamingDloadStreamWriteWorkerRequest request);
		void complete(StreamingDloadStreamWriteWorkerRequest request);
		void error(StreamingDloadStreamWriteWorkerRequest request, QString msg);
	};
}

#endif // _WORKER_STREAMING_DLOAD_STREAM_WRITE_WORKER_H