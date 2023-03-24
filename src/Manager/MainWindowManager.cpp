#include "MainWindowManager.h"

extern bool isBusy;

MainWindowManager::MainWindowManager(Ui::MainWindow* ui) : ui(ui),currentIndex(0)
{
	copyLightsModel(); 
	setStyle();
	readSettings();

	QThreadPool* global = QThreadPool::globalInstance();
	global->setMaxThreadCount(1); //设置线程池最多执行单线程

	QIcon icon(":icons/play.ico");
	ui->renderButton->setIcon(icon);
	ui->renderButton->setIconSize(QSize(30, 30));
	ui->renderButton->setFixedSize(20, 20); //设置按钮相关属性大小
	ui->renderWidget->hide();

	ui->stackedWidget->setCurrentIndex(1);
	ui->stackedWidget->setCurrentIndex(0);  
	QFont font("宋体",15, QFont::Bold);
	ui->fpslabel->setFont(font);
	//相关设置

	connect(&fpsTimer, &QTimer::timeout, this, &MainWindowManager::showSceneInfo);
	fpsTimer.setInterval(50);
	fpsTimer.start();
}

MainWindowManager::~MainWindowManager()
{
	ui = nullptr;
	saveSettings();
}

void MainWindowManager::bindSignals()
{
	connect(ui->saveSceneJson, &QAction::triggered, this, &MainWindowManager::saveScene);
	connect(ui->loadSceneJson, &QAction::triggered, this, &MainWindowManager::loadSceneFromAction);
	connect(ui->loadModel, &QAction::triggered, this, &MainWindowManager::loadModelFromAction);
	connect(ui->saveAsSceneJson, &QAction::triggered, this, &MainWindowManager::saveSceneAS);
	connect(ui->createScene, &QAction::triggered, this, &MainWindowManager::crateScene);
	connect(ui->closeScene, &QAction::triggered, this, &MainWindowManager::closeScene);
	auto& seceneManager = SceneManager::GetInstance();
	connect(seceneManager.data(), &SceneManager::updateList, ui->listWidget, &ModelListWidget::updateList); //关联更新信号

	connect(ui->listWidget, &ModelListWidget::sendNewname, seceneManager.data(), &SceneManager::rename);
	connect(ui->listWidget, &ModelListWidget::sendRemoveName, seceneManager.data(), &SceneManager::removeModelByName);
	connect(ui->listWidget, &ModelListWidget::sendCopyName, seceneManager.data(), &SceneManager::pasteByName);
	connect(ui->listWidget, &ModelListWidget::sendAddPath, this, &MainWindowManager::loadModel);
	connect(ui->listWidget, &ModelListWidget::itemDoubleClicked, ui->listWidget, &ModelListWidget::lookAt);
	connect(ui->listWidget, &ModelListWidget::itemClicked, ui->listWidget, &ModelListWidget::getSelectedName);
	connect(ui->listWidget, &ModelListWidget::sendSelectedName, seceneManager.data(), &SceneManager::getEditModel);
	connect(ui->listWidget, &ModelListWidget::sendLookAtName, seceneManager.data(), &SceneManager::lookAtModel);
	connect(ui->listWidget, &ModelListWidget::sendRevert, seceneManager.data(), &SceneManager::revertAction);

	connect(seceneManager.data(), &SceneManager::Info, ui->console, &Console::Info);
	connect(seceneManager.data(), &SceneManager::Error, ui->console, &Console::Error);
	connect(seceneManager.data(), &SceneManager::Clear, ui->console, &Console::Clear);

	auto& modelLoader = ModelLoader::GetInstance();
	connect(modelLoader.data(), &ModelLoader::Info, ui->console, &Console::Info);
	connect(modelLoader.data(), &ModelLoader::Error, ui->console, &Console::Error);
	connect(modelLoader.data(), &ModelLoader::Warning, ui->console, &Console::Warning);

	auto& dataBuilder = DataBuilder::GetInstance();
	dataBuilder->setModels(seceneManager->getModels());
	connect(dataBuilder.data(), &DataBuilder::Info, ui->console, &Console::Info);

	connect(ui->rayTracing, &RayTracingOpenGLWidget::Info, ui->console, &Console::Info);
	connect(ui->rayTracing, &RayTracingOpenGLWidget::SendHideRenderWidget, this, &MainWindowManager::hideRenderWidget);

	connect(ui->editor, &EditorOpenGLWidget::SendHideRenderWidget, this, &MainWindowManager::hideRenderWidget);

	connect(ui->renderButton, &QPushButton::clicked, this, &MainWindowManager::changeRenderWindow);

	connect(this, &MainWindowManager::sendCurrentIndex, ui->listWidget, &ModelListWidget::receiveIndex);

	connect(ui->console, &Console::anchorClicked, ui->console, &Console::openImage);
}

