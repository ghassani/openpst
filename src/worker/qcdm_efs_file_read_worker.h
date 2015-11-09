/**
* LICENSE PLACEHOLDER
*
* @file qcdm_efs_file_read_worker.h
* @class QcdmEfsFileReadWorker
* @package OpenPST
* @brief Handles background processing of file reading
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#ifndef _WORKER_QCDM_EFS_FILE_READ_WORKER_H
#define _WORKER_QCDM_EFS_FILE_READ_WORKER_H

#include <QThread>
#include "qc/dm_efs_manager.h"

using namespace serial;

namespace OpenPST {

    struct QcdmEfsFileReadWorkerRequest {
        std::string remotePath;
        std::string localPath;
    };

    class QcdmEfsFileReadWorker : public QThread
    {
        Q_OBJECT

    public:
        QcdmEfsFileReadWorker(DmEfsManager& efsManager, QcdmEfsFileReadWorkerRequest request, QObject *parent = 0);
        ~QcdmEfsFileReadWorker();
        void cancel();
    protected:
        DmEfsManager&  efsManager;
        QcdmEfsFileReadWorkerRequest request;

        void run() Q_DECL_OVERRIDE;
        bool cancelled;
    signals:
        void update(QcdmEfsFileReadWorkerRequest request);
        void complete(QcdmEfsFileReadWorkerRequest request);
        void error(QcdmEfsFileReadWorkerRequest request, QString msg);
    };
}

#endif // _WORKER_QCDM_EFS_FILE_READ_WORKER_H