#include "WindowActions.h"

extern bool isBusy;

WindowActions::WindowActions(Ui::MainWindow* ui) : ui(ui)
{
}

WindowActions::~WindowActions()
{
	ui = nullptr;
}

void WindowActions::bind()
{
	connect(ui->saveSceneJson, &QAction::triggered, this, &WindowActions::saveScene);
	connect(ui->loadSceneJson, &QAction::triggered, this, &WindowActions::loadSceneFromAction);
	connect(ui->loadModel, &QAction::triggered, this, &WindowActions::loadModelFromAction);
	connect(ui->saveAsSceneJson, &QAction::triggered, this, &WindowActions::saveSceneAS);
	connect(ui->createScene, &QAction::triggered, this, &WindowActions::crateScene);
	connect(ui->closeScene, &QAction::triggered, this, &WindowActions::closeScene);
	
}

void WindowActions::loadModel(const QString& path)
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
	QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), isShow);
	
}

void WindowActions::crateScene()
{
	auto sceneManager = ui->editor->getSceneManager();
	auto editorRenderer = ui->editor->getEditorRenderer();
	editorRenderer->setSelected(nullptr);
	editorRenderer->getGizmo()->setEditModel(nullptr);
	sceneManager->createScene();
}

void WindowActions::loadScene(const QString& path)
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
	isBusy = false;
	QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(),true);
}

void WindowActions::saveScene()
{
	auto sceneManager = ui->editor->getSceneManager();
	if (sceneManager->getState() == NONE) {
		QMessageBox::warning(ui->editor, "警告", "尚未打开场景！", QMessageBox::Yes);
		return;
	}
	sceneManager->saveScene();
}

void WindowActions::saveSceneAS()
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


void WindowActions::closeScene()
{
	auto sceneManager = ui->editor->getSceneManager();
	if (sceneManager->getState() == NONE) return;
	sceneManager->closeScene();
}

void WindowActions::loadSceneFromAction()
{
	loadScene("");
}


void WindowActions::loadModelFromAction() 
{
	loadModel("");
}