void MainWindowManager::loadModel(const QString& path)
{
	QString fileName;
	bool isShow;
	if (path.isEmpty()) {
		if (lastModelPath.isEmpty()) lastModelPath = QDir::currentPath();
		fileName = QFileDialog::getOpenFileName(nullptr, "打开模型文件", lastModelPath, tr("模型文件 (*.fbx *.obj *.3ds *.gltf *.blend)"));
		QFileInfo fileInfo(fileName);
		if (fileName.isEmpty()) {
			return;
		}
		lastModelPath = fileInfo.absolutePath();
	}
	else {
		fileName = path;
	}
	auto sceneManager = ui->editor->getSceneManager();
	auto editorRenderer = ui->editor->getEditorRenderer();
	editorRenderer->setSelected(nullptr);
	editorRenderer->getGizmo()->setEditModel(nullptr);
	if (sceneManager->getState() == NONE) {
		sceneManager->createScene();
	}
	if (path.contains("/lights/rectlight.obj") || path.contains("/lights/spherelight.obj")) {
		sceneManager->addModel(fileName, "", false, true); //路径为添加灯光
		isShow = false;
	}
	else {
		isShow = !sceneManager->getModels()->size();
		isBusy = true;
		sceneManager->addModel(fileName);
		isBusy = false;
	}
	if (currentIndex) {
		changeRenderWindow();
	}
	QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), isShow, true);
	ui->renderWidget->show();
}

void MainWindowManager::crateScene()
{
	auto sceneManager = ui->editor->getSceneManager();
	auto editorRenderer = ui->editor->getEditorRenderer();
	editorRenderer->setSelected(nullptr);
	editorRenderer->getGizmo()->setEditModel(nullptr);
	sceneManager->createScene();
	if (currentIndex) {
		changeRenderWindow();
	}
	ui->renderWidget->show();
	QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, true);
}

void MainWindowManager::loadScene(const QString& path)
{
	QString fileName;
	if (path.isEmpty()) {
		if (lastScenePath.isEmpty()) lastScenePath = QDir::currentPath();
		fileName = QFileDialog::getOpenFileName(nullptr, "打开场景文件", lastScenePath, tr("场景文件 (*.json)"));
	}
	else {
		fileName = path;
	}
	QFileInfo fileInfo(fileName);
	if (fileName.isEmpty()) {
		return;
	}
	lastScenePath = fileInfo.absolutePath();
	auto sceneManager = ui->editor->getSceneManager();
	auto editorRenderer = ui->editor->getEditorRenderer();
	editorRenderer->setSelected(nullptr);
	editorRenderer->getGizmo()->setEditModel(nullptr);
	isBusy = true;
	sceneManager->loadScene(fileName);
	if (currentIndex) {
		changeRenderWindow();
	}
	isBusy = false;
	QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(),true, true);
	ui->renderWidget->show();
}

