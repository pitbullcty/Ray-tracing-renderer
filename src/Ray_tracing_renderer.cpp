#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>

void getSupportedFormats()
{
    std::string all = "All (";
    std::string individual = "";
    Assimp::Exporter exporter;
    for (size_t i = 0; i < exporter.GetExportFormatCount(); ++i)
    {
        const aiExportFormatDesc* desc = exporter.GetExportFormatDescription(i);
        all += "*." + std::string(desc->fileExtension) + " ";
        individual += ";;" + std::string(desc->description) + " (*." +
            std::string(desc->fileExtension) + ")";
    }
    all += ")";
    std::cout << all + individual << std::endl;
}


int main(int argc, char* argv[])
{
    getSupportedFormats();
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("OpenGL");
    w.setStyle();
    w.show();
    return a.exec();
}