/**
* LICENSE PLACEHOLDER
*
* @file qcdm_prl_read_worker.h
* @class QcdmPrlReadWorker
* @package OpenPST
* @brief Handles background processing of reading PRL
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#ifndef _WORKER_QCDM_PRL_READ_WORKER_H
#define _WORKER_QCDM_PRL_READ_WORKER_H

#include <QThread>
#include "serial/qcdm_serial.h"

using namespace serial;

namespace OpenPST {

	struct QcdmPrlReadWorkerRequest {
		int nam;
		std::string outPath;
	};

	class QcdmPrlReadWorker : public QThread
	{
		Q_OBJECT

	public:
		QcdmPrlReadWorker(QcdmSerial& port, QcdmPrlReadWorkerRequest request, QObject *parent = 0);
		~QcdmPrlReadWorker();
		void cancel();
	protected:
		QcdmSerial&  port;
		QcdmPrlReadWorkerRequest request;

		void run() Q_DECL_OVERRIDE;
		bool cancelled;
	signals:
		void chunkReady(QcdmPrlReadWorkerRequest request);
		void complete(QcdmPrlReadWorkerRequest request);
		void error(QcdmPrlReadWorkerRequest request, QString msg);
	};
}

#endif // _WORKER_QCDM_PRL_READ_WORKER_H