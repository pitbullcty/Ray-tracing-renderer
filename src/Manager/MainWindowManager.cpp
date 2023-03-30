#include "MainWindowManager.h"

extern bool isBusy;

MainWindowManager::MainWindowManager(Ui::MainWindow* ui) : 
	ui(ui),
	transformInspector(new TransformInspector),
	materialInspector(new MaterialInspector),
	renderOptionInspector(new RenderOptionInspector),
	skyboxInspector(new SkyboxInspector),
	currentIndex(0)
{
	copyLightsModel(); 
	setStyle(0);
	readSettings();
	setUi();
}

void MainWindowManager::setUi()
{

	QThreadPool* global = QThreadPool::globalInstance();
	global->setMaxThreadCount(1); //设置线程池最多执行单线程

	QIcon icon(":icons/play.ico");
	ui->renderButton->setIcon(icon);
	ui->renderButton->setIconSize(QSize(30, 30));
	ui->renderButton->setFixedSize(20, 20); //设置按钮相关属性大小
	ui->renderWidget->hide();

	ui->actionNew->setIcon(QIcon(":/icons/new.ico"));
	ui->actionSave->setIcon(QIcon(":/icons/save.ico"));
	ui->actionOpen->setIcon(QIcon(":/icons/open.ico"));
	ui->toolBar->setIconSize(QSize(30, 30));

	ui->stackedWidget->setCurrentIndex(1);
	ui->stackedWidget->setCurrentIndex(0);

	//相关设置
	ui->inspector->addWidget("天空盒", skyboxInspector);
	ui->inspector->addWidget("变换", transformInspector);
	ui->inspector->addWidget("材质", materialInspector);

	ui->inspector->addWidget("渲染设置", renderOptionInspector);
	ui->inspector->hideWidget(3);
	ui->inspector->setEnabled(false); //设置不可展开

	for (auto& sceneFile : sceneList) {
		QAction* action = new QAction(sceneFile, ui->menuClear);
		connect(action, &QAction::triggered, [=]() {
			loadScene(sceneFile);
		});
		actions.push_back(action);
	}
	ui->menuClear->insertActions(ui->actionClear, actions); //插入
}

