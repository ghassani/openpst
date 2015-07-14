/**
* LICENSE PLACEHOLDER
*
* @file qcdm_window.h
* @class QcdmWindow
* @package OpenPST
* @brief QCDM GUI interface class definitions
*
* @author Gassan Idriss <ghassani@gmail.com>
* @author Matteson Raab <mraabhimself@gmail.com>
*/

#ifndef _GUI_QCDM_WINDOW_H
#define _GUI_QCDM_WINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QFileDialog>
#include "ui_qcdm_window.h"
#include "qc/diag_nv.h"
#include "serial/qcdm_serial.h"
#include "util/convert.h"


namespace Ui {
    class QcdmWindow;
}

namespace openpst {
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

        void DisableUiButtons();
        void EnableUiButtons();

        void SendSpc();
        void SendPassword();

        void ReadImei();
        void ReadMeid();
        void WriteMeid();
        void ReadNam();
        void WriteNam();
        void ReadNvItem();
        void ReadSpc();
        void WriteSpc();
        void ReadSubscription();
        void WriteSubscription();

        void SendPhoneMode();

        void SpcTextChanged(QString value);

        void ClearLog();
        void SaveLog();

    private:
        void ReadMdn();
        void WriteMdn();
        void ReadMin();
        void ReadSid();
        void ReadSystemPref();
        void WriteSystemPref();
        void ReadPrefMode();
        void ReadPrefServ();
        void WritePrefServ();
        void ReadRoamPref();
        void WriteRoamPref();

        void ReadPapUserId();
        void ReadPapPassword();
        void ReadPppUserId();
        void ReadPppPassword();
        void ReadHdrAnUserId();
        void ReadHdrAnPassword();
        void ReadHdrAnLongUserId();
        void ReadHdrAnLongPassword();
        void ReadHdrAnPppUserId();
        void ReadHdrAnPppPassword();

        QString FixedEmptyTrim(QString input);

        void log(int type, const char* message);
        void log(int type, std::string message);
        void log(int type, QString message);

        Ui::QcdmWindow *ui;
        openpst::QcdmSerial port;
        serial::PortInfo currentPort;

        int LOGTYPE_DEBUG = 0;
        int LOGTYPE_ERROR = -1;
        int LOGTYPE_INFO = 1;
        int LOGTYPE_WARNING = 2;

    };
}
#endif // _GUI_QCDM_WINDOW_H