void MainWindowManager::saveScene()
{
	auto sceneManager = ui->editor->getSceneManager();
	if (sceneManager->getState() == NONE) {
		QMessageBox::warning(ui->editor, "警告", "尚未打开场景！", QMessageBox::Yes);
		return;
	}
	sceneManager->saveScene();
}

void MainWindowManager::saveSceneAS()
{
	auto sceneManager = ui->editor->getSceneManager();
	if (sceneManager->getState() == NONE) {
		QMessageBox::warning(ui->editor, "警告", "尚未打开场景！", QMessageBox::Yes);
		return;
	}
	QString fileName = QFileDialog::getSaveFileName(ui->editor, "选择保存路径", QDir::currentPath(), "场景文件(*.json)");
	if (fileName.isEmpty()) {
		return;
	}
	sceneManager->saveSceneAs(fileName);
}


void MainWindowManager::closeScene()
{
	auto sceneManager = ui->editor->getSceneManager();
	if (sceneManager->getState() == NONE) return;
	sceneManager->closeScene();
	if (currentIndex) {
		changeRenderWindow();
	}
	ui->renderWidget->hide();
}

void MainWindowManager::setStyle(int style)
{
	QFile* f = nullptr;
	if (style == 0)
		f = new QFile(":/qss/flatgray.css");
	else if (style == 1)
		f = new QFile(":/qss/lightblue.css");
	else
		f = new QFile(":/qss/blacksoft.css");

	if (!f->exists()) {
		qDebug() << "Unable to set stylesheet, file not found\n";
	}
	else {
		f->open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(f);
		qApp->setStyleSheet(ts.readAll());
	}
	delete f;
}

void MainWindowManager::readSettings()
{
	if (!QFile::exists("settings.ini")) {
		ui->console->Warning("未找到ini文件，已采用默认配置！");
		return;
	} //不存在文件
	QSettings settings("settings.ini", QSettings::IniFormat);
	lastModelPath = settings.value("dir/model").toString();
	lastScenePath = settings.value("dir/scene").toString();
}

void MainWindowManager::saveSettings()
{
	QSettings settings("settings.ini", QSettings::IniFormat);
	settings.setValue("dir/model", lastModelPath);
	settings.setValue("dir/scene", lastScenePath);
}

void MainWindowManager::loadSceneFromAction()
{
	loadScene("");
}

void MainWindowManager::changeRenderWindow()
{
	if (ui->renderWidget->isHidden()) return;
	currentIndex = (currentIndex == 0) ? 1 : 0;
	ui->stackedWidget->setCurrentIndex(currentIndex);
	setButtonStyle(currentIndex);
	emit sendCurrentIndex(currentIndex);
}

void MainWindowManager::hideRenderWidget(bool isHide)
{
	if (isHide) ui->renderWidget->hide();
	else ui->renderWidget->show();
}

void MainWindowManager::copyLightsModel()
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

void MainWindowManager::showSceneInfo()
{
	auto sceneManager = ui->editor->getSceneManager();
	ui->groupBox->setTitle(sceneManager->getSceneName());
	if (ui->renderWidget->isHidden()) {
		ui->fpslabel->setText("");
		return;
	}
	int fps = 0;
	if (currentIndex) fps = ui->rayTracing->getFPS();
	else fps = ui->editor->getFPS();
	if (fps) {
		ui->fpslabel->setText("FPS:" + QString::number(fps));
	}
}

void MainWindowManager::setButtonStyle(int index)
{
	if (index) {
		QIcon icon(":icons/pause.ico");
		ui->renderButton->setIcon(icon);
		ui->rayTracing->getRayTracingRenderer()->clearFrameCounter(); //清除帧计数器
		ui->rayTracing->setFocus();
	}
	else {
		QIcon icon(":icons/play.ico");
		ui->renderButton->setIcon(icon);
		ui->editor->setFocus();
	}
}


void MainWindowManager::loadModelFromAction() 
{
	loadModel("");
}

