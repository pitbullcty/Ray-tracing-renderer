#include "mainwindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("OpenGL");
    w.setStyle();
    w.showMaximized();
    return a.exec();
}   