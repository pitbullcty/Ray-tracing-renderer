#include "src/Ui/mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts); //全局共享OpenGL上下文，全屏切换
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load(":/qt_zh_CN.qm");
    qApp->installTranslator(&translator);
  
    QTranslator baseTranslator;
    qApp->installTranslator(&baseTranslator);
    baseTranslator.load(":/qtbase_zh_CN.qm");

    MainWindow w;
    return a.exec();
}   