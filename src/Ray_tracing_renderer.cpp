#include "testwindow.h"
#include <QApplication>
#include<iostream>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Hello, OneFLOW CFD!");
    w.show();
    return a.exec();
}