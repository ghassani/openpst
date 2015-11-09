/**
* LICENSE PLACEHOLDER
*
* @file qcdm_efs_directory_tree_worker.h
* @class QcdmEfsDirectoryTreeWorker
* @package OpenPST
* @brief Handles background processing of directory reading
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#ifndef _WORKER_QCDM_EFS_DIRECTORY_TREE_WORKER_H
#define _WORKER_QCDM_EFS_DIRECTORY_TREE_WORKER_H

#include <QThread>
#include "qc/dm_efs_manager.h"

using namespace serial;

namespace OpenPST {

    struct QcdmEfsDirectoryTreeWorkerRequest {
        bool recursive;
        std::string path;
        std::vector<DmEfsNode> contents;
    };
    
    class QcdmEfsDirectoryTreeWorker : public QThread
    {
        Q_OBJECT

        public:
            QcdmEfsDirectoryTreeWorker(DmEfsManager& efsManager, QcdmEfsDirectoryTreeWorkerRequest request, QObject *parent = 0);
            ~QcdmEfsDirectoryTreeWorker();
            void cancel();
        protected:
            DmEfsManager&  efsManager;
            QcdmEfsDirectoryTreeWorkerRequest request;

            void run() Q_DECL_OVERRIDE;         
            bool cancelled;
        signals:
            void update(QcdmEfsDirectoryTreeWorkerRequest request, std::vector<DmEfsNode> folderContents);
            void complete(QcdmEfsDirectoryTreeWorkerRequest request);
            void error(QcdmEfsDirectoryTreeWorkerRequest request, QString msg);
    };
}

#endif // _WORKER_QCDM_EFS_DIRECTORY_TREE_WORKER_H