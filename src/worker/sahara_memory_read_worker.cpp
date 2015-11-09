/**
* LICENSE PLACEHOLDER
*
* @file sahara_memory_read_worker.cpp
* @class SaharaMemoryReadWorker
* @package OpenPST
* @brief Handles background processing of memory reading in sahara mode
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "sahara_memory_read_worker.h"

using namespace OpenPST;

SaharaMemoryReadWorker::SaharaMemoryReadWorker(SaharaSerial& port, SaharaMemoryReadWorkerRequest request, QObject *parent) :
    port(port),
    request(request),
    QThread(parent)
{
    cancelled = false;
}

SaharaMemoryReadWorker::~SaharaMemoryReadWorker()
{

}
void SaharaMemoryReadWorker::cancel()
{
    cancelled = true;
}
void SaharaMemoryReadWorker::run() 
{

    QString tmp; 
    request.lastAddress = 0x00;
    request.lastChunkSize = 0;
    request.outSize = 0;

    std::ofstream file(request.outFilePath.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

    if (!file.is_open()) {
        emit error(request, "Error opening file for writing");
        return;
    }

    if (request.stepSize > SAHARA_MAX_MEMORY_REQUEST_SIZE) {
        request.stepSize = SAHARA_MAX_MEMORY_REQUEST_SIZE;
    }

    if (request.size > request.stepSize) {
        
        do {

            // change read size if on the last chunk
            if ((request.size - request.outSize) < request.stepSize) {
                request.stepSize = (request.size - request.outSize);
            }

            uint32_t address = request.address + request.outSize;

            int result = port.readMemory(address, request.stepSize, file, request.lastChunkSize);

            if (result != port.kSaharaSuccess) {
                file.close();
                emit error(request, tmp.sprintf("Error reading %lu bytes starting from 0x%08X - %d", request.stepSize, address, result));
                return;
            }
                        
            request.outSize += request.lastChunkSize;
            request.lastAddress = address;

            emit chunkReady(request);

        } while (request.outSize < request.size && !cancelled);

    } else {
        
        int result = port.readMemory(request.address, request.size, file, request.lastChunkSize);
        
        if (result != port.kSaharaSuccess) {
            file.close();
            emit error(request, tmp.sprintf("Error reading %lu bytes starting from 0x%08X %i", request.size, request.address, result));
            return;
        }

        request.outSize += request.lastChunkSize;
        request.lastAddress = request.address;

        emit chunkReady(request);
    }

    file.close();

    emit complete(request);

    return;
}

