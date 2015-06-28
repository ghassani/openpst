/**
* LICENSE PLACEHOLDER
*
* @file sahara_memory_read_worker.h
* @class SaharaMemoryReadWorker
* @package OpenPST
* @brief Handles background processing of memory reading in sahara mode
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#ifndef _WORKER_SAHARA_MEMORY_READ_WORKER_H
#define _WORKER_SAHARA_MEMORY_READ_WORKER_H

#include <QThread>
#include <serial/sahara_serial.h>
#include <qc/sahara.h>

using namespace serial;

namespace openpst {


	struct sahara_memory_read_worker_request {
		uint32_t		address;
		uint32_t		lastAddress;
		size_t			size;
		size_t			outSize;
		size_t			lastChunkSize;
		std::string		outFilePath;		
	};
	
	class SaharaMemoryReadWorker : public QThread
	{
		Q_OBJECT

		public:
			SaharaMemoryReadWorker(SaharaSerial& port, sahara_memory_read_worker_request request, QObject *parent = 0);
			~SaharaMemoryReadWorker();
			void cancel();
		protected:
			SaharaSerial&  port;
			sahara_memory_read_worker_request request;

			void run() Q_DECL_OVERRIDE;			
			bool cancelled;
		signals:
			void chunkReady(sahara_memory_read_worker_request request);
			void complete(sahara_memory_read_worker_request request);
			void error(sahara_memory_read_worker_request request, QString msg);
	};
}

#endif // _WORKER_SAHARA_MEMORY_READ_WORKER_H