#include "Mainwindow.h"
#include <QFile>

extern bool isBusy;

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    actions(ui),
    currentIndex(0)
{
	ui->setupUi(this);
    this->setWindowIcon(QIcon(":icons/title.ico"));
    this->setWindowTitle("光线追踪渲染器");
    this->setStyle();
    this->showMaximized();
    this->setAcceptDrops(true); //接受拖拽
    QThreadPool* global = QThreadPool::globalInstance();
    global->setMaxThreadCount(1); //设置线程池最多执行单线程
    actions.bind(); //绑定actions
    bindSignals(); //绑定其他信号
    copyLightsModel();
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget->setCurrentIndex(0);
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
    if (ui->editor->closeApp()) {
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
            actions.loadModel(filename);
        }
        else if (filename.contains(".json")) {
            actions.loadScene(filename);
        }
        else;
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    Qt::Key key = (Qt::Key)(event->key());
    if (key == Qt::Key_F5) {
        currentIndex = (currentIndex == 0) ? 1 : 0;
        ui->stackedWidget->setCurrentIndex(currentIndex);
        if (currentIndex) ui->rayTracing->getRayTracingRenderer()->clearFrameCounter(); //清除帧计数器
    }
    else;
}

void MainWindow::bindSignals()
{
    auto& seceneManager = SceneManager::GetInstance();
    connect(seceneManager.data(), &SceneManager::updateList, ui->listWidget, &ModelListWidget::updateList); //关联更新信号
    connect(ui->listWidget, &ModelListWidget::sendNewname, seceneManager.data(), &SceneManager::rename);
    connect(ui->listWidget, &ModelListWidget::sendRemoveName, seceneManager.data(), &SceneManager::removeModelByName);
    connect(ui->listWidget, &ModelListWidget::sendCopyName, seceneManager.data(), &SceneManager::pasteByName);
    connect(ui->listWidget, &ModelListWidget::sendAddPath, &actions, &WindowActions::loadModel);
    connect(ui->listWidget, &ModelListWidget::itemDoubleClicked, ui->listWidget, &ModelListWidget::lookAt);
    connect(ui->listWidget, &ModelListWidget::itemClicked, ui->listWidget, &ModelListWidget::getSelectedName);
    connect(ui->listWidget, &ModelListWidget::sendSelectedName, seceneManager.data(), &SceneManager::getEditModel);
    connect(ui->listWidget, &ModelListWidget::sendLookAtName, seceneManager.data(), &SceneManager::lookAtModel);
    connect(ui->listWidget, &ModelListWidget::sendRevert, seceneManager.data(), &SceneManager::revertAction);

    connect(seceneManager.data(), &SceneManager::Info, ui->console, &Console::Info);
    connect(seceneManager.data(), &SceneManager::Error,ui->console, &Console::Error);
    connect(seceneManager.data(), &SceneManager::Clear,ui->console, &Console::Clear);

    auto& modelLoader = ModelLoader::GetInstance();
    connect(modelLoader.data(), &ModelLoader::Info, ui->console, &Console::Info);
    connect(modelLoader.data(), &ModelLoader::Error, ui->console, &Console::Error);
    connect(modelLoader.data(), &ModelLoader::Warning, ui->console, &Console::Warning);

    auto& dataBuilder = DataBuilder::GetInstance();
    dataBuilder->setModels(seceneManager->getModels());
    connect(dataBuilder.data(), &DataBuilder::Info, ui->console, &Console::Info);

    connect(ui->closeWindow, &QAction::triggered, this, &MainWindow::close);
    
}

void MainWindow::copyLightsModel()
{
    QDir dir;
    QString lightPath(QDir::currentPath() + "/lights");
    if (!dir.exists(lightPath)) {
        dir.mkdir(lightPath);
    }

    QString spherePath(lightPath + "/spherelight.obj");
    QString rectPath(lightPath + "/rectlight.obj");

    if (!QFile::exists(spherePath))
        QFile::copy(":/light/spherelight.obj", spherePath);

    if (!QFile::exists(rectPath))
        QFile::copy(":/light/rectlight.obj", rectPath);

}

