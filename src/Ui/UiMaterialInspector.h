/********************************************************************************
** Form generated from reading UI file 'MaterialInspector.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UIMATERIALINSPECTOR_H
#define UIMATERIALINSPECTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MaterialInspector
{
public:
    QGridLayout *gridLayout;
    QWidget *baseColorWidget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelBaseColor;
    QToolButton *toolButtonBaseColor;
    QWidget *brdf;
    QGridLayout *gridLayout_8;
    QSlider *horizontalSliderSpecularTint;
    QSlider *horizontalSliderSheenTint;
    QLabel *label_10;
    QSlider *horizontalSliderMetallic;
    QSlider *horizontalSliderClearcoat;
    QLabel *subsurface;
    QLabel *roughness;
    QLabel *metallic;
    QLabel *label_11;
    QSlider *horizontalSliderRoughness;
    QLabel *specular;
    QLabel *label_4;
    QLabel *label_6;
    QLabel *label_5;
    QLabel *specularTint;
    QLabel *label_9;
    QLabel *anisotropic;
    QLabel *label_12;
    QSlider *horizontalSliderSpecular;
    QLabel *label_8;
    QLabel *clearcoatGloss;
    QSlider *horizontalSliderSheen;
    QLabel *label_7;
    QLabel *sheen;
    QLabel *sheenTint;
    QLabel *clearcoat;
    QSlider *horizontalSliderAnisotropic;
    QSlider *horizontalSliderSubsurface;
    QSlider *horizontalSliderClearcoatGloss;
    QLabel *label_3;
    QWidget *emissiveWidget;
    QGridLayout *gridLayout_2;
    QLabel *label_13;
    QLabel *labelFactor;
    QSlider *horizontalSliderFactor;
    QLabel *label;
    QToolButton *toolButtonEmissive;
    QLabel *labelEmissive;

    void setupUi(QWidget *MaterialInspector)
    {
        if (MaterialInspector->objectName().isEmpty())
            MaterialInspector->setObjectName(QString::fromUtf8("MaterialInspector"));
        MaterialInspector->resize(591, 472);
        gridLayout = new QGridLayout(MaterialInspector);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        baseColorWidget = new QWidget(MaterialInspector);
        baseColorWidget->setObjectName(QString::fromUtf8("baseColorWidget"));
        horizontalLayout_3 = new QHBoxLayout(baseColorWidget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(baseColorWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        labelBaseColor = new QLabel(baseColorWidget);
        labelBaseColor->setObjectName(QString::fromUtf8("labelBaseColor"));

        horizontalLayout_2->addWidget(labelBaseColor);

        toolButtonBaseColor = new QToolButton(baseColorWidget);
        toolButtonBaseColor->setObjectName(QString::fromUtf8("toolButtonBaseColor"));

        horizontalLayout_2->addWidget(toolButtonBaseColor);


        horizontalLayout_3->addLayout(horizontalLayout_2);


        gridLayout->addWidget(baseColorWidget, 2, 0, 1, 2);

        brdf = new QWidget(MaterialInspector);
        brdf->setObjectName(QString::fromUtf8("brdf"));
        gridLayout_8 = new QGridLayout(brdf);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        horizontalSliderSpecularTint = new QSlider(brdf);
        horizontalSliderSpecularTint->setObjectName(QString::fromUtf8("horizontalSliderSpecularTint"));
        horizontalSliderSpecularTint->setMaximum(100);
        horizontalSliderSpecularTint->setPageStep(1);
        horizontalSliderSpecularTint->setOrientation(Qt::Horizontal);

        gridLayout_8->addWidget(horizontalSliderSpecularTint, 6, 2, 1, 1);

        horizontalSliderSheenTint = new QSlider(brdf);
        horizontalSliderSheenTint->setObjectName(QString::fromUtf8("horizontalSliderSheenTint"));
        horizontalSliderSheenTint->setMaximum(100);
        horizontalSliderSheenTint->setPageStep(1);
        horizontalSliderSheenTint->setOrientation(Qt::Horizontal);

        gridLayout_8->addWidget(horizontalSliderSheenTint, 10, 2, 1, 1);

        label_10 = new QLabel(brdf);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_8->addWidget(label_10, 11, 0, 1, 1);

        horizontalSliderMetallic = new QSlider(brdf);
        horizontalSliderMetallic->setObjectName(QString::fromUtf8("horizontalSliderMetallic"));
        horizontalSliderMetallic->setMaximum(100);
        horizontalSliderMetallic->setPageStep(1);
        horizontalSliderMetallic->setOrientation(Qt::Horizontal);

        gridLayout_8->addWidget(horizontalSliderMetallic, 3, 2, 1, 1);

        horizontalSliderClearcoat = new QSlider(brdf);
        horizontalSliderClearcoat->setObjectName(QString::fromUtf8("horizontalSliderClearcoat"));
        horizontalSliderClearcoat->setMaximum(100);
        horizontalSliderClearcoat->setPageStep(1);
        horizontalSliderClearcoat->setOrientation(Qt::Horizontal);

        gridLayout_8->addWidget(horizontalSliderClearcoat, 11, 2, 1, 1);

        subsurface = new QLabel(brdf);
        subsurface->setObjectName(QString::fromUtf8("subsurface"));

        gridLayout_8->addWidget(subsurface, 1, 1, 1, 1);

        roughness = new QLabel(brdf);
        roughness->setObjectName(QString::fromUtf8("roughness"));

        gridLayout_8->addWidget(roughness, 7, 1, 1, 1);

        metallic = new QLabel(brdf);
        metallic->setObjectName(QString::fromUtf8("metallic"));

        gridLayout_8->addWidget(metallic, 3, 1, 1, 1);

        label_11 = new QLabel(brdf);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_8->addWidget(label_11, 12, 0, 1, 1);

        horizontalSliderRoughness = new QSlider(brdf);
        horizontalSliderRoughness->setObjectName(QString::fromUtf8("horizontalSliderRoughness"));
        horizontalSliderRoughness->setMaximum(100);
        horizontalSliderRoughness->setPageStep(1);
        horizontalSliderRoughness->setOrientation(Qt::Horizontal);

        gridLayout_8->addWidget(horizontalSliderRoughness, 7, 2, 1, 1);

        specular = new QLabel(brdf);
        specular->setObjectName(QString::fromUtf8("specular"));

        gridLayout_8->addWidget(specular, 5, 1, 1, 1);

        label_4 = new QLabel(brdf);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_8->addWidget(label_4, 5, 0, 1, 1);

        label_6 = new QLabel(brdf);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_8->addWidget(label_6, 7, 0, 1, 1);

        label_5 = new QLabel(brdf);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_8->addWidget(label_5, 6, 0, 1, 1);

        specularTint = new QLabel(brdf);
        specularTint->setObjectName(QString::fromUtf8("specularTint"));

        gridLayout_8->addWidget(specularTint, 6, 1, 1, 1);

        label_9 = new QLabel(brdf);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_8->addWidget(label_9, 10, 0, 1, 1);

        anisotropic = new QLabel(brdf);
        anisotropic->setObjectName(QString::fromUtf8("anisotropic"));

        gridLayout_8->addWidget(anisotropic, 8, 1, 1, 1);

        label_12 = new QLabel(brdf);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_8->addWidget(label_12, 3, 0, 1, 1);

        horizontalSliderSpecular = new QSlider(brdf);
        horizontalSliderSpecular->setObjectName(QString::fromUtf8("horizontalSliderSpecular"));
        horizontalSliderSpecular->setMaximum(100);
        horizontalSliderSpecular->setPageStep(1);
        horizontalSliderSpecular->setOrientation(Qt::Horizontal);

        gridLayout_8->addWidget(horizontalSliderSpecular, 5, 2, 1, 1);

        label_8 = new QLabel(brdf);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_8->addWidget(label_8, 9, 0, 1, 1);

        clearcoatGloss = new QLabel(brdf);
        clearcoatGloss->setObjectName(QString::fromUtf8("clearcoatGloss"));

        gridLayout_8->addWidget(clearcoatGloss, 12, 1, 1, 1);

        horizontalSliderSheen = new QSlider(brdf);
        horizontalSliderSheen->setObjectName(QString::fromUtf8("horizontalSliderSheen"));
        horizontalSliderSheen->setMaximum(100);
        horizontalSliderSheen->setPageStep(1);
        horizontalSliderSheen->setOrientation(Qt::Horizontal);

        gridLayout_8->addWidget(horizontalSliderSheen, 9, 2, 1, 1);

        label_7 = new QLabel(brdf);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_8->addWidget(label_7, 8, 0, 1, 1);

        sheen = new QLabel(brdf);
        sheen->setObjectName(QString::fromUtf8("sheen"));

        gridLayout_8->addWidget(sheen, 9, 1, 1, 1);

        sheenTint = new QLabel(brdf);
        sheenTint->setObjectName(QString::fromUtf8("sheenTint"));

        gridLayout_8->addWidget(sheenTint, 10, 1, 1, 1);

        clearcoat = new QLabel(brdf);
        clearcoat->setObjectName(QString::fromUtf8("clearcoat"));

        gridLayout_8->addWidget(clearcoat, 11, 1, 1, 1);

        horizontalSliderAnisotropic = new QSlider(brdf);
        horizontalSliderAnisotropic->setObjectName(QString::fromUtf8("horizontalSliderAnisotropic"));
        horizontalSliderAnisotropic->setMaximum(100);
        horizontalSliderAnisotropic->setPageStep(1);
        horizontalSliderAnisotropic->setOrientation(Qt::Horizontal);

        gridLayout_8->addWidget(horizontalSliderAnisotropic, 8, 2, 1, 1);

        horizontalSliderSubsurface = new QSlider(brdf);
        horizontalSliderSubsurface->setObjectName(QString::fromUtf8("horizontalSliderSubsurface"));
        horizontalSliderSubsurface->setFocusPolicy(Qt::WheelFocus);
        horizontalSliderSubsurface->setMaximum(100);
        horizontalSliderSubsurface->setPageStep(1);
        horizontalSliderSubsurface->setValue(0);
        horizontalSliderSubsurface->setOrientation(Qt::Horizontal);

        gridLayout_8->addWidget(horizontalSliderSubsurface, 1, 2, 1, 1);

        horizontalSliderClearcoatGloss = new QSlider(brdf);
        horizontalSliderClearcoatGloss->setObjectName(QString::fromUtf8("horizontalSliderClearcoatGloss"));
        horizontalSliderClearcoatGloss->setMaximum(100);
        horizontalSliderClearcoatGloss->setPageStep(1);
        horizontalSliderClearcoatGloss->setOrientation(Qt::Horizontal);

        gridLayout_8->addWidget(horizontalSliderClearcoatGloss, 12, 2, 1, 1);

        label_3 = new QLabel(brdf);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_8->addWidget(label_3, 1, 0, 1, 1);


        gridLayout->addWidget(brdf, 4, 0, 1, 2);

        emissiveWidget = new QWidget(MaterialInspector);
        emissiveWidget->setObjectName(QString::fromUtf8("emissiveWidget"));
        gridLayout_2 = new QGridLayout(emissiveWidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_13 = new QLabel(emissiveWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_2->addWidget(label_13, 2, 0, 1, 1);

        labelFactor = new QLabel(emissiveWidget);
        labelFactor->setObjectName(QString::fromUtf8("labelFactor"));

        gridLayout_2->addWidget(labelFactor, 2, 1, 1, 1);

        horizontalSliderFactor = new QSlider(emissiveWidget);
        horizontalSliderFactor->setObjectName(QString::fromUtf8("horizontalSliderFactor"));
        horizontalSliderFactor->setMinimum(1);
        horizontalSliderFactor->setMaximum(255);
        horizontalSliderFactor->setValue(20);
        horizontalSliderFactor->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(horizontalSliderFactor, 2, 2, 1, 1);

        label = new QLabel(emissiveWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 1, 0, 1, 1);

        toolButtonEmissive = new QToolButton(emissiveWidget);
        toolButtonEmissive->setObjectName(QString::fromUtf8("toolButtonEmissive"));

        gridLayout_2->addWidget(toolButtonEmissive, 1, 3, 1, 1);

        labelEmissive = new QLabel(emissiveWidget);
        labelEmissive->setObjectName(QString::fromUtf8("labelEmissive"));

        gridLayout_2->addWidget(labelEmissive, 1, 1, 1, 2);


        gridLayout->addWidget(emissiveWidget, 0, 0, 1, 2);


        retranslateUi(MaterialInspector);

        QMetaObject::connectSlotsByName(MaterialInspector);
    } // setupUi

    void retranslateUi(QWidget *MaterialInspector)
    {
        MaterialInspector->setWindowTitle(QCoreApplication::translate("MaterialInspector", "Form", nullptr));
#if QT_CONFIG(tooltip)
        label_2->setToolTip(QCoreApplication::translate("MaterialInspector", "\350\241\250\347\244\272\347\211\251\344\275\223\350\241\250\351\235\242\347\232\204\345\237\272\346\234\254\351\242\234\350\211\262", nullptr));
#endif // QT_CONFIG(tooltip)
        label_2->setText(QCoreApplication::translate("MaterialInspector", "\345\237\272\346\234\254\351\242\234\350\211\262", nullptr));
        labelBaseColor->setText(QString());
        toolButtonBaseColor->setText(QCoreApplication::translate("MaterialInspector", "\347\274\226\350\276\221", nullptr));
#if QT_CONFIG(tooltip)
        label_10->setToolTip(QCoreApplication::translate("MaterialInspector", "\350\241\250\347\244\272\347\211\251\344\275\223\350\241\250\351\235\242\347\232\204\346\270\205\346\274\206\345\261\202\345\274\272\345\272\246\357\274\214\346\225\260\345\200\274\350\266\212\345\244\247\345\274\272\345\272\246\350\266\212\351\253\230", nullptr));
#endif // QT_CONFIG(tooltip)
        label_10->setText(QCoreApplication::translate("MaterialInspector", "\346\270\205\346\274\206\345\274\272\345\272\246", nullptr));
        subsurface->setText(QString());
        roughness->setText(QString());
        metallic->setText(QString());
#if QT_CONFIG(tooltip)
        label_11->setToolTip(QCoreApplication::translate("MaterialInspector", "\350\241\250\347\244\272\347\211\251\344\275\223\350\241\250\351\235\242\346\270\205\346\274\206\347\232\204\345\205\211\346\263\275\347\250\213\345\272\246\357\274\214\346\225\260\345\200\274\350\266\212\345\244\247\350\241\250\347\244\272\350\266\212\345\205\211\346\273\221\343\200\202", nullptr));
#endif // QT_CONFIG(tooltip)
        label_11->setText(QCoreApplication::translate("MaterialInspector", "\346\270\205\346\274\206\345\205\211\346\263\275\345\272\246", nullptr));
        specular->setText(QString());
#if QT_CONFIG(tooltip)
        label_4->setToolTip(QCoreApplication::translate("MaterialInspector", "\350\241\250\347\244\272\347\211\251\344\275\223\350\241\250\351\235\242\345\217\215\345\260\204\345\205\211\347\232\204\345\274\272\345\272\246\357\274\214\346\225\260\345\200\274\350\266\212\351\253\230\345\274\272\345\272\246\350\266\212\345\274\272", nullptr));
#endif // QT_CONFIG(tooltip)
        label_4->setText(QCoreApplication::translate("MaterialInspector", "\351\253\230\345\205\211\345\274\272\345\272\246", nullptr));
#if QT_CONFIG(tooltip)
        label_6->setToolTip(QCoreApplication::translate("MaterialInspector", "\350\241\250\347\244\272\347\211\251\344\275\223\350\241\250\351\235\242\347\232\204\345\205\211\346\273\221\347\250\213\345\272\246\357\274\214\346\225\260\345\200\274\350\266\212\345\260\217\350\241\250\347\244\272\350\266\212\345\205\211\346\273\221", nullptr));
#endif // QT_CONFIG(tooltip)
        label_6->setText(QCoreApplication::translate("MaterialInspector", "\347\262\227\347\263\231\345\272\246", nullptr));
#if QT_CONFIG(tooltip)
        label_5->setToolTip(QCoreApplication::translate("MaterialInspector", "\350\241\250\347\244\272\351\253\230\345\205\211\347\232\204\351\242\234\350\211\262\357\274\214\346\225\260\345\200\274\350\266\212\345\244\247\350\241\250\347\244\272\351\242\234\350\211\262\350\266\212\346\216\245\350\277\221\345\237\272\346\234\254\351\242\234\350\211\262", nullptr));
#endif // QT_CONFIG(tooltip)
        label_5->setText(QCoreApplication::translate("MaterialInspector", "\351\253\230\345\205\211\350\211\262\350\260\203", nullptr));
        specularTint->setText(QString());
#if QT_CONFIG(tooltip)
        label_9->setToolTip(QCoreApplication::translate("MaterialInspector", "\350\241\250\347\244\272\345\205\211\346\263\275\347\232\204\351\242\234\350\211\262\357\274\214\346\225\260\345\200\274\350\266\212\345\244\247\350\241\250\347\244\272\351\242\234\350\211\262\350\266\212\346\216\245\350\277\221\345\237\272\346\234\254\351\242\234\350\211\262", nullptr));
#endif // QT_CONFIG(tooltip)
        label_9->setText(QCoreApplication::translate("MaterialInspector", "\345\205\211\346\263\275\350\211\262\350\260\203", nullptr));
        anisotropic->setText(QString());
#if QT_CONFIG(tooltip)
        label_12->setToolTip(QCoreApplication::translate("MaterialInspector", "\350\241\250\347\244\272\347\211\251\344\275\223\346\230\257\345\220\246\346\230\257\351\207\221\345\261\236\346\235\220\350\264\250\357\274\214\346\225\260\345\200\274\350\266\212\345\244\247\350\241\250\347\244\272\350\266\212\351\235\240\350\277\221\351\207\221\345\261\236", nullptr));
#endif // QT_CONFIG(tooltip)
        label_12->setText(QCoreApplication::translate("MaterialInspector", "\351\207\221\345\261\236\345\272\246", nullptr));
#if QT_CONFIG(tooltip)
        label_8->setToolTip(QCoreApplication::translate("MaterialInspector", "\350\241\250\347\244\272\347\211\251\344\275\223\350\241\250\351\235\242\347\232\204\345\205\211\346\263\275\347\250\213\345\272\246\357\274\214\346\225\260\345\200\274\350\266\212\345\244\247\350\241\250\347\244\272\350\266\212\346\234\211\345\205\211\346\263\275", nullptr));
#endif // QT_CONFIG(tooltip)
        label_8->setText(QCoreApplication::translate("MaterialInspector", "\345\205\211\346\263\275\345\272\246", nullptr));
        clearcoatGloss->setText(QString());
#if QT_CONFIG(tooltip)
        label_7->setToolTip(QCoreApplication::translate("MaterialInspector", "\350\241\250\347\244\272\347\211\251\344\275\223\350\241\250\351\235\242\347\232\204\345\217\215\345\260\204\347\211\271\346\200\247\346\230\257\345\220\246\345\220\204\345\220\221\345\220\214\346\200\247\357\274\214\346\225\260\345\200\274\350\266\212\345\244\247\350\241\250\347\244\272\345\220\204\345\220\221\345\274\202\346\200\247\350\266\212\345\274\272", nullptr));
#endif // QT_CONFIG(tooltip)
        label_7->setText(QCoreApplication::translate("MaterialInspector", "\345\220\204\345\220\221\345\274\202\346\200\247", nullptr));
        sheen->setText(QString());
        sheenTint->setText(QString());
        clearcoat->setText(QString());
#if QT_CONFIG(tooltip)
        label_3->setToolTip(QCoreApplication::translate("MaterialInspector", "\346\216\247\345\210\266\346\274\253\345\217\215\345\260\204\347\232\204\345\275\242\347\212\266\357\274\214\351\200\232\345\270\270\347\224\250\344\272\216\346\250\241\346\213\237\351\200\217\346\230\216\347\211\251\344\275\223\357\274\214\344\276\213\345\246\202\347\232\256\350\202\244\343\200\201\345\244\247\347\220\206\347\237\263\347\255\211", nullptr));
#endif // QT_CONFIG(tooltip)
        label_3->setText(QCoreApplication::translate("MaterialInspector", "\346\254\241\350\241\250\351\235\242", nullptr));
#if QT_CONFIG(tooltip)
        label_13->setToolTip(QCoreApplication::translate("MaterialInspector", "\350\207\252\345\217\221\345\205\211\345\274\272\345\272\246\357\274\214\346\225\260\345\200\274\350\266\212\351\253\230\345\274\272\345\272\246\350\266\212\345\244\247", nullptr));
#endif // QT_CONFIG(tooltip)
        label_13->setText(QCoreApplication::translate("MaterialInspector", "\345\205\211\347\205\247\345\274\272\345\272\246", nullptr));
        labelFactor->setText(QCoreApplication::translate("MaterialInspector", "20", nullptr));
#if QT_CONFIG(tooltip)
        label->setToolTip(QCoreApplication::translate("MaterialInspector", "\344\275\234\344\270\272\345\205\211\346\272\220\346\227\266\347\232\204\345\217\221\345\205\211\351\242\234\350\211\262\357\274\214\351\273\221\350\211\262\344\273\243\350\241\250\344\270\215\345\217\221\345\205\211", nullptr));
#endif // QT_CONFIG(tooltip)
        label->setText(QCoreApplication::translate("MaterialInspector", "\345\217\221\345\205\211\351\242\234\350\211\262", nullptr));
        toolButtonEmissive->setText(QCoreApplication::translate("MaterialInspector", "\347\274\226\350\276\221", nullptr));
        labelEmissive->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MaterialInspector: public Ui_MaterialInspector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UIMATERIALINSPECTOR_H
