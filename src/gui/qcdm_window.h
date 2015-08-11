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
#include "about_dialog.h"
#include "qc/dm.h"
#include "qc/dm_nv.h"
#include "qc/dm_efs.h"
#include "qc/dm_efs_manager.h"
#include "serial/qcdm_serial.h"
#include "util/convert.h"

namespace Ui {
    class QcdmWindow;
}

namespace OpenPST {
	
	enum LOGTYPE {
		LOGTYPE_DEBUG = 0,
		LOGTYPE_ERROR = -1,
		LOGTYPE_INFO = 1,
		LOGTYPE_WARNING = 2
	};

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
			void ProbeSubsys();
			void EfsHello();
			void EfsGetDeviceInfo();
			void EfsListDirectories();
			void EfsQuery();
			void EfsExtractFactoryImage();
			void EfsMakeGoldenCopy();
			void EfsReadAll();
			void ClearLog();
			void SaveLog();

			void About();


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

		void EfsAddTreeNodes(QTreeWidgetItem* parent, std::vector<DmEfsNode>& entries);


        QString FixedEmptyTrim(QString input);

        void log(int type, const char* message);
        void log(int type, std::string message);
        void log(int type, QString message);

        Ui::QcdmWindow *ui;
        OpenPST::QcdmSerial port;
        serial::PortInfo currentPort;
		OpenPST::DmEfsManager efsManager;
		AboutDialog *aboutDialog;


    };
}
#endif // _GUI_QCDM_WINDOW_H
