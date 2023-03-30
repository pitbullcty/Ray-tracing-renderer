/********************************************************************************
** Form generated from reading UI file 'SkyboxInspector.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UISKYBOXINSPECTOR_H
#define UISKYBOXINSPECTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SkyboxInspector
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QWidget *skyboxwidget;
    QGridLayout *gridLayout_2;
    QLabel *labelNZ;
    QToolButton *toolButtonNY;
    QLabel *labelNX;
    QLabel *label_3;
    QLabel *labelPY;
    QLabel *labelNY;
    QLabel *label_5;
    QToolButton *toolButtonPZ;
    QLabel *label_6;
    QToolButton *toolButtonNX;
    QToolButton *toolButtonPX;
    QToolButton *toolButtonPY;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButtonReset;
    QLabel *label_4;
    QLabel *labelPZ;
    QLabel *labelPX;
    QToolButton *toolButtonNZ;
    QPushButton *pushButtonEditAll;

    void setupUi(QWidget *SkyboxInspector)
    {
        if (SkyboxInspector->objectName().isEmpty())
            SkyboxInspector->setObjectName(QString::fromUtf8("SkyboxInspector"));
        SkyboxInspector->resize(400, 361);
        gridLayout = new QGridLayout(SkyboxInspector);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        checkBox = new QCheckBox(SkyboxInspector);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setChecked(true);

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        skyboxwidget = new QWidget(SkyboxInspector);
        skyboxwidget->setObjectName(QString::fromUtf8("skyboxwidget"));
        gridLayout_2 = new QGridLayout(skyboxwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        labelNZ = new QLabel(skyboxwidget);
        labelNZ->setObjectName(QString::fromUtf8("labelNZ"));

        gridLayout_2->addWidget(labelNZ, 6, 1, 1, 1);

        toolButtonNY = new QToolButton(skyboxwidget);
        toolButtonNY->setObjectName(QString::fromUtf8("toolButtonNY"));

        gridLayout_2->addWidget(toolButtonNY, 4, 2, 1, 1);

        labelNX = new QLabel(skyboxwidget);
        labelNX->setObjectName(QString::fromUtf8("labelNX"));

        gridLayout_2->addWidget(labelNX, 2, 1, 1, 1);

        label_3 = new QLabel(skyboxwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 3, 0, 1, 1);

        labelPY = new QLabel(skyboxwidget);
        labelPY->setObjectName(QString::fromUtf8("labelPY"));

        gridLayout_2->addWidget(labelPY, 3, 1, 1, 1);

        labelNY = new QLabel(skyboxwidget);
        labelNY->setObjectName(QString::fromUtf8("labelNY"));

        gridLayout_2->addWidget(labelNY, 4, 1, 1, 1);

        label_5 = new QLabel(skyboxwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 5, 0, 1, 1);

        toolButtonPZ = new QToolButton(skyboxwidget);
        toolButtonPZ->setObjectName(QString::fromUtf8("toolButtonPZ"));

        gridLayout_2->addWidget(toolButtonPZ, 5, 2, 1, 1);

        label_6 = new QLabel(skyboxwidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 6, 0, 1, 1);

        toolButtonNX = new QToolButton(skyboxwidget);
        toolButtonNX->setObjectName(QString::fromUtf8("toolButtonNX"));

        gridLayout_2->addWidget(toolButtonNX, 2, 2, 1, 1);

        toolButtonPX = new QToolButton(skyboxwidget);
        toolButtonPX->setObjectName(QString::fromUtf8("toolButtonPX"));

        gridLayout_2->addWidget(toolButtonPX, 1, 2, 1, 1);

        toolButtonPY = new QToolButton(skyboxwidget);
        toolButtonPY->setObjectName(QString::fromUtf8("toolButtonPY"));

        gridLayout_2->addWidget(toolButtonPY, 3, 2, 1, 1);

        label = new QLabel(skyboxwidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 1, 0, 1, 1);

        label_2 = new QLabel(skyboxwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 2, 0, 1, 1);

        pushButtonReset = new QPushButton(skyboxwidget);
        pushButtonReset->setObjectName(QString::fromUtf8("pushButtonReset"));

        gridLayout_2->addWidget(pushButtonReset, 7, 0, 1, 1);

        label_4 = new QLabel(skyboxwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 4, 0, 1, 1);

        labelPZ = new QLabel(skyboxwidget);
        labelPZ->setObjectName(QString::fromUtf8("labelPZ"));

        gridLayout_2->addWidget(labelPZ, 5, 1, 1, 1);

        labelPX = new QLabel(skyboxwidget);
        labelPX->setObjectName(QString::fromUtf8("labelPX"));

        gridLayout_2->addWidget(labelPX, 1, 1, 1, 1);

        toolButtonNZ = new QToolButton(skyboxwidget);
        toolButtonNZ->setObjectName(QString::fromUtf8("toolButtonNZ"));

        gridLayout_2->addWidget(toolButtonNZ, 6, 2, 1, 1);

        pushButtonEditAll = new QPushButton(skyboxwidget);
        pushButtonEditAll->setObjectName(QString::fromUtf8("pushButtonEditAll"));

        gridLayout_2->addWidget(pushButtonEditAll, 8, 0, 1, 1);


        gridLayout->addWidget(skyboxwidget, 1, 0, 1, 1);


        retranslateUi(SkyboxInspector);

        QMetaObject::connectSlotsByName(SkyboxInspector);
    } // setupUi

    void retranslateUi(QWidget *SkyboxInspector)
    {
        SkyboxInspector->setWindowTitle(QCoreApplication::translate("SkyboxInspector", "Form", nullptr));
#if QT_CONFIG(tooltip)
        checkBox->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        checkBox->setText(QCoreApplication::translate("SkyboxInspector", "\345\274\200\345\220\257\345\244\251\347\251\272\347\233\222", nullptr));
        labelNZ->setText(QString());
        toolButtonNY->setText(QCoreApplication::translate("SkyboxInspector", "...", nullptr));
        labelNX->setText(QString());
#if QT_CONFIG(tooltip)
        label_3->setToolTip(QCoreApplication::translate("SkyboxInspector", "\345\244\251\347\251\272\347\233\222\351\241\266\351\203\250\357\274\214\346\202\254\346\265\256\345\217\263\344\276\247\346\240\207\347\255\276\345\217\257\346\237\245\347\234\213\345\233\276\347\211\207", nullptr));
#endif // QT_CONFIG(tooltip)
        label_3->setText(QCoreApplication::translate("SkyboxInspector", "+y:", nullptr));
        labelPY->setText(QString());
        labelNY->setText(QString());
#if QT_CONFIG(tooltip)
        label_5->setToolTip(QCoreApplication::translate("SkyboxInspector", "\345\244\251\347\251\272\347\233\222\345\217\263\344\276\247\357\274\214\346\202\254\346\265\256\345\217\263\344\276\247\346\240\207\347\255\276\345\217\257\346\237\245\347\234\213\345\233\276\347\211\207", nullptr));
#endif // QT_CONFIG(tooltip)
        label_5->setText(QCoreApplication::translate("SkyboxInspector", "+z:", nullptr));
        toolButtonPZ->setText(QCoreApplication::translate("SkyboxInspector", "...", nullptr));
#if QT_CONFIG(tooltip)
        label_6->setToolTip(QCoreApplication::translate("SkyboxInspector", "\345\244\251\347\251\272\347\233\222\345\267\246\344\276\247\357\274\214\346\202\254\346\265\256\345\217\263\344\276\247\346\240\207\347\255\276\345\217\257\346\237\245\347\234\213\345\233\276\347\211\207", nullptr));
#endif // QT_CONFIG(tooltip)
        label_6->setText(QCoreApplication::translate("SkyboxInspector", "-z:", nullptr));
        toolButtonNX->setText(QCoreApplication::translate("SkyboxInspector", "...", nullptr));
        toolButtonPX->setText(QCoreApplication::translate("SkyboxInspector", "...", nullptr));
        toolButtonPY->setText(QCoreApplication::translate("SkyboxInspector", "...", nullptr));
#if QT_CONFIG(tooltip)
        label->setToolTip(QCoreApplication::translate("SkyboxInspector", "\345\244\251\347\251\272\347\233\222\346\255\243\351\235\242\357\274\214\346\202\254\346\265\256\345\217\263\344\276\247\346\240\207\347\255\276\345\217\257\346\237\245\347\234\213\345\233\276\347\211\207", nullptr));
#endif // QT_CONFIG(tooltip)
        label->setText(QCoreApplication::translate("SkyboxInspector", "+x:", nullptr));
#if QT_CONFIG(tooltip)
        label_2->setToolTip(QCoreApplication::translate("SkyboxInspector", "\345\244\251\347\251\272\347\233\222\350\203\214\351\235\242\357\274\214\346\202\254\346\265\256\345\217\263\344\276\247\346\240\207\347\255\276\345\217\257\346\237\245\347\234\213\345\233\276\347\211\207", nullptr));
#endif // QT_CONFIG(tooltip)
        label_2->setText(QCoreApplication::translate("SkyboxInspector", "-x:", nullptr));
        pushButtonReset->setText(QCoreApplication::translate("SkyboxInspector", "\351\207\215\347\275\256", nullptr));
#if QT_CONFIG(tooltip)
        label_4->setToolTip(QCoreApplication::translate("SkyboxInspector", "\345\244\251\347\251\272\347\233\222\345\272\225\351\203\250\357\274\214\346\202\254\346\265\256\345\217\263\344\276\247\346\240\207\347\255\276\345\217\257\346\237\245\347\234\213\345\233\276\347\211\207", nullptr));
#endif // QT_CONFIG(tooltip)
        label_4->setText(QCoreApplication::translate("SkyboxInspector", "-y:", nullptr));
        labelPZ->setText(QString());
        labelPX->setText(QString());
        toolButtonNZ->setText(QCoreApplication::translate("SkyboxInspector", "...", nullptr));
        pushButtonEditAll->setText(QCoreApplication::translate("SkyboxInspector", "\346\211\271\351\207\217\344\277\256\346\224\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SkyboxInspector: public Ui_SkyboxInspector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UISKYBOXINSPECTOR_H
