/********************************************************************************
** Form generated from reading UI file 'RenderOptionInspector.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UIRENDEROPTIONINSPECTOR_H
#define UIRENDEROPTIONINSPECTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RenderOptionInspector
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QLabel *labelDepth;
    QToolButton *toolButton;
    QLabel *label_6;
    QLabel *labelMaxFrameCount;
    QWidget *denoiserCount;
    QHBoxLayout *horizontalLayout;
    QLabel *label_5;
    QLabel *labelDenoiserStep;
    QSlider *horizontalSliderDenoiserStep;
    QComboBox *comboBoxResolution;
    QLabel *label_10;
    QSlider *horizontalSliderDepth;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton;
    QLabel *label_3;
    QLabel *label_2;
    QSlider *horizontalSliderMaxFrameCount;
    QLabel *label_11;
    QComboBox *comboBoxQuality;
    QLabel *label_4;
    QLabel *labelOutputPath;
    QPushButton *pushButtonReset;

    void setupUi(QWidget *RenderOptionInspector)
    {
        if (RenderOptionInspector->objectName().isEmpty())
            RenderOptionInspector->setObjectName(QString::fromUtf8("RenderOptionInspector"));
        RenderOptionInspector->resize(465, 336);
        gridLayout = new QGridLayout(RenderOptionInspector);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        checkBox = new QCheckBox(RenderOptionInspector);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout->addWidget(checkBox, 2, 3, 1, 1);

        labelDepth = new QLabel(RenderOptionInspector);
        labelDepth->setObjectName(QString::fromUtf8("labelDepth"));

        gridLayout->addWidget(labelDepth, 1, 2, 1, 1);

        toolButton = new QToolButton(RenderOptionInspector);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));

        gridLayout->addWidget(toolButton, 6, 3, 1, 1);

        label_6 = new QLabel(RenderOptionInspector);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 6, 0, 1, 1);

        labelMaxFrameCount = new QLabel(RenderOptionInspector);
        labelMaxFrameCount->setObjectName(QString::fromUtf8("labelMaxFrameCount"));

        gridLayout->addWidget(labelMaxFrameCount, 0, 2, 1, 1);

        denoiserCount = new QWidget(RenderOptionInspector);
        denoiserCount->setObjectName(QString::fromUtf8("denoiserCount"));
        horizontalLayout = new QHBoxLayout(denoiserCount);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_5 = new QLabel(denoiserCount);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout->addWidget(label_5);

        labelDenoiserStep = new QLabel(denoiserCount);
        labelDenoiserStep->setObjectName(QString::fromUtf8("labelDenoiserStep"));
        labelDenoiserStep->setOpenExternalLinks(false);

        horizontalLayout->addWidget(labelDenoiserStep);

        horizontalSliderDenoiserStep = new QSlider(denoiserCount);
        horizontalSliderDenoiserStep->setObjectName(QString::fromUtf8("horizontalSliderDenoiserStep"));
        horizontalSliderDenoiserStep->setMinimum(20);
        horizontalSliderDenoiserStep->setMaximum(100);
        horizontalSliderDenoiserStep->setValue(50);
        horizontalSliderDenoiserStep->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSliderDenoiserStep);


        gridLayout->addWidget(denoiserCount, 3, 0, 1, 4);

        comboBoxResolution = new QComboBox(RenderOptionInspector);
        comboBoxResolution->setObjectName(QString::fromUtf8("comboBoxResolution"));
        comboBoxResolution->setEditable(false);

        gridLayout->addWidget(comboBoxResolution, 4, 3, 1, 1);

        label_10 = new QLabel(RenderOptionInspector);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 4, 0, 1, 1);

        horizontalSliderDepth = new QSlider(RenderOptionInspector);
        horizontalSliderDepth->setObjectName(QString::fromUtf8("horizontalSliderDepth"));
        horizontalSliderDepth->setMaximum(20);
        horizontalSliderDepth->setValue(2);
        horizontalSliderDepth->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSliderDepth, 1, 3, 1, 1);

        widget = new QWidget(RenderOptionInspector);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMaximumSize(QSize(150, 16777215));

        horizontalLayout_2->addWidget(pushButton);


        gridLayout->addWidget(widget, 9, 0, 1, 4);

        label_3 = new QLabel(RenderOptionInspector);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        label_2 = new QLabel(RenderOptionInspector);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        horizontalSliderMaxFrameCount = new QSlider(RenderOptionInspector);
        horizontalSliderMaxFrameCount->setObjectName(QString::fromUtf8("horizontalSliderMaxFrameCount"));
        horizontalSliderMaxFrameCount->setMinimum(100);
        horizontalSliderMaxFrameCount->setMaximum(1000);
        horizontalSliderMaxFrameCount->setValue(200);
        horizontalSliderMaxFrameCount->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSliderMaxFrameCount, 0, 3, 1, 1);

        label_11 = new QLabel(RenderOptionInspector);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 5, 0, 1, 1);

        comboBoxQuality = new QComboBox(RenderOptionInspector);
        comboBoxQuality->setObjectName(QString::fromUtf8("comboBoxQuality"));
        comboBoxQuality->setEditable(false);

        gridLayout->addWidget(comboBoxQuality, 5, 3, 1, 1);

        label_4 = new QLabel(RenderOptionInspector);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        labelOutputPath = new QLabel(RenderOptionInspector);
        labelOutputPath->setObjectName(QString::fromUtf8("labelOutputPath"));
        labelOutputPath->setOpenExternalLinks(true);

        gridLayout->addWidget(labelOutputPath, 6, 2, 1, 1);

        pushButtonReset = new QPushButton(RenderOptionInspector);
        pushButtonReset->setObjectName(QString::fromUtf8("pushButtonReset"));

        gridLayout->addWidget(pushButtonReset, 8, 0, 1, 1);


        retranslateUi(RenderOptionInspector);

        comboBoxResolution->setCurrentIndex(-1);
        comboBoxQuality->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(RenderOptionInspector);
    } // setupUi

    void retranslateUi(QWidget *RenderOptionInspector)
    {
        RenderOptionInspector->setWindowTitle(QCoreApplication::translate("RenderOptionInspector", "Form", nullptr));
        checkBox->setText(QCoreApplication::translate("RenderOptionInspector", "\345\274\200\345\220\257", nullptr));
        labelDepth->setText(QCoreApplication::translate("RenderOptionInspector", "2", nullptr));
        toolButton->setText(QCoreApplication::translate("RenderOptionInspector", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
#if QT_CONFIG(tooltip)
        label_6->setToolTip(QCoreApplication::translate("RenderOptionInspector", "\346\270\262\346\237\223\347\273\223\346\236\234\347\232\204\344\277\235\345\255\230\347\233\256\345\275\225", nullptr));
#endif // QT_CONFIG(tooltip)
        label_6->setText(QCoreApplication::translate("RenderOptionInspector", "\350\276\223\345\207\272\346\226\207\344\273\266\345\220\215", nullptr));
        labelMaxFrameCount->setText(QCoreApplication::translate("RenderOptionInspector", "200", nullptr));
#if QT_CONFIG(tooltip)
        label_5->setToolTip(QCoreApplication::translate("RenderOptionInspector", "<html><head/><body><p>\351\231\215\345\231\252\347\232\204\351\207\207\346\240\267\351\227\264\351\232\224(\346\225\260\345\200\274\350\266\212\344\275\216\346\270\262\346\237\223\346\227\266\351\227\264\350\266\212\351\225\277\357\274\214\346\225\210\346\236\234\346\233\264\345\245\275)</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        label_5->setText(QCoreApplication::translate("RenderOptionInspector", "\351\231\215\345\231\252\345\270\247\351\227\264\351\232\224", nullptr));
        labelDenoiserStep->setText(QCoreApplication::translate("RenderOptionInspector", "20", nullptr));
        comboBoxResolution->setCurrentText(QString());
#if QT_CONFIG(statustip)
        label_10->setStatusTip(QCoreApplication::translate("RenderOptionInspector", "\346\270\262\346\237\223\347\273\223\346\236\234\347\232\204\345\210\206\350\276\250\347\216\207", nullptr));
#endif // QT_CONFIG(statustip)
        label_10->setText(QCoreApplication::translate("RenderOptionInspector", "\346\270\262\346\237\223\345\210\206\350\276\250\347\216\207", nullptr));
        pushButton->setText(QCoreApplication::translate("RenderOptionInspector", "\347\246\273\345\261\217\346\270\262\346\237\223", nullptr));
#if QT_CONFIG(tooltip)
        label_3->setToolTip(QCoreApplication::translate("RenderOptionInspector", "<html><head/><body><p>\345\205\211\347\272\277\350\277\275\350\270\252\346\267\261\345\272\246\357\274\214\345\257\271\346\257\217\346\235\241\345\205\211\347\272\277\345\217\215\345\260\204\350\277\275\350\270\252\347\232\204\346\254\241\346\225\260\357\274\210<span style=\" font-weight:700;\">\346\225\260\345\200\274\350\266\212\345\244\247\345\257\271\346\200\247\350\203\275\350\246\201\346\261\202\350\266\212\351\253\230</span>\357\274\211</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        label_3->setText(QCoreApplication::translate("RenderOptionInspector", "\350\277\275\350\270\252\346\267\261\345\272\246", nullptr));
#if QT_CONFIG(tooltip)
        label_2->setToolTip(QCoreApplication::translate("RenderOptionInspector", "\351\207\207\346\240\267\346\200\273\346\254\241\346\225\260\357\274\214\346\234\200\345\244\247\345\200\274\344\270\272\346\227\240\351\231\220", nullptr));
#endif // QT_CONFIG(tooltip)
        label_2->setText(QCoreApplication::translate("RenderOptionInspector", "\351\207\207\346\240\267\346\254\241\346\225\260", nullptr));
#if QT_CONFIG(statustip)
        label_11->setStatusTip(QCoreApplication::translate("RenderOptionInspector", "\346\270\262\346\237\223\345\233\276\347\211\207\347\232\204\344\277\235\345\255\230\350\264\250\351\207\217", nullptr));
#endif // QT_CONFIG(statustip)
        label_11->setText(QCoreApplication::translate("RenderOptionInspector", "\345\233\276\347\211\207\350\264\250\351\207\217", nullptr));
        comboBoxQuality->setCurrentText(QString());
#if QT_CONFIG(tooltip)
        label_4->setToolTip(QCoreApplication::translate("RenderOptionInspector", "<html><head/><body><p>\346\270\262\346\237\223\350\277\207\347\250\213\344\270\255\345\256\236\346\227\266\351\231\215\345\231\252\357\274\210<span style=\" font-weight:700;\">\345\257\271\346\200\247\350\203\275\350\246\201\346\261\202\350\276\203\351\253\230</span>\357\274\211</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        label_4->setText(QCoreApplication::translate("RenderOptionInspector", "\345\256\236\346\227\266\351\231\215\345\231\252", nullptr));
        labelOutputPath->setText(QString());
        pushButtonReset->setText(QCoreApplication::translate("RenderOptionInspector", "\351\207\215\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RenderOptionInspector: public Ui_RenderOptionInspector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UIRENDEROPTIONINSPECTOR_H
