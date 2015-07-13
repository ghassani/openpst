/**
* LICENSE PLACEHOLDER
*
* @file streaming_dload_read_worker.cpp
* @class StreamingDloadReadWorker
* @package OpenPST
* @brief Handles background processing of open/open-multi mode reading
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "streaming_dload_read_worker.h"

using namespace openpst;

StreamingDloadReadWorker::StreamingDloadReadWorker(StreamingDloadSerial& port, streaming_dload_read_worker_request request, QObject *parent) :
    port(port),
    request(request),
    QThread(parent)
{
    cancelled = false;
}

StreamingDloadReadWorker::~StreamingDloadReadWorker()
{

}

void StreamingDloadReadWorker::cancel()
{
    cancelled = true;
}

void StreamingDloadReadWorker::run()
{
	QString tmp;

#ifdef _WIN32
	FILE* fp;
	fopen_s(&fp, request.outFilePath.c_str(), "a+b");
#else
	FILE* fp = fopen(request.outFilePath.c_str(), "a+b");
#endif

	if (!fp) {
		emit error(request, tmp.sprintf("Error opening %s for writing", request.outFilePath.c_str()));
		return;
	}

	if (request.chunkSize > port.state.hello.maxPreferredBlockSize) {
		request.chunkSize = port.state.hello.maxPreferredBlockSize;
	}

	request.outSize = 0;

	
	if (request.size <= request.chunkSize) {
		if (port.readAddress(request.address, request.chunkSize, fp, request.outSize, request.chunkSize) != STREAMING_DLOAD_OPERATION_SUCCESS) {
			fclose(fp); 
			emit error(request, tmp.sprintf("Error reading %lu bytes from address 0x%08X", request.chunkSize, request.address));
			return;
		}

		emit chunkReady(request);
	} else {

		size_t outSize;

		do {
			outSize = 0;

			if (port.readAddress(request.address, request.chunkSize, fp, outSize, request.chunkSize) != STREAMING_DLOAD_OPERATION_SUCCESS) {
				fclose(fp);
				emit error(request, tmp.sprintf("Error reading %lu bytes from address 0x%08X", request.chunkSize, request.address));
				return;
			}

			request.outSize += outSize;

			emit chunkReady(request);

			request.address += outSize;

		} while (request.outSize < request.size && !cancelled);

	}		

	fclose(fp);

	emit complete(request);
}

