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
	
	Q_OBJECT

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
	* @brief SendUnlock
	*/
	void SendUnlock();

	/**
	* @brief SetSecurityMode
	*/
	void SetSecurityMode();

	/**
	* @brief SendNop
	*/
	void SendNop();

	/**
	* @brief SendReset
	*/
	void SendReset();

	/**
	* @brief SendPowerDown
	*/
	void SendPowerDown();

	/**
	* @brief CloseMode
	*/
	void CloseMode();

	/**
	* @brief ReadEccState
	*/
	void ReadEccState();

	/**
	* @brief SetEccState
	*/
	void SetEccState();

	/**
	* @brief ClearLog
	*/
	void ClearLog();
	
	/**
     * @brief SaveLog
     */
	void SaveLog();


	void log(const char* message);
	void log(QString message);

private:
	Ui::StreamingDloadWindow *ui;
	openpst::StreamingDloadSerial port;
	serial::PortInfo currentPort;
};

#endif // _GUI_STREAMING_DLOAD_WINDOW_H
