/**
* LICENSE PLACEHOLDER
*/

#ifndef _GUI_STREAMING_DLOAD_WINDOW_H
#define _GUI_STREAMING_DLOAD_WINDOW_H

#include <QMainWindow>
#include "ui_streaming_dload_window.h"
#include "qc/streaming_dload.h"
#include "serial/streaming_dload_serial.h"

namespace Ui {
class StreamingDloadWindow;
}

class StreamingDloadWindow : public QMainWindow
{
	

public:
    explicit StreamingDloadWindow(QWidget *parent = 0);
    ~StreamingDloadWindow();
	

public slots:
    /**
     * @brief UpdatePortList
     */
    void UpdatePortList();

    /**
     * @brief ConnectToPort
     */
	void ConnectToPort();

    /**
     * @brief DisconnectPort
     */
	void DisconnectPort();

    /**
     * @brief SendHello
     */
	void SendHello();

    /**
     * @brief ClearLog
     */
	void ClearLog();

    /**
     * @brief SaveLog
     */
	void SaveLog();


	void log(const char* message);
	void log(std::string message);
	void log(QString message);
	void logHex(uint8_t* data, size_t amount);
	void logRxHex(uint8_t* data, size_t amount);
	void logTxHex(uint8_t* data, size_t amount);

private:
	Ui::StreamingDloadWindow *ui;
	openpst::StreamingDloadSerial port;
	serial::PortInfo currentPort;
};

#endif // _GUI_STREAMING_DLOAD_WINDOW_H
