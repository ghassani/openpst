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
#include <QMessageBox>
#include <QInputDialog>
#include <QClipboard>
#include "ui_qcdm_window.h"
#include "about_dialog.h"
#include "qc/dm.h"
#include "qc/dm_nv.h"
#include "qc/dm_efs.h"
#include "qc/dm_efs_manager.h"
#include "serial/qcdm_serial.h"
#include "worker/qcdm_efs_directory_tree_worker.h"
#include "worker/qcdm_efs_file_read_worker.h"
#include "worker/qcdm_efs_file_write_worker.h"
#include "worker/qcdm_memory_read_worker.h"
#include "worker/qcdm_prl_write_worker.h"
#include "worker/qcdm_prl_read_worker.h"

#include "util/convert.h"

namespace Ui {
	class QcdmWindow;

}

namespace OpenPST {

	class QcdmWindow : public QMainWindow
	{
		Q_OBJECT

		enum LogType {
			kLogTypeError = 1,
			kLogTypeWarning = 2,
			kLogTypeInfo = 3,
			kLogTypeDebug = 4
		};

		enum EfsBrowserColumns {
			kEfsBrowserColumnName = 0,
			kEfsBrowserColumnType,
			kEfsBrowserColumnSize,
			kEfsBrowserColumnMode,
			kEfsBrowserColumnATime,
			kEfsBrowserColumnMTime,
			kEfsBrowserColumnCTime,
			kEfsBrowserColumnFullPath
		};

		std::vector<QThread*> workers;

	public:
		Ui::QcdmWindow *ui;
		OpenPST::QcdmSerial port;
		serial::PortInfo currentPort;
		OpenPST::DmEfsManager efsManager;
		AboutDialog *aboutDialog;
		
		/**
		* @brief
		*/
		explicit QcdmWindow(QWidget *parent = 0);

		/**
		* @brief
		*/
		~QcdmWindow();

	public slots:
		/**
		* @brief
		*/
		void UpdatePortList();

		/**
		* @brief
		*/
		void ConnectToPort();

		/**
		* @brief
		*/
		void DisconnectPort();

		/**
		* @brief
		*/
		void DisableUiButtons();

		/**
		* @brief
		*/
		void EnableUiButtons();

		/**
		* @brief
		*/
		void SendSpc();

		/**
		* @brief
		*/
		void SendPassword();

		/**
		* @brief
		*/
		void ReadImei();

		/**
		* @brief
		*/
		void ReadMeid();

		/**
		* @brief
		*/
		void WriteMeid();

		/**
		* @brief
		*/
		void ReadNam();

		/**
		* @brief
		*/
		void WriteNam();

		/**
		* @brief
		*/
		void ReadNvItem();

		/**
		* @brief
		*/
		void ReadSpc();

		/**
		* @brief
		*/
		void WriteSpc();

		/**
		* @brief
		*/
		void ReadSubscription();

		/**
		* @brief
		*/
		void WriteSubscription();

		/**
		* @brief
		*/
		void SendPhoneMode();

		/**
		* @brief
		*/
		void SpcTextChanged(QString value);

		/**
		* @brief
		*/
		void ProbeCommands();

		/**
		* @brief
		*/
		void EfsHello();

		/**
		* @brief
		*/
		void EfsGetDeviceInfo();

		/**
		* @brief
		*/
		void EfsListDirectories();

		/**
		* @brief
		*/
		void EfsQuery();

		/**
		* @brief
		*/
		void EfsExtractFactoryImage();

		/**
		* @brief
		*/
		void EfsMakeGoldenCopy();

		/**
		* @brief
		*/
		void EfsFilesystemImage();

		/**
		* @brief
		*/
		void EfsGetFileInfo();

		/**
		* @brief
		*/
		void EfsGetFile();

		/**
		* @brief
		*/
		void EfsDeleteFile();

		/**
		* @brief
		*/
		void onEfsTreeNodeRightClick(const QPoint& point);

		/**
		* @brief
		*/
		void onEfsSubsysCommandCodeChange();

		/**
		* @brief
		*/
		void onEfsSubsysIdChange();

		/**
		* @brief
		*/
		void EfsReadAll();

		/**
		* @brief
		*/
		void EfsContextMenuSaveFile();

		/**
		* @brief
		*/
		void EfsContextMenuSaveDirectory();

		/**
		* @brief
		*/
		void EfsContextMenuSaveDirectoryCompressed();

		/**
		* @brief
		*/
		void EfsContextMenuUploadFile();

		/**
		* @brief
		*/
		void EfsContextMenuCopyPathToClipboard();

		/**
		* @brief
		*/
		void EfsContextMenuDeleteDirectory();

		/**
		* @brief
		*/
		void EfsContextMenuDeleteFile();

		/**
		* @brief
		*/
		void EfsContextMenuCreateDirectory();


		/**
		* @brief
		*/
		void EfsContextMenuCreateLink();

		/**
		* @brief
		*/
		void ClearLog();

		/**
		* @brief
		*/
		void SaveLog();

		/**
		* @brief
		*/
		void About();

		void ReadSome();

		void SendCommand();

	private:
		/**
		* @brief
		*/
		void ReadMdn();

		/**
		* @brief
		*/
		void WriteMdn();

		/**
		* @brief
		*/
		void ReadMin();

		/**
		* @brief
		*/
		void ReadSid();

		/**
		* @brief
		*/
		void ReadSystemPref();

		/**
		* @brief
		*/
		void WriteSystemPref();

		/**
		* @brief
		*/
		void ReadPrefMode();

		/**
		* @brief
		*/
		void ReadPrefServ();

		/**
		* @brief
		*/
		void WritePrefServ();

		/**
		* @brief
		*/
		void ReadRoamPref();

		/**
		* @brief
		*/
		void WriteRoamPref();


		/**
		* @brief
		*/
		void ReadPapUserId();

		/**
		* @brief
		*/
		void ReadPapPassword();

		/**
		* @brief
		*/
		void ReadPppUserId();

		/**
		* @brief
		*/
		void ReadPppPassword();

		/**
		* @brief
		*/
		void ReadHdrAnUserId();

		/**
		* @brief
		*/
		void ReadHdrAnPassword();

		/**
		* @brief
		*/
		void ReadHdrAnLongUserId();

		/**
		* @brief
		*/
		void ReadHdrAnLongPassword();

		/**
		* @brief
		*/
		void ReadHdrAnPppUserId();

		/**
		* @brief
		*/
		void ReadHdrAnPppPassword();


		/**
		* @brief
		*/
		void EfsAddTreeNodes(QTreeWidgetItem* parent, std::vector<DmEfsNode>& entries);



		/**
		* @brief
		*/
		QString FixedEmptyTrim(QString input);


		/**
		* @brief
		*/
		void log(const char* message);

		/**
		* @brief
		*/
		void log(int type, const char* message);

		/**
		* @brief
		*/
		void log(int type, std::string message);

		/**
		* @brief
		*/
		void log(int type, QString message);





	};

}
#endif // _GUI_QCDM_WINDOW_H
