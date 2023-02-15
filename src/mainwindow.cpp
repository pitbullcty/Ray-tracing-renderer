#include "Mainwindow.h"
#include <QFile>

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent),ui(new Ui::MainWindow),actions(WindowActions(ui))
{
	ui->setupUi(this);
    this->setWindowIcon(QIcon(":icons/title.ico"));
    this->setWindowTitle("光线追踪渲染器");
    this->setStyle();
    this->showMaximized();
    Console::setConsole(ui->console); //加载控制台
    connect(ui->openGLWidget,SIGNAL(sendCloseSignal(int)),this,SLOT(receiveCloseSignal(int))); //绑定子窗口信号
    actions.bind(); //绑定actions
}

MainWindow::~MainWindow()//析构函数，关掉ＵＩ界面
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
        qDebug() << "Unable to set stylesheet, file not found\n" ;
    }
    else {
        f->open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(f);
        qApp->setStyleSheet(ts.readAll());
    }
    delete f;
}

void MainWindow::receiveCloseSignal(int signal) {
    if (signal) this->close();
}
