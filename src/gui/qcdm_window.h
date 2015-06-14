/**
* LICENSE PLACEHOLDER
*/

#ifndef _GUI_QCDM_WINDOW_H
#define _GUI_QCDM_WINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QFileDialog>
#include "ui_qcdm_window.h"
#include "serial/qcdm_serial.h"


namespace Ui {
class QcdmWindow;
}

class QcdmWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QcdmWindow(QWidget *parent = 0);
    ~QcdmWindow();

public slots:
    void UpdatePortList();
    void ConnectToPort();
    void DisconnectPort();
    void SecuritySendSpc();

private:
    void log(const char* message);
    void log(std::string message);
    void log(QString message);

    Ui::QcdmWindow *ui;
    openpst::QcdmSerial port;
    serial::PortInfo currentPort;



};

#endif // _GUI_QCDM_WINDOW_H
