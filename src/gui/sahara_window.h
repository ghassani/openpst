#ifndef SAHARAWINDOW_H
#define SAHARAWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include "ui_sahara_window.h"
#include "serial/sahara_serial.h"
#include "helper/sahara_helper.h"
#include "qc_sahara.h"
#include "util.h"
#include <QFileDialog>

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

private:
    Ui::SaharaWindow *ui;
    openpst::SaharaSerial port;
    serial::PortInfo currentPort;
    uint8_t* readBuffer;
    size_t readBufferSize = 1024;

    sahara_hello_rx_t deviceState;
    sahara_read_data_rx_t requestedImage;

    void log(const char* message);
    void log(std::string message);
    void log(QString message);
    void logHex(uint8_t* data, size_t amount);
    void logRxHex(uint8_t* data, size_t amount);
    void logTxHex(uint8_t* data, size_t amount);

};

#endif // SAHARAWINDOW_H
