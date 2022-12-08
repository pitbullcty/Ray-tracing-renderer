#include "mainwindow.h"
#include <QFile>

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent),ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()//�����������ص��գɽ���
{
	delete ui;
}

void MainWindow::setStyle(int style)
{
    QFile* f = nullptr;
    if (style)
        f = new QFile(":/qdarkstyle/dark/darkstyle.qss");
    else
        f = new QFile(":/qdarkstyle/light/lightstyle.qss");

    if (!f->exists()) {
        printf("Unable to set stylesheet, file not found\n");
    }
    else {
        f->open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(f);
        qApp->setStyleSheet(ts.readAll());
    }
    delete f;
}
