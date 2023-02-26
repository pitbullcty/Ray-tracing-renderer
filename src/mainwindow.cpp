#include "Mainwindow.h"
#include <QFile>

extern bool isBusy;

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent),ui(new Ui::MainWindow),actions(WindowActions(ui))
{
	ui->setupUi(this);
    this->setWindowIcon(QIcon(":icons/title.ico"));
    this->setWindowTitle("光线追踪渲染器");
    this->setStyle();
    this->showMaximized();
    actions.bind(); //绑定actions
    bindSignals(); //绑定其他信号
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

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (isBusy) {
        event->ignore();
        return;
    }
    if (ui->openGLWidget->closeApp()) {
        event->accept();
    }
    else {
        event->ignore();
    }
}

void MainWindow::bindSignals()
{
    auto& seceneManager = SceneManager::GetInstance();
    connect(seceneManager.data(), &SceneManager::updateList, ui->listWidget, &ModelListWidget::updateList); //关联更新信号
    connect(ui->listWidget, &ModelListWidget::sendNewname, seceneManager.data(), &SceneManager::rename);
    connect(ui->listWidget, &ModelListWidget::sendRemoveName, seceneManager.data(), &SceneManager::removeModelByName);
    connect(ui->listWidget, &ModelListWidget::sendCopyName, seceneManager.data(), &SceneManager::pasteByName);
    connect(ui->listWidget, &ModelListWidget::sendAddPath, &actions, &WindowActions::loadModel);
    connect(ui->listWidget, &ModelListWidget::itemClicked, ui->listWidget, &ModelListWidget::lookAt);
    connect(ui->listWidget, &ModelListWidget::sendLookAtName, seceneManager.data(), &SceneManager::lookAtModel);

    connect(seceneManager.data(), &SceneManager::Info, ui->console, &Console::Info);
    connect(seceneManager.data(), &SceneManager::Error,ui->console, &Console::Error);
    connect(seceneManager.data(), &SceneManager::Clear,ui->console, &Console::Clear);

    auto& modelLoader = ModelLoader::GetInstance();
    connect(modelLoader.data(), &ModelLoader::Info, ui->console, &Console::Info);
    connect(modelLoader.data(), &ModelLoader::Error, ui->console, &Console::Error);
    connect(modelLoader.data(), &ModelLoader::Warning, ui->console, &Console::Warning);

    auto& rayTracingRender = RayTracingRender::GetInstance();
    connect(rayTracingRender.data(), &RayTracingRender::Info, ui->console, &Console::Info);

    connect(ui->closeWindow, &QAction::triggered, this, &MainWindow::close);
    
}



