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
	
}

