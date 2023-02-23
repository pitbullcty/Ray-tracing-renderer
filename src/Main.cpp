#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts); //全局共享OpenGL上下文
    QApplication a(argc, argv);

    QTranslator basetranslator;
    basetranslator.load(":/qtbase_zh_CN.qm");
    qApp->installTranslator(&basetranslator);
  
    QTranslator translator;
    qApp->installTranslator(&translator);
    translator.load(":/qtbase_zh_CN.qm");

    MainWindow w;
    return a.exec();
}   