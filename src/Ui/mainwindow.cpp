#include "Mainwindow.h"


extern bool isBusy;

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
  
    this->setWindowIcon(QIcon(":icons/title.ico"));
    this->setWindowTitle("光线追踪渲染器");
    this->showMaximized();
    this->setAcceptDrops(true); //接受拖拽

    mainWindowManager = new MainWindowManager(ui);
    mainWindowManager->bindSignals(); //绑定窗口相关信号
    this->bindSignals(); //绑定其他信号

}

MainWindow::~MainWindow()//析构函数，关掉ＵＩ界面
{
    delete mainWindowManager;
	delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (isBusy) {
        event->ignore();
        return;
    }
    if (ui->editor->closeApp()) {
        ui->editor->destoryData();
        ui->rayTracing->destoryData();
        event->accept();
    }
    else {
        event->ignore();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
    else {
        event->ignore();
    }
 
}

void MainWindow::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        auto urlList = mimeData->urls();
        QString filename = urlList.at(0).toLocalFile(); 
        if (filename.contains(".obj") || filename.contains(".fbx") || filename.contains(".gltf")) {
            mainWindowManager->loadModel(filename);
        }
        else if (filename.contains(".json")) {
            mainWindowManager->loadScene(filename);
        }
        else;
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    Qt::Key key = (Qt::Key)(event->key());
    if (key == Qt::Key_F5) {
        emit sendChangeWindow();
    }
    else;
}

void MainWindow::bindSignals()
{
    connect(ui->closeWindow, &QAction::triggered, this, &MainWindow::close);
    connect(this, &MainWindow::sendChangeWindow, mainWindowManager, &MainWindowManager::changeRenderWindow);
}