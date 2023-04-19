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
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TransformInspector
{
public:
    QGridLayout *gridLayout;
    QLabel *label_3;
    QLabel *label_8;
    QDoubleSpinBox *doubleSpinBoxPosY;
    QLabel *label_11;
    QLabel *label_7;
    QLabel *label;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_4;
    QDoubleSpinBox *doubleSpinBoxRotationZ;
    QLabel *label_10;
    QDoubleSpinBox *doubleSpinBoxRotationY;
    QSpacerItem *horizontalSpacer_5;
    QDoubleSpinBox *doubleSpinBoxScaleZ;
    QLabel *label_14;
    QDoubleSpinBox *doubleSpinBoxScaleY;
    QLabel *label_5;
    QDoubleSpinBox *doubleSpinBoxPosZ;
    QDoubleSpinBox *doubleSpinBoxRotationX;
    QLabel *label_13;
    QDoubleSpinBox *doubleSpinBoxPosX;
    QLabel *label_2;
    QLabel *label_6;
    QLabel *label_12;
    QDoubleSpinBox *doubleSpinBoxScaleX;
    QSpacerItem *horizontalSpacer;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioButtonPos;
    QRadioButton *radioButtonRotation;
    QRadioButton *radioButtonScale;
    QLabel *label_9;
    QLabel *label_4;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButtonWorld;
    QRadioButton *radioButtonLocal;
    QButtonGroup *buttonGroup;
    QButtonGroup *buttonGroup2;

    void setupUi(QWidget *TransformInspector)
    {
        if (TransformInspector->objectName().isEmpty())
            TransformInspector->setObjectName(QString::fromUtf8("TransformInspector"));
        TransformInspector->resize(735, 593);
        gridLayout = new QGridLayout(TransformInspector);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_3 = new QLabel(TransformInspector);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 6, 0, 1, 1);

        label_8 = new QLabel(TransformInspector);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 12, 0, 1, 1);

        doubleSpinBoxPosY = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxPosY->setObjectName(QString::fromUtf8("doubleSpinBoxPosY"));
        doubleSpinBoxPosY->setKeyboardTracking(false);
        doubleSpinBoxPosY->setDecimals(4);
        doubleSpinBoxPosY->setMinimum(-1000000000.000000000000000);
        doubleSpinBoxPosY->setMaximum(1000000000.000000000000000);
        doubleSpinBoxPosY->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);

        gridLayout->addWidget(doubleSpinBoxPosY, 6, 3, 1, 1);

        label_11 = new QLabel(TransformInspector);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 15, 0, 1, 1);

        label_7 = new QLabel(TransformInspector);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 11, 0, 1, 1);

        label = new QLabel(TransformInspector);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 4, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 13, 3, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 8, 3, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 0, 3, 1, 1);

        doubleSpinBoxRotationZ = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxRotationZ->setObjectName(QString::fromUtf8("doubleSpinBoxRotationZ"));
        doubleSpinBoxRotationZ->setKeyboardTracking(false);
        doubleSpinBoxRotationZ->setDecimals(4);
        doubleSpinBoxRotationZ->setMinimum(-1000000000.000000000000000);
        doubleSpinBoxRotationZ->setMaximum(1000000000.000000000000000);
        doubleSpinBoxRotationZ->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
        doubleSpinBoxRotationZ->setValue(0.000000000000000);

        gridLayout->addWidget(doubleSpinBoxRotationZ, 12, 3, 1, 1);

        label_10 = new QLabel(TransformInspector);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 14, 0, 1, 1);

        doubleSpinBoxRotationY = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxRotationY->setObjectName(QString::fromUtf8("doubleSpinBoxRotationY"));
        doubleSpinBoxRotationY->setKeyboardTracking(false);
        doubleSpinBoxRotationY->setDecimals(4);
        doubleSpinBoxRotationY->setMaximum(1000000000.000000000000000);
        doubleSpinBoxRotationY->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
        doubleSpinBoxRotationY->setValue(0.000000000000000);

        gridLayout->addWidget(doubleSpinBoxRotationY, 11, 3, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 2, 3, 1, 1);

        doubleSpinBoxScaleZ = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxScaleZ->setObjectName(QString::fromUtf8("doubleSpinBoxScaleZ"));
        doubleSpinBoxScaleZ->setKeyboardTracking(false);
        doubleSpinBoxScaleZ->setDecimals(4);
        doubleSpinBoxScaleZ->setMinimum(0.000000000000000);
        doubleSpinBoxScaleZ->setMaximum(1000000000.000000000000000);
        doubleSpinBoxScaleZ->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);

        gridLayout->addWidget(doubleSpinBoxScaleZ, 16, 3, 1, 1);

        label_14 = new QLabel(TransformInspector);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout->addWidget(label_14, 2, 0, 1, 1);

        doubleSpinBoxScaleY = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxScaleY->setObjectName(QString::fromUtf8("doubleSpinBoxScaleY"));
        doubleSpinBoxScaleY->setKeyboardTracking(false);
        doubleSpinBoxScaleY->setDecimals(4);
        doubleSpinBoxScaleY->setMinimum(0.000000000000000);
        doubleSpinBoxScaleY->setMaximum(1000000000.000000000000000);
        doubleSpinBoxScaleY->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
        doubleSpinBoxScaleY->setValue(0.000000000000000);

        gridLayout->addWidget(doubleSpinBoxScaleY, 15, 3, 1, 1);

        label_5 = new QLabel(TransformInspector);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 8, 0, 1, 1);

        doubleSpinBoxPosZ = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxPosZ->setObjectName(QString::fromUtf8("doubleSpinBoxPosZ"));
        doubleSpinBoxPosZ->setKeyboardTracking(false);
        doubleSpinBoxPosZ->setDecimals(4);
        doubleSpinBoxPosZ->setMinimum(-1000000000.000000000000000);
        doubleSpinBoxPosZ->setMaximum(1000000000.000000000000000);
        doubleSpinBoxPosZ->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);

        gridLayout->addWidget(doubleSpinBoxPosZ, 7, 3, 1, 1);

        doubleSpinBoxRotationX = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxRotationX->setObjectName(QString::fromUtf8("doubleSpinBoxRotationX"));
        doubleSpinBoxRotationX->setKeyboardTracking(false);
        doubleSpinBoxRotationX->setDecimals(4);
        doubleSpinBoxRotationX->setMinimum(-1000000000.000000000000000);
        doubleSpinBoxRotationX->setMaximum(1000000000.000000000000000);
        doubleSpinBoxRotationX->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);

        gridLayout->addWidget(doubleSpinBoxRotationX, 9, 3, 1, 1);

        label_13 = new QLabel(TransformInspector);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout->addWidget(label_13, 0, 0, 1, 1);

        doubleSpinBoxPosX = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxPosX->setObjectName(QString::fromUtf8("doubleSpinBoxPosX"));
        doubleSpinBoxPosX->setKeyboardTracking(false);
        doubleSpinBoxPosX->setDecimals(4);
        doubleSpinBoxPosX->setMinimum(-1000000000.000000000000000);
        doubleSpinBoxPosX->setMaximum(1000000000.000000000000000);
        doubleSpinBoxPosX->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);

        gridLayout->addWidget(doubleSpinBoxPosX, 5, 3, 1, 1);

        label_2 = new QLabel(TransformInspector);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 5, 0, 1, 1);

        label_6 = new QLabel(TransformInspector);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 9, 0, 1, 1);

        label_12 = new QLabel(TransformInspector);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout->addWidget(label_12, 16, 0, 1, 1);

        doubleSpinBoxScaleX = new QDoubleSpinBox(TransformInspector);
        doubleSpinBoxScaleX->setObjectName(QString::fromUtf8("doubleSpinBoxScaleX"));
        doubleSpinBoxScaleX->setKeyboardTracking(false);
        doubleSpinBoxScaleX->setDecimals(4);
        doubleSpinBoxScaleX->setMinimum(0.000000000000000);
        doubleSpinBoxScaleX->setMaximum(1000000000.000000000000000);
        doubleSpinBoxScaleX->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
        doubleSpinBoxScaleX->setValue(0.000000000000000);

        gridLayout->addWidget(doubleSpinBoxScaleX, 14, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 4, 3, 1, 1);

        widget = new QWidget(TransformInspector);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        radioButtonPos = new QRadioButton(widget);
        buttonGroup = new QButtonGroup(TransformInspector);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(radioButtonPos);
        radioButtonPos->setObjectName(QString::fromUtf8("radioButtonPos"));

        horizontalLayout->addWidget(radioButtonPos);

        radioButtonRotation = new QRadioButton(widget);
        buttonGroup->addButton(radioButtonRotation);
        radioButtonRotation->setObjectName(QString::fromUtf8("radioButtonRotation"));

        horizontalLayout->addWidget(radioButtonRotation);

        radioButtonScale = new QRadioButton(widget);
        buttonGroup->addButton(radioButtonScale);
        radioButtonScale->setObjectName(QString::fromUtf8("radioButtonScale"));

        horizontalLayout->addWidget(radioButtonScale);


        gridLayout->addWidget(widget, 1, 0, 1, 4);

        label_9 = new QLabel(TransformInspector);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 13, 0, 1, 1);

        label_4 = new QLabel(TransformInspector);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 7, 0, 1, 1);

        widget_2 = new QWidget(TransformInspector);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        radioButtonWorld = new QRadioButton(widget_2);
        buttonGroup2 = new QButtonGroup(TransformInspector);
        buttonGroup2->setObjectName(QString::fromUtf8("buttonGroup2"));
        buttonGroup2->addButton(radioButtonWorld);
        radioButtonWorld->setObjectName(QString::fromUtf8("radioButtonWorld"));

        horizontalLayout_2->addWidget(radioButtonWorld);

        radioButtonLocal = new QRadioButton(widget_2);
        buttonGroup2->addButton(radioButtonLocal);
        radioButtonLocal->setObjectName(QString::fromUtf8("radioButtonLocal"));

        horizontalLayout_2->addWidget(radioButtonLocal);


        gridLayout->addWidget(widget_2, 3, 0, 1, 4);


        retranslateUi(TransformInspector);

        QMetaObject::connectSlotsByName(TransformInspector);
    } // setupUi

    void retranslateUi(QWidget *TransformInspector)
    {
        TransformInspector->setWindowTitle(QCoreApplication::translate("TransformInspector", "Form", nullptr));
#if QT_CONFIG(tooltip)
        label_3->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223\347\232\204y\345\235\220\346\240\207", nullptr));
#endif // QT_CONFIG(tooltip)
        label_3->setText(QCoreApplication::translate("TransformInspector", "y:", nullptr));
#if QT_CONFIG(tooltip)
        label_8->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223\347\273\225z\350\275\264\346\227\213\350\275\254\350\247\222\345\272\246", nullptr));
#endif // QT_CONFIG(tooltip)
        label_8->setText(QCoreApplication::translate("TransformInspector", "\316\263:", nullptr));
#if QT_CONFIG(tooltip)
        label_11->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223y\350\275\264\347\274\251\346\224\276\345\244\247\345\260\217", nullptr));
#endif // QT_CONFIG(tooltip)
        label_11->setText(QCoreApplication::translate("TransformInspector", "Sy:", nullptr));
#if QT_CONFIG(tooltip)
        label_7->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223\347\273\225y\350\275\264\346\227\213\350\275\254\350\247\222\345\272\246", nullptr));
#endif // QT_CONFIG(tooltip)
        label_7->setText(QCoreApplication::translate("TransformInspector", "\316\262:", nullptr));
#if QT_CONFIG(tooltip)
        label->setToolTip(QCoreApplication::translate("TransformInspector", "\347\274\226\350\276\221\347\211\251\344\275\223\344\270\211\347\273\264\345\235\220\346\240\207", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        label->setWhatsThis(QCoreApplication::translate("TransformInspector", "<html><head/><body><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        label->setText(QCoreApplication::translate("TransformInspector", "\344\275\215\347\275\256", nullptr));
        doubleSpinBoxRotationZ->setSuffix(QCoreApplication::translate("TransformInspector", "\302\260", nullptr));
#if QT_CONFIG(tooltip)
        label_10->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223x\350\275\264\347\274\251\346\224\276\345\244\247\345\260\217", nullptr));
#endif // QT_CONFIG(tooltip)
        label_10->setText(QCoreApplication::translate("TransformInspector", "Sx:", nullptr));
        doubleSpinBoxRotationY->setSuffix(QCoreApplication::translate("TransformInspector", "\302\260", nullptr));
#if QT_CONFIG(tooltip)
        label_14->setToolTip(QCoreApplication::translate("TransformInspector", "\347\274\226\350\276\221\345\231\250\346\230\276\347\244\272\347\232\204\345\217\202\350\200\203\345\235\220\346\240\207\347\263\273", nullptr));
#endif // QT_CONFIG(tooltip)
        label_14->setText(QCoreApplication::translate("TransformInspector", "\347\274\226\350\276\221\345\231\250\345\217\202\350\200\203\345\235\220\346\240\207\347\263\273", nullptr));
#if QT_CONFIG(tooltip)
        label_5->setToolTip(QCoreApplication::translate("TransformInspector", "\347\274\226\350\276\221\347\211\251\344\275\223\347\232\204\346\227\213\350\275\254\350\247\222\345\272\246", nullptr));
#endif // QT_CONFIG(tooltip)
        label_5->setText(QCoreApplication::translate("TransformInspector", "\346\227\213\350\275\254", nullptr));
        doubleSpinBoxRotationX->setSuffix(QCoreApplication::translate("TransformInspector", "\302\260", nullptr));
#if QT_CONFIG(tooltip)
        label_13->setToolTip(QCoreApplication::translate("TransformInspector", "\344\277\256\346\224\271gizmo\347\261\273\345\236\213", nullptr));
#endif // QT_CONFIG(tooltip)
        label_13->setText(QCoreApplication::translate("TransformInspector", "\347\274\226\350\276\221\345\231\250\347\261\273\345\236\213", nullptr));
#if QT_CONFIG(tooltip)
        label_2->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223\347\232\204x\345\235\220\346\240\207", nullptr));
#endif // QT_CONFIG(tooltip)
        label_2->setText(QCoreApplication::translate("TransformInspector", "x:", nullptr));
#if QT_CONFIG(tooltip)
        label_6->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223\347\273\225x\350\275\264\346\227\213\350\275\254\350\247\222\345\272\246", nullptr));
#endif // QT_CONFIG(tooltip)
        label_6->setText(QCoreApplication::translate("TransformInspector", "\316\261:", nullptr));
#if QT_CONFIG(tooltip)
        label_12->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223z\350\275\264\347\274\251\346\224\276\345\244\247\345\260\217", nullptr));
#endif // QT_CONFIG(tooltip)
        label_12->setText(QCoreApplication::translate("TransformInspector", "Sz:", nullptr));
        radioButtonPos->setText(QCoreApplication::translate("TransformInspector", "\344\275\215\347\275\256", nullptr));
        radioButtonRotation->setText(QCoreApplication::translate("TransformInspector", "\346\227\213\350\275\254", nullptr));
        radioButtonScale->setText(QCoreApplication::translate("TransformInspector", "\347\274\251\346\224\276", nullptr));
#if QT_CONFIG(tooltip)
        label_9->setToolTip(QCoreApplication::translate("TransformInspector", "\347\274\226\350\276\221\347\211\251\344\275\223\347\274\251\346\224\276", nullptr));
#endif // QT_CONFIG(tooltip)
        label_9->setText(QCoreApplication::translate("TransformInspector", "\347\274\251\346\224\276", nullptr));
#if QT_CONFIG(tooltip)
        label_4->setToolTip(QCoreApplication::translate("TransformInspector", "\347\211\251\344\275\223\347\232\204z\345\235\220\346\240\207", nullptr));
#endif // QT_CONFIG(tooltip)
        label_4->setText(QCoreApplication::translate("TransformInspector", "z:", nullptr));
        radioButtonWorld->setText(QCoreApplication::translate("TransformInspector", "\344\270\226\347\225\214", nullptr));
        radioButtonLocal->setText(QCoreApplication::translate("TransformInspector", "\350\207\252\350\272\253", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TransformInspector: public Ui_TransformInspector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UITRANSFORMINSPECTOR_H
