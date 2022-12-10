#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("OpenGL");
    w.setStyle();
    w.showMaximized();
    return a.exec();
}