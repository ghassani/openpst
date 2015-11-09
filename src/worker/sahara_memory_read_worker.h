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
#include "serial/sahara_serial.h"
#include "qc/sahara.h"

using namespace serial;

namespace OpenPST {


    struct SaharaMemoryReadWorkerRequest {
        uint32_t        address;
        uint32_t        lastAddress;
        size_t          size;
        size_t          outSize;
        size_t          lastChunkSize;
        size_t          stepSize;
        std::string     outFilePath;
    };
    
    class SaharaMemoryReadWorker : public QThread
    {
        Q_OBJECT

        public:
            SaharaMemoryReadWorker(SaharaSerial& port, SaharaMemoryReadWorkerRequest request, QObject *parent = 0);
            ~SaharaMemoryReadWorker();
            void cancel();
        protected:
            SaharaSerial&  port;
            SaharaMemoryReadWorkerRequest request;

            void run() Q_DECL_OVERRIDE;         
            bool cancelled;
        signals:
            void chunkReady(SaharaMemoryReadWorkerRequest request);
            void complete(SaharaMemoryReadWorkerRequest request);
            void error(SaharaMemoryReadWorkerRequest request, QString msg);
    };
}

#endif // _WORKER_SAHARA_MEMORY_READ_WORKER_H