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

	struct streaming_dload_stream_write_worker_request {
		uint32_t		address;
		std::string		filePath;
		size_t			outSize;
		bool			unframed;
	};

	class StreamingDloadStreamWriteWorker : public QThread
	{
		Q_OBJECT

	public:
		StreamingDloadStreamWriteWorker(StreamingDloadSerial& port, streaming_dload_stream_write_worker_request request, QObject *parent = 0);
		~StreamingDloadStreamWriteWorker();
		void cancel();
	protected:
		StreamingDloadSerial&  port;
		streaming_dload_stream_write_worker_request request;

		void run() Q_DECL_OVERRIDE;
		bool cancelled;
	signals:
		void chunkComplete(streaming_dload_stream_write_worker_request request);
		void complete(streaming_dload_stream_write_worker_request request);
		void error(streaming_dload_stream_write_worker_request request, QString msg);
	};
}

#endif // _WORKER_STREAMING_DLOAD_STREAM_WRITE_WORKER_H