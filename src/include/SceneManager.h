#ifndef __SCENE_MANAGER__
#define __SCENE_MANAGER__

#include "ModelLoader.h"
#include "Camera.h"
#include "Skybox.h"
#include "Gizmo.h"

class SceneManager
{
public:
	static QSharedPointer<SceneManager>& GetInstance();
	static void destory(SceneManager* sceneManager);

	void addModel(const QString& path);
	void clearModel(const QString& name);
	void clearModels();

	QSharedPointer<Camera> getCamera();
	QMap<QString, Model>* getModels();

	void loadScene();
	void saveScene();

private:

	QSharedPointer<ModelLoader> modelLoader; //模型加载器

	QMap<QString, Model> models; //模型
	QSharedPointer<Camera> camera; //摄像机
	QSharedPointer<Skybox> skybox; //天空盒

	static QSharedPointer<SceneManager> instance;
	SceneManager();
	~SceneManager() = default;
};

#endif 

