#include "mainwindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts); //一定要全局共享上下文
    QApplication a(argc, argv);
    MainWindow w;
    return a.exec();
}   