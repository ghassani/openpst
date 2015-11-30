/********************************************************************************
** Form generated from reading UI file 'qcsamunlock_window.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QCSAMUNLOCK_WINDOW_H
#define UI_QCSAMUNLOCK_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QcSamUnlockWindow
{
public:
    QWidget *centralwidget;
    QComboBox *portListComboBox;
    QPushButton *unlockButton;
    QPushButton *clearLogButton;
    QPushButton *saveLogButton;
    QPlainTextEdit *log;
    QPushButton *updatePortListButton;

    void setupUi(QMainWindow *QcSamUnlockWindow)
    {
        if (QcSamUnlockWindow->objectName().isEmpty())
            QcSamUnlockWindow->setObjectName(QStringLiteral("QcSamUnlockWindow"));
        QcSamUnlockWindow->resize(625, 240);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QcSamUnlockWindow->sizePolicy().hasHeightForWidth());
        QcSamUnlockWindow->setSizePolicy(sizePolicy);
        QcSamUnlockWindow->setMinimumSize(QSize(625, 240));
        QcSamUnlockWindow->setMaximumSize(QSize(625, 240));
        QcSamUnlockWindow->setContextMenuPolicy(Qt::DefaultContextMenu);
        QcSamUnlockWindow->setAcceptDrops(false);
        QcSamUnlockWindow->setAutoFillBackground(true);
        QcSamUnlockWindow->setUnifiedTitleAndToolBarOnMac(false);
        centralwidget = new QWidget(QcSamUnlockWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        portListComboBox = new QComboBox(centralwidget);
        portListComboBox->setObjectName(QStringLiteral("portListComboBox"));
        portListComboBox->setGeometry(QRect(10, 10, 471, 27));
        QFont font;
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        portListComboBox->setFont(font);
        unlockButton = new QPushButton(centralwidget);
        unlockButton->setObjectName(QStringLiteral("unlockButton"));
        unlockButton->setGeometry(QRect(550, 9, 61, 29));
        unlockButton->setFont(font);
        clearLogButton = new QPushButton(centralwidget);
        clearLogButton->setObjectName(QStringLiteral("clearLogButton"));
        clearLogButton->setGeometry(QRect(469, 200, 71, 29));
        clearLogButton->setFont(font);
        saveLogButton = new QPushButton(centralwidget);
        saveLogButton->setObjectName(QStringLiteral("saveLogButton"));
        saveLogButton->setGeometry(QRect(540, 200, 71, 29));
        saveLogButton->setFont(font);
        log = new QPlainTextEdit(centralwidget);
        log->setObjectName(QStringLiteral("log"));
        log->setGeometry(QRect(10, 50, 601, 141));
        QFont font1;
        font1.setPointSize(10);
        log->setFont(font1);
        updatePortListButton = new QPushButton(centralwidget);
        updatePortListButton->setObjectName(QStringLiteral("updatePortListButton"));
        updatePortListButton->setGeometry(QRect(489, 9, 61, 29));
        updatePortListButton->setFont(font);
        QcSamUnlockWindow->setCentralWidget(centralwidget);

        retranslateUi(QcSamUnlockWindow);

        QMetaObject::connectSlotsByName(QcSamUnlockWindow);
    } // setupUi

    void retranslateUi(QMainWindow *QcSamUnlockWindow)
    {
        QcSamUnlockWindow->setWindowTitle(QApplication::translate("QcSamUnlockWindow", "QCSamUnlock BETA - OpenPST", 0));
        unlockButton->setText(QApplication::translate("QcSamUnlockWindow", "Unlock", 0));
        clearLogButton->setText(QApplication::translate("QcSamUnlockWindow", "Clear Log", 0));
        saveLogButton->setText(QApplication::translate("QcSamUnlockWindow", "Save Log", 0));
        updatePortListButton->setText(QApplication::translate("QcSamUnlockWindow", "Refresh", 0));
    } // retranslateUi

};

namespace Ui {
    class QcSamUnlockWindow: public Ui_QcSamUnlockWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QCSAMUNLOCK_WINDOW_H
