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

using namespace openpst;

SaharaMemoryReadWorker::SaharaMemoryReadWorker(SaharaSerial& port, sahara_memory_read_worker_request request, QObject *parent) :
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

#ifdef _WIN32
	FILE* fp;
	fopen_s(&fp, request.outFilePath.c_str(), "a+b");
#else
	FILE* fp = fopen(request.outFilePath.c_str(), "a+b");

	if (!fp) {
		emit error(request, tmp.sprintf("Error opening %s for writing", request.outFilePath.c_str()));
		return;
	}
#endif
	
	if (request.size > 100000) {
		
		size_t requestSize = 100000;

		do {

			// change read size if on the last chunk
			if ((request.size - request.outSize) < requestSize) {
				requestSize = (request.size - request.outSize);
			}

			uint32_t address = request.address + request.outSize;

			if (!port.readMemory(address, requestSize, fp, request.lastChunkSize)) {
				fclose(fp);
				emit error(request, tmp.sprintf("Error reading %lu bytes starting from 0x%08X", requestSize, address));
				return;
			}
						
			request.outSize += request.lastChunkSize;
			request.lastAddress = address;

			emit chunkReady(request);

		} while (request.outSize < request.size && !cancelled);

	} else {
		if (!port.readMemory(request.address, request.size, fp, request.lastChunkSize)) {
			fclose(fp); 
			emit error(request, tmp.sprintf("Error reading %lu bytes starting from 0x%08X", request.size, request.address));
			return;
		}
		request.outSize += request.lastChunkSize;
		request.lastAddress = request.address;

		emit chunkReady(request);
	}

	fclose(fp);

	emit complete(request);
}

