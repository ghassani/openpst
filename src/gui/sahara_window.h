/**
* LICENSE PLACEHOLDER
*/

#ifndef _GUI_SAHARAWINDOW_H
#define _GUI_SAHARAWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QFileDialog>
#include <iostream>
#include <stdio.h>
#include "ui_sahara_window.h"
#include "serial/sahara_serial.h"
#include "qc/sahara.h"
#include "qc/streaming_dload.h"
#include "qc/hdlc.h"
#include "util/hexdump.h"

namespace Ui {
class SaharaWindow;
}

class SaharaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SaharaWindow(QWidget *parent = 0);
    ~SaharaWindow();

public slots:
    void UpdatePortList();
    void ConnectToPort();
    void DisconnectPort();
    void SayHello();
    void SwitchMode();
    void SendClientCommand();
    void SendReset();
    void ReadSome();
    void BrowseForImage();
    void SendImage();
    void SendCommandButtonAction();
    void SendDone();
    void SendStreamingDloadHello();

private:
    Ui::SaharaWindow *ui;
    openpst::SaharaSerial port;
    serial::PortInfo currentPort;

    void log(const char* message);
    void log(std::string message);
    void log(QString message);
    void logHex(uint8_t* data, size_t amount);
    void logRxHex(uint8_t* data, size_t amount);
    void logTxHex(uint8_t* data, size_t amount);

};

#endif // _GUI_SAHARAWINDOW_H
