/**
* LICENSE PLACEHOLDER
*/

#ifndef _GUI_MAINWINDOW_H
#define _GUI_MAINWINDOW_H

#include <QMainWindow>
#include "ui_main_window.h"
#include "sahara_window.h"
#include "send_raw_window.h"
#include "mbn_tool_window.h"
#include "qcdm_window.h"
#include "streaming_dload_window.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void startSaharaMode();
    void startPortRawWriteRead();
    void startMbnTool();
	void startQcdm();
	void startStreamingDload();

private:
    SaharaWindow		 *saharaWindow			   = NULL;
	SendRawWindow		 *sendRawWindow			   = NULL;
	MbnToolWindow		 *mbnToolWindow			   = NULL;
	QcdmWindow			 *qcdmWindow			   = NULL;
	StreamingDloadWindow *streamingDloadWindow     = NULL;

    Ui::MainWindow *ui;
};

#endif // _GUI_MAINWINDOW_H
