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

// Can't build on Linux unless this is these are commented out. Output of make below...
// Otherwise, building on Linux via QT Creator is taken care of.

// g++ -c -pipe -g -std=c++0x -Wall -W -D_REENTRANT -fPIC -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I../../qmake -I. -I../../src -I../../lib/serial/include -isystem /usr/include/qt5 -isystem /usr/include/qt5/QtWidgets -isystem /usr/include/qt5/QtGui -isystem /usr/include/qt5/QtCore -I. -I. -I/usr/lib64/qt5/mkspecs/linux-g++ -o streaming_dload_read_worker.o ../../src/worker/streaming_dload_read_worker.cpp
// In file included from ../../src/worker/streaming_dload_read_worker.cpp:12:0:
// ../../src/worker/streaming_dload_read_worker.h: In constructor 'openpst::StreamingDloadReadWorker::StreamingDloadReadWorker(openpst::StreamingDloadSerial&, openpst::streaming_dload_read_worker_request, QObject*)':
// ../../src/worker/streaming_dload_read_worker.h:42:40: warning: 'openpst::StreamingDloadReadWorker::request' will be initialized after [-Wreorder]
//     streaming_dload_read_worker_request request;
//                                         ^
// ../../src/worker/streaming_dload_read_worker.cpp:19:19: warning:   base 'QThread' [-Wreorder]
//      QThread(parent)
//                    ^
// ../../src/worker/streaming_dload_read_worker.cpp:16:1: warning:   when initialized here [-Wreorder]
//  StreamingDloadReadWorker::StreamingDloadReadWorker(StreamingDloadSerial& port, streaming_dload_read_worker_request request, QObject *parent) :
//  ^
// g++  -o streaming_dload hexdump.o endian.o sleep.o hdlc.o crc.o streaming_dload_window.o hdlc_serial.o streaming_dload_serial.o streaming_dload_read_worker.o streaming_dload.o moc_streaming_dload_window.o   -Lopenpst/build/qmake/ -lserial -lQt5Widgets -lQt5Gui -lQt5Core -lGL -lpthread
// streaming_dload_read_worker.o: In function `openpst::StreamingDloadReadWorker::StreamingDloadReadWorker(openpst::StreamingDloadSerial&, openpst::streaming_dload_read_worker_request, QObject*)':
// openpst/build/qmake/../../src/worker/streaming_dload_read_worker.cpp:19: undefined reference to `vtable for openpst::StreamingDloadReadWorker'
// streaming_dload_read_worker.o: In function `openpst::StreamingDloadReadWorker::~StreamingDloadReadWorker()':
// Makefile:174: recipe for target 'streaming_dload' failed
// openpst/build/qmake/../../src/worker/streaming_dload_read_worker.cpp:24: undefined reference to `vtable for openpst::StreamingDloadReadWorker'

// StreamingDloadReadWorker::StreamingDloadReadWorker(StreamingDloadSerial& port, streaming_dload_read_worker_request request, QObject *parent) :
//     port(port),
//     request(request),
//     QThread(parent)
// {
//     cancelled = false;
// }
//
// StreamingDloadReadWorker::~StreamingDloadReadWorker()
// {
//
// }

void StreamingDloadReadWorker::cancel()
{
    cancelled = true;
}
void StreamingDloadReadWorker::run()
{

}

