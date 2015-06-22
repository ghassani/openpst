/**
* LICENSE PLACEHOLDER
*
* @file streaming_dload_window.h
* @class StreamingDloadWindow
* @package OpenPST
* @brief Streaming DLOAD GUI interface class definitions
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#ifndef _GUI_STREAMING_DLOAD_WINDOW_H
#define _GUI_STREAMING_DLOAD_WINDOW_H

#include <QMainWindow>
#include "QFileDialog.h"
#include "ui_streaming_dload_window.h"
#include "qc/streaming_dload.h"
#include "serial/streaming_dload_serial.h"
#include <iostream>
#include <fstream>

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
     * @brief updatePortList
     */
    void updatePortList();

    /**
     * @brief connectToPort
     */
	void connectToPort();

    /**
     * @brief disconnectPort
     */
	void disconnectPort();

    /**
     * @brief sendHello
     */
	void sendHello();

	/**
	* @brief sendUnlock
	*/
	void sendUnlock();

	/**
	* @brief setSecurityMode
	*/
	void setSecurityMode();

	/**
	* @brief sendNop
	*/
	void sendNop();

	/**
	* @brief sendReset
	*/
	void sendReset();

	/**
	* @brief sendPowerDown
	*/
	void sendPowerDown();

	/**
	* @brief openMode
	*/
	void openMode();

	/**
	* @brief closeMode
	*/
	void closeMode();

	/**
	* @brief openMultiMode
	*/
	void openMultiMode();

	/**
	* @brief readEccState
	*/
	void readEccState();

	/**
	* @brief setEccState
	*/
	void setEccState();

	/**
	* @brief read - Read address and size
	*/
	void read();

	/**
	* @brief readQfprom
	*/
	void readQfprom();

	/**
	* @brief eraseFlash
	*/
	void eraseFlash();

	/**
	* @brief clearLog
	*/
	void clearLog();
	
	/**
     * @brief SaveLog
     */
	void saveLog();

	/**
	* @brief browseForPartitionTable
	*/
	void browseForParitionTable();

	/**
	* @brief writePartitionTable
	*/
	void writePartitionTable();

	/**
	* @brief browseForWriteFile
	*/
	void browseForWriteFile();

	/**
	* @brief log
	*/
	void log(const char* message);

	/**
	* @brief SaveLog
	*/
	void log(QString message);

private:
	Ui::StreamingDloadWindow *ui;
	openpst::StreamingDloadSerial port;
	serial::PortInfo currentPort;
};

#endif // _GUI_STREAMING_DLOAD_WINDOW_H
