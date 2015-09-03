/**
* LICENSE PLACEHOLDER
*
* @file qcdm_memory_read_worker.h
* @class QcdmMemoryReadWorker
* @package OpenPST
* @brief Handles background processing of memory reading
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#ifndef _WORKER_QCDM_MEMORY_READ_WORKER_H
#define _WORKER_QCDM_MEMORY_READ_WORKER_H

#include <QThread>
#include "serial/qcdm_serial.h"

using namespace serial;

namespace OpenPST {

	struct QcdmMemoryReadWorkerRequest {

	};

	class QcdmMemoryReadWorker : public QThread
	{
		Q_OBJECT

	public:
		QcdmMemoryReadWorker(QcdmSerial& port, QcdmMemoryReadWorkerRequest request, QObject *parent = 0);
		~QcdmMemoryReadWorker();
		void cancel();
	protected:
		QcdmSerial&  port;
		QcdmMemoryReadWorkerRequest request;

		void run() Q_DECL_OVERRIDE;
		bool cancelled;
	signals:
		void chunkReady(QcdmMemoryReadWorkerRequest request);
		void complete(QcdmMemoryReadWorkerRequest request);
		void error(QcdmMemoryReadWorkerRequest request, QString msg);
	};
}

#endif // _WORKER_QCDM_MEMORY_READ_WORKER_H