/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Console.h"
#include "EditorOpenglWidget.h"
#include "ModelListWidget.h"
#include "RayTracingOpenGLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *save;
    QAction *createScene;
    QAction *loadModel;
    QAction *loadSceneJson;
    QAction *saveSceneJson;
    QAction *saveAsSceneJson;
    QAction *closeScene;
    QAction *closeWindow;
    QAction *actionloadRecent;
    QWidget *widget;
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *stackedWidgetPage1;
    QHBoxLayout *horizontalLayout_2;
    EditorOpenGLWidget *editor;
    QWidget *stackedWidgetPage2;
    QHBoxLayout *horizontalLayout;
    RayTracingOpenGLWidget *rayTracing;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_3;
    QMenu *menu_2;
    QMenu *menu_4;
    QMenu *menu_5;
    QStatusBar *statusbar;
    QToolBar *toolBar;
    QDockWidget *dockWidgetInfos;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    ModelListWidget *listWidget;
    QDockWidget *dockWidgetDetails;
    QWidget *dockWidgetContents_10;
    QDockWidget *dockWidgetConsole;
    QWidget *consoleContents;
    QVBoxLayout *verticalLayout;
    Console *console;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(850, 533);
        save = new QAction(MainWindow);
        save->setObjectName(QString::fromUtf8("save"));
        createScene = new QAction(MainWindow);
        createScene->setObjectName(QString::fromUtf8("createScene"));
        loadModel = new QAction(MainWindow);
        loadModel->setObjectName(QString::fromUtf8("loadModel"));
        loadSceneJson = new QAction(MainWindow);
        loadSceneJson->setObjectName(QString::fromUtf8("loadSceneJson"));
        saveSceneJson = new QAction(MainWindow);
        saveSceneJson->setObjectName(QString::fromUtf8("saveSceneJson"));
        saveSceneJson->setShortcutContext(Qt::WindowShortcut);
        saveAsSceneJson = new QAction(MainWindow);
        saveAsSceneJson->setObjectName(QString::fromUtf8("saveAsSceneJson"));
        closeScene = new QAction(MainWindow);
        closeScene->setObjectName(QString::fromUtf8("closeScene"));
        closeWindow = new QAction(MainWindow);
        closeWindow->setObjectName(QString::fromUtf8("closeWindow"));
        actionloadRecent = new QAction(MainWindow);
        actionloadRecent->setObjectName(QString::fromUtf8("actionloadRecent"));
        widget = new QWidget(MainWindow);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(0, 0));
        widget->setMaximumSize(QSize(16777215, 16777215));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        stackedWidget = new QStackedWidget(widget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidgetPage1 = new QWidget();
        stackedWidgetPage1->setObjectName(QString::fromUtf8("stackedWidgetPage1"));
        horizontalLayout_2 = new QHBoxLayout(stackedWidgetPage1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        editor = new EditorOpenGLWidget(stackedWidgetPage1);
        editor->setObjectName(QString::fromUtf8("editor"));

        horizontalLayout_2->addWidget(editor);

        stackedWidget->addWidget(stackedWidgetPage1);
        stackedWidgetPage2 = new QWidget();
        stackedWidgetPage2->setObjectName(QString::fromUtf8("stackedWidgetPage2"));
        horizontalLayout = new QHBoxLayout(stackedWidgetPage2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        rayTracing = new RayTracingOpenGLWidget(stackedWidgetPage2);
        rayTracing->setObjectName(QString::fromUtf8("rayTracing"));

        horizontalLayout->addWidget(rayTracing);

        stackedWidget->addWidget(stackedWidgetPage2);

        gridLayout->addWidget(stackedWidget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(widget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 850, 22));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu->setSeparatorsCollapsible(false);
        menu_3 = new QMenu(menu);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_2->setGeometry(QRect(477, 126, 117, 44));
        menu_4 = new QMenu(menubar);
        menu_4->setObjectName(QString::fromUtf8("menu_4"));
        menu_5 = new QMenu(menubar);
        menu_5->setObjectName(QString::fromUtf8("menu_5"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        dockWidgetInfos = new QDockWidget(MainWindow);
        dockWidgetInfos->setObjectName(QString::fromUtf8("dockWidgetInfos"));
        dockWidgetInfos->setMinimumSize(QSize(200, 140));
        dockWidgetInfos->setMaximumSize(QSize(500, 524287));
        dockWidgetInfos->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        widget_2 = new QWidget();
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        listWidget = new ModelListWidget(widget_2);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setStyleSheet(QString::fromUtf8("font: 700 10pt \"Microsoft YaHei UI\";"));

        verticalLayout_2->addWidget(listWidget);

        dockWidgetInfos->setWidget(widget_2);
        MainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidgetInfos);
        dockWidgetDetails = new QDockWidget(MainWindow);
        dockWidgetDetails->setObjectName(QString::fromUtf8("dockWidgetDetails"));
        dockWidgetDetails->setMinimumSize(QSize(200, 39));
        dockWidgetDetails->setMaximumSize(QSize(500, 524287));
        dockWidgetDetails->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents_10 = new QWidget();
        dockWidgetContents_10->setObjectName(QString::fromUtf8("dockWidgetContents_10"));
        dockWidgetDetails->setWidget(dockWidgetContents_10);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidgetDetails);
        dockWidgetConsole = new QDockWidget(MainWindow);
        dockWidgetConsole->setObjectName(QString::fromUtf8("dockWidgetConsole"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(dockWidgetConsole->sizePolicy().hasHeightForWidth());
        dockWidgetConsole->setSizePolicy(sizePolicy1);
        dockWidgetConsole->setMinimumSize(QSize(89, 112));
        dockWidgetConsole->setMaximumSize(QSize(524287, 500));
        dockWidgetConsole->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::TopDockWidgetArea);
        consoleContents = new QWidget();
        consoleContents->setObjectName(QString::fromUtf8("consoleContents"));
        verticalLayout = new QVBoxLayout(consoleContents);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        console = new Console(consoleContents);
        console->setObjectName(QString::fromUtf8("console"));

        verticalLayout->addWidget(console);

        dockWidgetConsole->setWidget(consoleContents);
        MainWindow->addDockWidget(Qt::BottomDockWidgetArea, dockWidgetConsole);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_5->menuAction());
        menubar->addAction(menu_4->menuAction());
        menubar->addAction(menu_2->menuAction());
        menu->addAction(createScene);
        menu->addAction(menu_3->menuAction());
        menu->addSeparator();
        menu->addAction(saveSceneJson);
        menu->addAction(saveAsSceneJson);
        menu->addSeparator();
        menu->addAction(closeScene);
        menu->addSeparator();
        menu->addAction(closeWindow);
        menu_3->addAction(loadModel);
        menu_3->addAction(loadSceneJson);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        save->setText(QCoreApplication::translate("MainWindow", "baocun1", nullptr));
        createScene->setText(QCoreApplication::translate("MainWindow", "\346\226\260\345\273\272(&N)...", nullptr));
#if QT_CONFIG(shortcut)
        createScene->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        loadModel->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\346\250\241\345\236\213", nullptr));
#if QT_CONFIG(shortcut)
        loadModel->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+M", nullptr));
#endif // QT_CONFIG(shortcut)
        loadSceneJson->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\345\234\272\346\231\257", nullptr));
#if QT_CONFIG(shortcut)
        loadSceneJson->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+F", nullptr));
#endif // QT_CONFIG(shortcut)
        saveSceneJson->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230(&S)...", nullptr));
#if QT_CONFIG(shortcut)
        saveSceneJson->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        saveAsSceneJson->setText(QCoreApplication::translate("MainWindow", "\345\217\246\345\255\230\344\270\272(&A)... ", nullptr));
#if QT_CONFIG(shortcut)
        saveAsSceneJson->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+A", nullptr));
#endif // QT_CONFIG(shortcut)
        closeScene->setText(QCoreApplication::translate("MainWindow", "\345\205\263\351\227\255(&C)... ", nullptr));
#if QT_CONFIG(shortcut)
        closeScene->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+F4", nullptr));
#endif // QT_CONFIG(shortcut)
        closeWindow->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272(&Q)...", nullptr));
        actionloadRecent->setText(QCoreApplication::translate("MainWindow", "loadRecent", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266(&F)", nullptr));
        menu_3->setTitle(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200(&O)", nullptr));
        menu_2->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251(&H)", nullptr));
        menu_4->setTitle(QCoreApplication::translate("MainWindow", "\347\252\227\345\217\243(&W)", nullptr));
        menu_5->setTitle(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256(&S)", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
        dockWidgetInfos->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\250\241\345\236\213", nullptr));
        dockWidgetDetails->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\250\241\345\236\213\344\277\241\346\201\257", nullptr));
        dockWidgetConsole->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\216\247\345\210\266\345\217\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