MainWindowManager::~MainWindowManager()
{
	ui = nullptr;
	transformInspector = nullptr;
	materialInspector = nullptr;
	skyboxInspector = nullptr;
	renderOptionInspector = nullptr;
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

	connect(ui->actionNew, &QAction::triggered, this, &MainWindowManager::crateScene);
	connect(ui->actionOpen, &QAction::triggered, this, &MainWindowManager::loadSceneFromAction);
	connect(ui->actionSave, &QAction::triggered, this, &MainWindowManager::saveScene);

	connect(ui->actionBlue, &QAction::triggered, [=]() {
		setStyle(1); 	
		ui->renderButton->setIconSize(QSize(30, 30));
		ui->renderButton->setFixedSize(20, 20); //设置按钮相关属性大小
	});
	connect(ui->actionGray, &QAction::triggered, [=]() {
		setStyle(0);
		ui->renderButton->setIconSize(QSize(30, 30));
		ui->renderButton->setFixedSize(20, 20); //设置按钮相关属性大小
	});
	connect(ui->actionDark, &QAction::triggered, [=]() {
		setStyle(2);
		ui->renderButton->setIconSize(QSize(30, 30));
		ui->renderButton->setFixedSize(20, 20); //设置按钮相关属性大小
	});

	connect(ui->actionConsole, &QAction::triggered, [=]() {
		if (ui->dockWidgetConsole->isHidden()) ui->dockWidgetConsole->show();
	});

	connect(ui->actionModel, &QAction::triggered, [=]() {
		if (ui->dockWidgetInfos->isHidden()) ui->dockWidgetInfos->show();
	});

	connect(ui->actionInspector, &QAction::triggered, [=]() {
		if (ui->dockWidgetDetails->isHidden()) ui->dockWidgetDetails->show();
	});

	connect(ui->actionClear, &QAction::triggered, [=]() {
		sceneList.clear();
		for (auto& action : actions) {
			ui->menuClear->removeAction(action);
		}
		actions.clear();
	});

	connect(ui->actionHelp, &QAction::triggered, [=]() {
		auto helpDialog = new HelpDialog();
		helpDialog->exec();
		delete helpDialog;
	});

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
	connect(seceneManager.data(), &SceneManager::sendSceneName, this, &MainWindowManager::showSceneName);
	connect(seceneManager.data(), &SceneManager::sendInspectorModel, transformInspector, &TransformInspector::bindModel);
	connect(seceneManager.data(), &SceneManager::sendInspectorModel, materialInspector, &MaterialInspector::bindModel);
	connect(seceneManager.data(), &SceneManager::sendInspectorName, ui->inspector, &Inspector::setModelName);

	auto& modelLoader = ModelLoader::GetInstance();
	connect(modelLoader.data(), &ModelLoader::Info, ui->console, &Console::Info);
	connect(modelLoader.data(), &ModelLoader::Error, ui->console, &Console::Error);
	connect(modelLoader.data(), &ModelLoader::Warning, ui->console, &Console::Warning);

	auto& dataBuilder = DataBuilder::GetInstance();
	dataBuilder->setModels(seceneManager->getModels());
	connect(dataBuilder.data(), &DataBuilder::Info, ui->console, &Console::Info);

	connect(ui->rayTracing, &RayTracingOpenGLWidget::Info, ui->console, &Console::Info);
	connect(ui->rayTracing, &RayTracingOpenGLWidget::sendHideRenderWidget, this, &MainWindowManager::hideRenderWidget);
	connect(ui->rayTracing, &RayTracingOpenGLWidget::sendFPS, this, &MainWindowManager::showFPS);

	connect(ui->editor, &EditorOpenGLWidget::sendHideRenderWidget, this, &MainWindowManager::hideRenderWidget);
	connect(ui->editor, &EditorOpenGLWidget::sendFPS, this, &MainWindowManager::showFPS);

	auto gizmo = ui->editor->getEditorRenderer()->getGizmo();
	connect(transformInspector, &TransformInspector::sendEditModel, gizmo.data(), &Gizmo::setEditModel);
	connect(transformInspector, &TransformInspector::sendGizmoType, gizmo.data(), &Gizmo::setType);
	connect(transformInspector, &TransformInspector::sendLocationType, gizmo.data(), &Gizmo::setLocation);
	connect(gizmo.data(), &Gizmo::sendChanged, transformInspector, &TransformInspector::setData);
	connect(gizmo.data(), &Gizmo::sendCheckedLocationButton, transformInspector, &TransformInspector::setCheckedLocationButton);
	connect(gizmo.data(), &Gizmo::sendCheckedTransformButton, transformInspector, &TransformInspector::setCheckedTransformButton);

	connect(transformInspector, &TransformInspector::sendRevertModel, seceneManager.data(), &SceneManager::addInspectorRevertModel);
	connect(materialInspector, &MaterialInspector::sendRevertModel, seceneManager.data(), &SceneManager::addInspectorRevertModel);
	

	auto rayTarcingRenderer = ui->rayTracing->getRayTracingRenderer();
	connect(renderOptionInspector, &RenderOptionInspector::sendRenderOption, rayTarcingRenderer.data(), &RayTracingRenderer::setRenderOption);
	connect(rayTarcingRenderer.data(), &RayTracingRenderer::Info, ui->console, &Console::Info);

	auto editorRenderer = ui->editor->getEditorRenderer();
	connect(skyboxInspector, &SkyboxInspector::sendUpdateSkybox, rayTarcingRenderer.data(), &RayTracingRenderer::initSkyboxTexture);
	connect(skyboxInspector, &SkyboxInspector::sendUpdateSkybox, editorRenderer.data(), &EditorRenderer::initSkyboxTexture);

	auto skybox = seceneManager->getSkybox();
	connect(skybox.data(), &Skybox::sendUpdateSkybox, rayTarcingRenderer.data(), &RayTracingRenderer::initSkyboxTexture);
	connect(skybox.data(), &Skybox::sendUpdateSkybox, editorRenderer.data(), &EditorRenderer::initSkyboxTexture);

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
		ui->inspector->setModelName("");
		renderOptionInspector->reset();
		sceneManager->getSkybox()->reset();
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
	ui->editor->update(); //刷新界面显示
	QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), isShow, true, true);
	ui->renderWidget->show();
}

