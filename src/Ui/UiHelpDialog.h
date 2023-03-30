/********************************************************************************
** Form generated from reading UI file 'HelpDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UIHELPDIALOG_H
#define UIHELPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_HelpDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QTextBrowser *textBrowser;

    void setupUi(QDialog *HelpDialog)
    {
        if (HelpDialog->objectName().isEmpty())
            HelpDialog->setObjectName(QString::fromUtf8("HelpDialog"));
        HelpDialog->resize(400, 300);
        horizontalLayout = new QHBoxLayout(HelpDialog);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(HelpDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(100, 100));

        horizontalLayout->addWidget(label);

        textBrowser = new QTextBrowser(HelpDialog);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        horizontalLayout->addWidget(textBrowser);


        retranslateUi(HelpDialog);

        QMetaObject::connectSlotsByName(HelpDialog);
    } // setupUi

    void retranslateUi(QDialog *HelpDialog)
    {
        HelpDialog->setWindowTitle(QCoreApplication::translate("HelpDialog", "Dialog", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class HelpDialog: public Ui_HelpDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UIHELPDIALOG_H
