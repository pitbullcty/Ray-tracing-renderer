/********************************************************************************
** Form generated from reading UI file 'TransformInspector.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UITRANSFORMINSPECTOR_H
#define UITRANSFORMINSPECTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TransformInspector
{
public:
    QGridLayout *gridLayout;
    QLabel *label_9;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBoxPosX;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBoxRotationY;
    QLabel *label_6;
    QDoubleSpinBox *doubleSpinBoxPosY;
    QLabel *label_7;
    QSpacerItem *horizontalSpacer_2;
    QDoubleSpinBox *doubleSpinBoxPosZ;
    QLabel *label_2;
    QLabel *label_10;
    QSpacerItem *horizontalSpacer;
    QLabel *label_11;
    QLabel *label_5;
    QLabel *label_12;
    QDoubleSpinBox *doubleSpinBoxRotationZ;
    QLabel *label_8;
    QDoubleSpinBox *doubleSpinBoxRotationX;
    QLabel *label;
    QSpacerItem *horizontalSpacer_3;
    QDoubleSpinBox *doubleSpinBoxScaleX;
    QDoubleSpinBox *doubleSpinBoxScaleY;
    QDoubleSpinBox *doubleSpinBoxScaleZ;

    void setupUi(QWidget *TransformInspector)
    {
        if (TransformInspector->objectName().isEmpty())
            TransformInspector->setObjectName(QString::fromUtf8("TransformInspector"));
        TransformInspector->resize(558, 346);
        gridLayout = new QGridLayout(TransformInspector);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_9 = new QLabel(TransformInspector);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 9, 0, 1, 1);

        label_4 = new QLabel(TransformInspector);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        doubleSpinBoxPosX = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxPosX->setObjectName(QString::fromUtf8("doubleSpinBoxPosX"));
        doubleSpinBoxPosX->setMinimum(-1000000000.000000000000000);
        doubleSpinBoxPosX->setMaximum(1000000000.000000000000000);
        doubleSpinBoxPosX->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);

        gridLayout->addWidget(doubleSpinBoxPosX, 1, 3, 1, 1);

        label_3 = new QLabel(TransformInspector);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        doubleSpinBoxRotationY = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxRotationY->setObjectName(QString::fromUtf8("doubleSpinBoxRotationY"));
        doubleSpinBoxRotationY->setMaximum(1000000000.000000000000000);
        doubleSpinBoxRotationY->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
        doubleSpinBoxRotationY->setValue(0.000000000000000);

        gridLayout->addWidget(doubleSpinBoxRotationY, 7, 3, 1, 1);

        label_6 = new QLabel(TransformInspector);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 5, 0, 1, 1);

        doubleSpinBoxPosY = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxPosY->setObjectName(QString::fromUtf8("doubleSpinBoxPosY"));
        doubleSpinBoxPosY->setMinimum(-1000000000.000000000000000);
        doubleSpinBoxPosY->setMaximum(1000000000.000000000000000);
        doubleSpinBoxPosY->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);

        gridLayout->addWidget(doubleSpinBoxPosY, 2, 3, 1, 1);

        label_7 = new QLabel(TransformInspector);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 7, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 4, 3, 1, 1);

        doubleSpinBoxPosZ = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxPosZ->setObjectName(QString::fromUtf8("doubleSpinBoxPosZ"));
        doubleSpinBoxPosZ->setMinimum(-1000000000.000000000000000);
        doubleSpinBoxPosZ->setMaximum(1000000000.000000000000000);
        doubleSpinBoxPosZ->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);

        gridLayout->addWidget(doubleSpinBoxPosZ, 3, 3, 1, 1);

        label_2 = new QLabel(TransformInspector);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_10 = new QLabel(TransformInspector);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 10, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 3, 1, 1);

        label_11 = new QLabel(TransformInspector);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 11, 0, 1, 1);

        label_5 = new QLabel(TransformInspector);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        label_12 = new QLabel(TransformInspector);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout->addWidget(label_12, 12, 0, 1, 1);

        doubleSpinBoxRotationZ = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxRotationZ->setObjectName(QString::fromUtf8("doubleSpinBoxRotationZ"));
        doubleSpinBoxRotationZ->setMinimum(-1000000000.000000000000000);
        doubleSpinBoxRotationZ->setMaximum(1000000000.000000000000000);
        doubleSpinBoxRotationZ->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
        doubleSpinBoxRotationZ->setValue(0.000000000000000);

        gridLayout->addWidget(doubleSpinBoxRotationZ, 8, 3, 1, 1);

        label_8 = new QLabel(TransformInspector);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 8, 0, 1, 1);

        doubleSpinBoxRotationX = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxRotationX->setObjectName(QString::fromUtf8("doubleSpinBoxRotationX"));
        doubleSpinBoxRotationX->setMinimum(-1000000000.000000000000000);
        doubleSpinBoxRotationX->setMaximum(1000000000.000000000000000);
        doubleSpinBoxRotationX->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);

        gridLayout->addWidget(doubleSpinBoxRotationX, 5, 3, 1, 1);

        label = new QLabel(TransformInspector);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 9, 3, 1, 1);

        doubleSpinBoxScaleX = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxScaleX->setObjectName(QString::fromUtf8("doubleSpinBoxScaleX"));
        doubleSpinBoxScaleX->setMinimum(0.000000000000000);
        doubleSpinBoxScaleX->setMaximum(1000000000.000000000000000);
        doubleSpinBoxScaleX->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
        doubleSpinBoxScaleX->setValue(0.000000000000000);

        gridLayout->addWidget(doubleSpinBoxScaleX, 10, 3, 1, 1);

        doubleSpinBoxScaleY = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxScaleY->setObjectName(QString::fromUtf8("doubleSpinBoxScaleY"));
        doubleSpinBoxScaleY->setMinimum(0.000000000000000);
        doubleSpinBoxScaleY->setMaximum(1000000000.000000000000000);
        doubleSpinBoxScaleY->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
        doubleSpinBoxScaleY->setValue(0.000000000000000);

        gridLayout->addWidget(doubleSpinBoxScaleY, 11, 3, 1, 1);

        doubleSpinBoxScaleZ = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxScaleZ->setObjectName(QString::fromUtf8("doubleSpinBoxScaleZ"));
        doubleSpinBoxScaleZ->setMinimum(0.000000000000000);
        doubleSpinBoxScaleZ->setMaximum(1000000000.000000000000000);
        doubleSpinBoxScaleZ->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);

        gridLayout->addWidget(doubleSpinBoxScaleZ, 12, 3, 1, 1);


        retranslateUi(TransformInspector);

        QMetaObject::connectSlotsByName(TransformInspector);
    } // setupUi

    void retranslateUi(QWidget *TransformInspector)
    {
        TransformInspector->setWindowTitle(QCoreApplication::translate("TransformInspector", "Form", nullptr));
#if QT_CONFIG(tooltip)
        label_9->setToolTip(QCoreApplication::translate("TransformInspector", "\347\274\226\350\276\221\347\211\251\344\275\223\347\274\251\346\224\276", nullptr));
#endif // QT_CONFIG(tooltip)
        label_9->setText(QCoreApplication::translate("TransformInspector", "\347\274\251\346\224\276", nullptr));
#if QT_CONFIG(tooltip)
        label_4->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223\347\232\204z\345\235\220\346\240\207", nullptr));
#endif // QT_CONFIG(tooltip)
        label_4->setText(QCoreApplication::translate("TransformInspector", "z:", nullptr));
#if QT_CONFIG(tooltip)
        label_3->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223\347\232\204y\345\235\220\346\240\207", nullptr));
#endif // QT_CONFIG(tooltip)
        label_3->setText(QCoreApplication::translate("TransformInspector", "y:", nullptr));
        doubleSpinBoxRotationY->setSuffix(QCoreApplication::translate("TransformInspector", "\302\260", nullptr));
#if QT_CONFIG(tooltip)
        label_6->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223\347\273\225x\350\275\264\346\227\213\350\275\254\350\247\222\345\272\246", nullptr));
#endif // QT_CONFIG(tooltip)
        label_6->setText(QCoreApplication::translate("TransformInspector", "\316\261:", nullptr));
#if QT_CONFIG(tooltip)
        label_7->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223\347\273\225y\350\275\264\346\227\213\350\275\254\350\247\222\345\272\246", nullptr));
#endif // QT_CONFIG(tooltip)
        label_7->setText(QCoreApplication::translate("TransformInspector", "\316\262:", nullptr));
#if QT_CONFIG(tooltip)
        label_2->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223\347\232\204x\345\235\220\346\240\207", nullptr));
#endif // QT_CONFIG(tooltip)
        label_2->setText(QCoreApplication::translate("TransformInspector", "x:", nullptr));
#if QT_CONFIG(tooltip)
        label_10->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223x\350\275\264\347\274\251\346\224\276\345\244\247\345\260\217", nullptr));
#endif // QT_CONFIG(tooltip)
        label_10->setText(QCoreApplication::translate("TransformInspector", "Sx:", nullptr));
#if QT_CONFIG(tooltip)
        label_11->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223y\350\275\264\347\274\251\346\224\276\345\244\247\345\260\217", nullptr));
#endif // QT_CONFIG(tooltip)
        label_11->setText(QCoreApplication::translate("TransformInspector", "Sy:", nullptr));
#if QT_CONFIG(tooltip)
        label_5->setToolTip(QCoreApplication::translate("TransformInspector", "\347\274\226\350\276\221\347\211\251\344\275\223\347\232\204\346\227\213\350\275\254\350\247\222\345\272\246", nullptr));
#endif // QT_CONFIG(tooltip)
        label_5->setText(QCoreApplication::translate("TransformInspector", "\346\227\213\350\275\254", nullptr));
#if QT_CONFIG(tooltip)
        label_12->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223z\350\275\264\347\274\251\346\224\276\345\244\247\345\260\217", nullptr));
#endif // QT_CONFIG(tooltip)
        label_12->setText(QCoreApplication::translate("TransformInspector", "Sz:", nullptr));
        doubleSpinBoxRotationZ->setSuffix(QCoreApplication::translate("TransformInspector", "\302\260", nullptr));
#if QT_CONFIG(tooltip)
        label_8->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223\347\273\225z\350\275\264\346\227\213\350\275\254\350\247\222\345\272\246", nullptr));
#endif // QT_CONFIG(tooltip)
        label_8->setText(QCoreApplication::translate("TransformInspector", "\316\263:", nullptr));
        doubleSpinBoxRotationX->setSuffix(QCoreApplication::translate("TransformInspector", "\302\260", nullptr));
#if QT_CONFIG(tooltip)
        label->setToolTip(QCoreApplication::translate("TransformInspector", "\347\274\226\350\276\221\347\211\251\344\275\223\344\270\211\347\273\264\345\235\220\346\240\207", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        label->setWhatsThis(QCoreApplication::translate("TransformInspector", "<html><head/><body><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        label->setText(QCoreApplication::translate("TransformInspector", "\344\275\215\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TransformInspector: public Ui_TransformInspector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UITRANSFORMINSPECTOR_H
