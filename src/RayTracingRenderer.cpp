#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "Model.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("OpenGL");
    w.setStyle();
    w.showMaximized();
    Model h(QString("C:/Users/admin/OneDrive/C and C++ Programs/Ray tracing renderer/resources/Model/1/file.fbx"));
    return a.exec();
}   