void MainWindowManager::crateScene()
{
	auto sceneManager = ui->editor->getSceneManager();
	auto editorRenderer = ui->editor->getEditorRenderer();
	editorRenderer->setSelected(nullptr);
	editorRenderer->getGizmo()->setEditModel(nullptr);
	sceneManager->createScene();
	ui->inspector->setModelName("");
	renderOptionInspector->reset();
	sceneManager->getSkybox()->reset();
	ui->editor->update(); //刷新界面显示
	if (currentIndex) {
		changeRenderWindow();
	}
	ui->renderWidget->show();
	QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, true, true);
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

	if (fileName.isEmpty()) {
		return;
	}

	if (sceneList.indexOf(fileName) == -1) {
		if (sceneList.size() >= MAXSCENEFILESIZE) sceneList.pop(); //最多5个不重复
		sceneList.push(fileName);
	}

	QFileInfo fileInfo(fileName);
	lastScenePath = fileInfo.absolutePath();
	auto sceneManager = ui->editor->getSceneManager();
	auto editorRenderer = ui->editor->getEditorRenderer();
	editorRenderer->setSelected(nullptr);
	editorRenderer->getGizmo()->setEditModel(nullptr);
	isBusy = true;
	sceneManager->loadScene(fileName);
	renderOptionInspector->reset();
	if (currentIndex) {
		changeRenderWindow();
	}
	isBusy = false;
	ui->editor->update(); //刷新界面显示
	QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), true, true, true);
	ui->inspector->setModelName("");
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
	ui->inspector->collapseAll(true);
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
		return;
	} //不存在文件
	QSettings settings("settings.ini", QSettings::IniFormat);

	lastModelPath = settings.value("dir/lastModelPath").toString();
	lastScenePath = settings.value("dir/lastScenePath").toString();

	int size = settings.beginReadArray("recentFiles");
	for (int i = 0; i < size; ++i) {
		settings.setArrayIndex(i);
		QString path("scene%1");
		sceneList.push(settings.value(path.arg(QString::number(i))).toString());
	}
	settings.endArray();

}

void MainWindowManager::saveSettings()
{
	if (lastModelPath.isEmpty() && lastScenePath.isEmpty()) return;

	QSettings settings("settings.ini", QSettings::IniFormat);

	settings.setValue("dir/lastModelPath", lastModelPath);
	settings.setValue("dir/lastScenePath", lastScenePath);

	settings.beginWriteArray("recentFiles");
	int count = 0;
	for (auto& sceneFile : sceneList) {
		settings.setArrayIndex(count);
		QString path("scene%1");
		settings.setValue(path.arg(QString::number(count++)), sceneFile);
	}
	settings.endArray();
}

void MainWindowManager::showFPS(int fps)
{
	if (fps) {
		QString text("<font size=\"5\"><b>FPS:%1</b></font>");
		ui->fpslabel->setText(text.arg(QString::number(fps)));
	}
}

void MainWindowManager::showSceneName(const QString& sceneName)
{
	ui->groupBox->setTitle(sceneName);
}

void MainWindowManager::loadSceneFromAction()
{
	loadScene("");
}

void MainWindowManager::changeRenderWindow()
{
	if (ui->renderWidget->isHidden()) return;
	ui->inspector->changeCurrentIndex(currentIndex);
	currentIndex = (currentIndex == 0) ? 1 : 0;
	ui->stackedWidget->setCurrentIndex(currentIndex);
	setButtonStyle(currentIndex);
	if (currentIndex == 0) {
		auto rayTracingRender = ui->rayTracing->getRayTracingRenderer();
		rayTracingRender->stopRender(); //停止渲染
	};
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

