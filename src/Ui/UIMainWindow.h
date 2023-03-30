/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UIMAINWINDOW_H
#define UIMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "src/Ui/Console.h"
#include "src/Ui/EditorOpenGLWidget.h"
#include "src/Ui/Inspector.h"
#include "src/Ui/ModelListWidget.h"
#include "src/Ui/RayTracingOpenGLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *createScene;
    QAction *loadModel;
    QAction *loadSceneJson;
    QAction *saveSceneJson;
    QAction *saveAsSceneJson;
    QAction *closeScene;
    QAction *closeWindow;
    QAction *actionGray;
    QAction *actionBlue;
    QAction *actionDark;
    QAction *actionInspector;
    QAction *actionConsole;
    QAction *actionModel;
    QAction *actionRestore;
    QAction *actionHelp;
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *action;
    QAction *actionClear;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QWidget *renderWidget;
    QHBoxLayout *horizontalLayout_4;
    QLabel *fpslabel;
    QPushButton *renderButton;
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
    QMenu *menuClear;
    QMenu *menu_2;
    QMenu *menu_4;
    QMenu *menu_5;
    QMenu *menu_I;
    QStatusBar *statusbar;
    QDockWidget *dockWidgetInfos;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    ModelListWidget *listWidget;
    QDockWidget *dockWidgetConsole;
    QWidget *consoleContents;
    QVBoxLayout *verticalLayout;
    Console *console;
    QDockWidget *dockWidgetDetails;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_5;
    Inspector *inspector;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1031, 692);
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
        actionGray = new QAction(MainWindow);
        actionGray->setObjectName(QString::fromUtf8("actionGray"));
        actionBlue = new QAction(MainWindow);
        actionBlue->setObjectName(QString::fromUtf8("actionBlue"));
        actionDark = new QAction(MainWindow);
        actionDark->setObjectName(QString::fromUtf8("actionDark"));
        actionInspector = new QAction(MainWindow);
        actionInspector->setObjectName(QString::fromUtf8("actionInspector"));
        actionConsole = new QAction(MainWindow);
        actionConsole->setObjectName(QString::fromUtf8("actionConsole"));
        actionModel = new QAction(MainWindow);
        actionModel->setObjectName(QString::fromUtf8("actionModel"));
        actionRestore = new QAction(MainWindow);
        actionRestore->setObjectName(QString::fromUtf8("actionRestore"));
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionHelp->setShortcutContext(Qt::WindowShortcut);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        action = new QAction(MainWindow);
        action->setObjectName(QString::fromUtf8("action"));
        actionClear = new QAction(MainWindow);
        actionClear->setObjectName(QString::fromUtf8("actionClear"));
        widget = new QWidget(MainWindow);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(0, 0));
        widget->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        renderWidget = new QWidget(groupBox);
        renderWidget->setObjectName(QString::fromUtf8("renderWidget"));
        horizontalLayout_4 = new QHBoxLayout(renderWidget);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        fpslabel = new QLabel(renderWidget);
        fpslabel->setObjectName(QString::fromUtf8("fpslabel"));

        horizontalLayout_4->addWidget(fpslabel);

        renderButton = new QPushButton(renderWidget);
        renderButton->setObjectName(QString::fromUtf8("renderButton"));
        renderButton->setEnabled(true);
        sizePolicy.setHeightForWidth(renderButton->sizePolicy().hasHeightForWidth());
        renderButton->setSizePolicy(sizePolicy);
        renderButton->setMinimumSize(QSize(0, 0));
        renderButton->setMaximumSize(QSize(20, 20));

        horizontalLayout_4->addWidget(renderButton, 0, Qt::AlignHCenter|Qt::AlignVCenter);


        verticalLayout_4->addWidget(renderWidget);

        stackedWidget = new QStackedWidget(groupBox);
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

        verticalLayout_4->addWidget(stackedWidget);


        verticalLayout_3->addWidget(groupBox);

        MainWindow->setCentralWidget(widget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1031, 22));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu->setSeparatorsCollapsible(false);
        menu_3 = new QMenu(menu);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        menuClear = new QMenu(menu);
        menuClear->setObjectName(QString::fromUtf8("menuClear"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_2->setGeometry(QRect(396, 154, 126, 72));
        menu_4 = new QMenu(menubar);
        menu_4->setObjectName(QString::fromUtf8("menu_4"));
        menu_5 = new QMenu(menubar);
        menu_5->setObjectName(QString::fromUtf8("menu_5"));
        menu_5->setGeometry(QRect(251, 154, 120, 72));
        menu_I = new QMenu(menu_5);
        menu_I->setObjectName(QString::fromUtf8("menu_I"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
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
        dockWidgetConsole = new QDockWidget(MainWindow);
        dockWidgetConsole->setObjectName(QString::fromUtf8("dockWidgetConsole"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(dockWidgetConsole->sizePolicy().hasHeightForWidth());
        dockWidgetConsole->setSizePolicy(sizePolicy1);
        dockWidgetConsole->setMinimumSize(QSize(89, 125));
        dockWidgetConsole->setMaximumSize(QSize(524287, 325));
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
        dockWidgetDetails = new QDockWidget(MainWindow);
        dockWidgetDetails->setObjectName(QString::fromUtf8("dockWidgetDetails"));
        dockWidgetDetails->setMinimumSize(QSize(300, 170));
        dockWidgetDetails->setMaximumSize(QSize(500, 524287));
        dockWidgetDetails->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout_5 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        inspector = new Inspector(dockWidgetContents);
        inspector->setObjectName(QString::fromUtf8("inspector"));

        verticalLayout_5->addWidget(inspector);

        dockWidgetDetails->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidgetDetails);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_5->menuAction());
        menubar->addAction(menu_4->menuAction());
        menubar->addAction(menu_2->menuAction());
        menu->addAction(createScene);
        menu->addAction(menu_3->menuAction());
        menu->addAction(menuClear->menuAction());
        menu->addSeparator();
        menu->addAction(saveSceneJson);
        menu->addAction(saveAsSceneJson);
        menu->addSeparator();
        menu->addAction(closeScene);
        menu->addSeparator();
        menu->addAction(closeWindow);
        menu_3->addAction(loadModel);
        menu_3->addAction(loadSceneJson);
        menuClear->addSeparator();
        menuClear->addAction(actionClear);
        menu_2->addAction(actionHelp);
        menu_4->addAction(actionInspector);
        menu_4->addAction(actionConsole);
        menu_4->addAction(actionModel);
        menu_4->addAction(actionRestore);
        menu_5->addAction(menu_I->menuAction());
        menu_I->addAction(actionGray);
        menu_I->addAction(actionBlue);
        menu_I->addAction(actionDark);
        toolBar->addAction(actionNew);
        toolBar->addAction(actionOpen);
        toolBar->addAction(actionSave);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
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
        actionGray->setText(QCoreApplication::translate("MainWindow", "\346\265\205\350\211\262(&L)", nullptr));
        actionBlue->setText(QCoreApplication::translate("MainWindow", "\350\223\235\350\211\262(&B)", nullptr));
        actionDark->setText(QCoreApplication::translate("MainWindow", "\346\267\261\350\211\262(&D)", nullptr));
        actionInspector->setText(QCoreApplication::translate("MainWindow", "\346\243\200\346\237\245\345\231\250(&I)", nullptr));
        actionConsole->setText(QCoreApplication::translate("MainWindow", "\346\216\247\345\210\266\345\217\260(&C)", nullptr));
        actionModel->setText(QCoreApplication::translate("MainWindow", "\346\250\241\345\236\213(&M)", nullptr));
        actionRestore->setText(QCoreApplication::translate("MainWindow", "\350\277\230\345\216\237\345\270\203\345\261\200(&R)", nullptr));
        actionHelp->setText(QCoreApplication::translate("MainWindow", "\346\237\245\347\234\213\345\270\256\345\212\251(&H)", nullptr));
#if QT_CONFIG(shortcut)
        actionHelp->setShortcut(QCoreApplication::translate("MainWindow", "?", nullptr));
#endif // QT_CONFIG(shortcut)
        actionNew->setText(QString());
#if QT_CONFIG(tooltip)
        actionNew->setToolTip(QCoreApplication::translate("MainWindow", "\346\226\260\345\273\272\345\234\272\346\231\257", nullptr));
#endif // QT_CONFIG(tooltip)
        actionOpen->setText(QString());
#if QT_CONFIG(tooltip)
        actionOpen->setToolTip(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\345\234\272\346\231\257", nullptr));
#endif // QT_CONFIG(tooltip)
        actionSave->setText(QString());
#if QT_CONFIG(tooltip)
        actionSave->setToolTip(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
#endif // QT_CONFIG(tooltip)
        action->setText(QCoreApplication::translate("MainWindow", ".", nullptr));
        actionClear->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\350\217\234\345\215\225(&M)", nullptr));
        groupBox->setTitle(QString());
        fpslabel->setText(QString());
        renderButton->setText(QString());
        menu->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266(&F)", nullptr));
        menu_3->setTitle(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200(&O)", nullptr));
        menuClear->setTitle(QCoreApplication::translate("MainWindow", "\345\212\240\350\275\275\346\234\200\350\277\221(&R)", nullptr));
        menu_2->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251(&H)", nullptr));
        menu_4->setTitle(QCoreApplication::translate("MainWindow", "\347\252\227\345\217\243(&W)", nullptr));
        menu_5->setTitle(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256(&S)", nullptr));
        menu_I->setTitle(QCoreApplication::translate("MainWindow", "\344\270\273\351\242\230(&I)", nullptr));
        dockWidgetInfos->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\250\241\345\236\213", nullptr));
        dockWidgetConsole->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\216\247\345\210\266\345\217\260", nullptr));
        dockWidgetDetails->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\243\200\346\237\245\345\231\250", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UIMAINWINDOW_H
