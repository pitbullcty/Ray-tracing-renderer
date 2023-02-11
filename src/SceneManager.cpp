#include "SceneManager.h"

QSharedPointer<SceneManager> SceneManager::instance = nullptr;

QSharedPointer<SceneManager>& SceneManager::GetInstance()
{
	if (instance.isNull())
		instance = QSharedPointer<SceneManager>(new SceneManager(), SceneManager::destory);
	return instance;
}

void SceneManager::destory(SceneManager* sceneManager)
{

	delete sceneManager;
}

void SceneManager::addModel(const QString& path)
{
	QString pathSep = path.right(path.size() - path.lastIndexOf('/') - 1);
	QString name = pathSep.left(pathSep.lastIndexOf('.'));
	models[name] = modelLoader->loadModel(path);
}

void SceneManager::clearModel(const QString& name)
{
	models[name].destroyTextures();
	models.remove(name);
}

void SceneManager::clearModels()
{
	for (auto it = models.begin(); it != models.end(); it++) {
		it->destroyTextures();
	}
	models.clear();
}

QSharedPointer<Camera> SceneManager::getCamera()
{
	return camera;
}

QMap<QString, Model>* SceneManager::getModels()
{
	return &models;
}

void SceneManager::loadScene()
{
}

void SceneManager::saveScene()
{
}

SceneManager::SceneManager()
{
	modelLoader = ModelLoader::GetInstance();
	camera = Camera::GetInstance();
	skybox = Skybox::GetInstance();
}
