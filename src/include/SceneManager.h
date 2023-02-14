#ifndef __SCENE_MANAGER__
#define __SCENE_MANAGER__

#include "ModelLoader.h"
#include "Camera.h"
#include "Skybox.h"
#include "Gizmo.h"
#include <QFile>
#include <QRegularExpression>
#include <QJsonArray>
#include <QJsonDocument>
#include <QOpenGLExtraFunctions>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QMessagebox>

enum STATE {
	NONE,
	CREATED,
}; //保存编辑状态 

class SceneManager
{
public:
	static QSharedPointer<SceneManager>& GetInstance();
	static void destory(SceneManager* sceneManager);

	void addModel(const QString& path, const QString& modelName="");
	void clearModel(const QString& name);
	void clearModels();

	void loadScene(const QString& path); //读取场景
	bool saveScene(); //保存场景
	void saveSceneAs(const QString& path);
	void createScene(); //创建场景
	void closeScene(); //关闭场景
	bool closeApp(); //关闭app

	QSharedPointer<Camera> getCamera();
	QMap<QString, Model>* getModels(); //返回模型指针
	STATE getState();


private:

	QSharedPointer<ModelLoader> modelLoader; //模型加载器

	QMap<QString, Model> models; //模型
	QSharedPointer<Camera> camera; //摄像机
	QSharedPointer<Skybox> skybox; //天空盒
	QString sceneFileName; //场景文件名
	STATE state; //当前编辑状态

	static QSharedPointer<SceneManager> instance;
	SceneManager();
	~SceneManager() = default;

	QJsonObject toJsonObeject();
	bool dealDifference();

};

#endif 

