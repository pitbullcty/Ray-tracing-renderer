/********************************************************************************
** Form generated from reading UI file 'InspectorPage.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UIINSPECTORPAGE_H
#define UIINSPECTORPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InspectorPage
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *pushButtonFold;
    QWidget *widgetContent;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayoutContent;

    void setupUi(QWidget *InspectorPage)
    {
        if (InspectorPage->objectName().isEmpty())
            InspectorPage->setObjectName(QString::fromUtf8("InspectorPage"));
        InspectorPage->resize(237, 278);
        verticalLayout = new QVBoxLayout(InspectorPage);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButtonFold = new QPushButton(InspectorPage);
        pushButtonFold->setObjectName(QString::fromUtf8("pushButtonFold"));
        pushButtonFold->setMinimumSize(QSize(0, 24));
        pushButtonFold->setCheckable(false);
        pushButtonFold->setFlat(true);

        verticalLayout->addWidget(pushButtonFold);

        widgetContent = new QWidget(InspectorPage);
        widgetContent->setObjectName(QString::fromUtf8("widgetContent"));
        verticalLayout_3 = new QVBoxLayout(widgetContent);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayoutContent = new QVBoxLayout();
        verticalLayoutContent->setObjectName(QString::fromUtf8("verticalLayoutContent"));

        verticalLayout_3->addLayout(verticalLayoutContent);


        verticalLayout->addWidget(widgetContent);


        retranslateUi(InspectorPage);

        QMetaObject::connectSlotsByName(InspectorPage);
    } // setupUi

    void retranslateUi(QWidget *InspectorPage)
    {
        InspectorPage->setWindowTitle(QCoreApplication::translate("InspectorPage", "Form", nullptr));
        pushButtonFold->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class InspectorPage: public Ui_InspectorPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UIINSPECTORPAGE_H
