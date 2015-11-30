/********************************************************************************
** Form generated from reading UI file 'about_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_DIALOG_H
#define UI_ABOUT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QCommandLinkButton *commandLinkButton;
    QCommandLinkButton *commandLinkButton_2;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QStringLiteral("AboutDialog"));
        AboutDialog->resize(432, 348);
        AboutDialog->setModal(true);
        label = new QLabel(AboutDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 111, 31));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label_2 = new QLabel(AboutDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 40, 111, 21));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        label_2->setFont(font1);
        label_3 = new QLabel(AboutDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 100, 131, 31));
        label_3->setFont(font);
        label_4 = new QLabel(AboutDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(310, 10, 111, 21));
        label_4->setFont(font1);
        label_5 = new QLabel(AboutDialog);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(180, 40, 251, 21));
        label_5->setFont(font1);
        label_6 = new QLabel(AboutDialog);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(130, 70, 301, 21));
        label_6->setFont(font1);
        commandLinkButton = new QCommandLinkButton(AboutDialog);
        commandLinkButton->setObjectName(QStringLiteral("commandLinkButton"));
        commandLinkButton->setGeometry(QRect(10, 250, 391, 41));
        commandLinkButton_2 = new QCommandLinkButton(AboutDialog);
        commandLinkButton_2->setObjectName(QStringLiteral("commandLinkButton_2"));
        commandLinkButton_2->setGeometry(QRect(10, 300, 391, 41));

        retranslateUi(AboutDialog);

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "About OpenPST", 0));
        label->setText(QApplication::translate("AboutDialog", "OpenPST", 0));
        label_2->setText(QApplication::translate("AboutDialog", "Version x.xx", 0));
        label_3->setText(QApplication::translate("AboutDialog", "License Info", 0));
        label_4->setText(QApplication::translate("AboutDialog", "Core Authors", 0));
        label_5->setText(QApplication::translate("AboutDialog", "Gassan Idriss ghassani@gmail.com", 0));
        label_6->setText(QApplication::translate("AboutDialog", "Matteson Raab mattesonraab@gmail.com", 0));
        commandLinkButton->setText(QApplication::translate("AboutDialog", "Visit http://www.openpst.io", 0));
        commandLinkButton_2->setText(QApplication::translate("AboutDialog", "Visit on Github", 0));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_DIALOG_H
