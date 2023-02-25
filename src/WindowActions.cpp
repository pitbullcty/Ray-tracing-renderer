#include "WindowActions.h"

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
	connect(ui->loadSceneJson, &QAction::triggered, this, &WindowActions::loadScene);
	connect(ui->loadModel, &QAction::triggered, this, &WindowActions::loadModel);
	connect(ui->saveAsSceneJson, &QAction::triggered, this, &WindowActions::saveSceneAS);
	connect(ui->createScene, &QAction::triggered, this, &WindowActions::crateScene);
	connect(ui->closeScene, &QAction::triggered, this, &WindowActions::closeScene);
	
}

void WindowActions::loadModel()
{
	QString fileName = QFileDialog::getOpenFileName(nullptr, "打开模型文件", QDir::currentPath(), tr("模型文件 (*.fbx *.obj *.3ds *.gltf *.blend)"));
	if (fileName.isEmpty()) {
		return;
	}
	auto sceneManager = ui->openGLWidget->getSceneManager();
	auto editorRenderer = ui->openGLWidget->getEditorRenderer();
	editorRenderer->setSelected(nullptr);
	editorRenderer->getGizmo()->setEditModel(nullptr);
	if (sceneManager->getState() == NONE) {
		sceneManager->createScene();
	}
	sceneManager->addModel(fileName);
	auto buildTask = ui->openGLWidget->getTask();
	if (buildTask->isRunning()) {
		buildTask->waitForFinished();
	}
	*buildTask = QtConcurrent::run(&RayTracingRender::buildBVH, RayTracingRender::GetInstance().data());

}

void WindowActions::crateScene()
{
	auto sceneManager = ui->openGLWidget->getSceneManager();
	auto editorRenderer = ui->openGLWidget->getEditorRenderer();
	editorRenderer->setSelected(nullptr);
	editorRenderer->getGizmo()->setEditModel(nullptr);
	sceneManager->createScene();
}

void WindowActions::loadScene()
{
	QString fileName = QFileDialog::getOpenFileName(nullptr, "打开场景文件", QDir::currentPath(), tr("场景文件 (*.json)"));
	if (fileName.isEmpty()) {
		return;
	}
	auto sceneManager = ui->openGLWidget->getSceneManager();
	auto editorRenderer = ui->openGLWidget->getEditorRenderer();
	editorRenderer->setSelected(nullptr);
	editorRenderer->getGizmo()->setEditModel(nullptr);
	sceneManager->loadScene(fileName);
	auto buildTask = ui->openGLWidget->getTask();
	if (buildTask->isRunning()) {
		buildTask->waitForFinished();
	}
	*buildTask = QtConcurrent::run(&RayTracingRender::buildBVH, RayTracingRender::GetInstance().data());
}

void WindowActions::saveScene()
{
	auto sceneManager = ui->openGLWidget->getSceneManager();
	if (sceneManager->getState() == NONE) {
		QMessageBox::warning(nullptr, "警告", "尚未打开场景！", QMessageBox::Yes);
		return;
	}
	sceneManager->saveScene();
}

void WindowActions::saveSceneAS()
{
	auto sceneManager = ui->openGLWidget->getSceneManager();
	if (sceneManager->getState() == NONE) {
		QMessageBox::warning(nullptr, "警告", "尚未打开场景！", QMessageBox::Yes);
		return;
	}
	QString fileName = QFileDialog::getSaveFileName(nullptr, "选择保存路径", QDir::currentPath(), "场景文件(*.json)");
	if (fileName.isEmpty()) {
		return;
	}
	sceneManager->saveSceneAs(fileName);
}


void WindowActions::closeScene()
{
	auto sceneManager = ui->openGLWidget->getSceneManager();
	if (sceneManager->getState() == NONE) return;
	sceneManager->closeScene();
}


