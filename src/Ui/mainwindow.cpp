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
    
    layoutData = saveState();
    geometryData = saveGeometry();
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
        for (auto& url : urlList) {
            QString filename = url.toLocalFile();
            if (filename.contains(".obj") || filename.contains(".fbx") || filename.contains(".gltf")) {
                mainWindowManager->loadModel(filename);
            }
            else if (filename.contains(".json")) {
                mainWindowManager->loadScene(filename);
            }
            else;
        }
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
    connect(ui->actionRestore, &QAction::triggered, [=]() {
        if (ui->dockWidgetConsole->isHidden()) ui->dockWidgetConsole->show();
        if (ui->dockWidgetInfos->isHidden()) ui->dockWidgetInfos->show();
        if (ui->dockWidgetDetails->isHidden()) ui->dockWidgetDetails->show();
        restoreState(layoutData);
        restoreGeometry(geometryData);
    });
}

void QAbstractSpinBox::wheelEvent(QWheelEvent* e) {
    e->ignore();
} //屏蔽滚轮事件

void QAbstractSlider::wheelEvent(QWheelEvent* e) {
    e->ignore();
} //屏蔽滚轮事件

void QDockWidget::closeEvent(QCloseEvent* e) {
    this->hide();
} //关闭事件修改为隐藏