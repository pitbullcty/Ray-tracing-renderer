#ifndef __SCENE_MANAGER__
#define __SCENE_MANAGER__

#include "ModelLoader.h"
#include "Camera.h"
#include "Skybox.h"
#include "DataBuilder.h"
#include "Gizmo.h"
#include <QFile>
#include <QRegularExpression>
#include <QJsonArray>
#include <QJsonDocument>
#include <QOpenGLExtraFunctions>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QMessagebox>
#include <QIcon>
#include <QtConcurrent>
#include <QStack>

const int MAXSIZE = 1000; //默认保存1000次操作

enum STATE {
	NONE,
	CREATED,
}; //保存编辑状态 

enum ACITIONTYPE {
	ADD,
	REMOVE,
	RENAME,
	MOVEPOS
}; //记录还原需要的操作类型

class SceneManager :public QObject
{
	Q_OBJECT

public:
	static QSharedPointer<SceneManager>& GetInstance();
	static void destory(SceneManager* sceneManager);

	Model* getSelected(int posx, int posy);
	void setSize(int width, int height);

	void loadScene(const QString& path); //读取场景
	bool saveScene(); //保存场景
	void saveSceneAs(const QString& path);
	void createScene(); //创建场景
	void closeScene(); //关闭场景
	bool closeApp(); //关闭app
	
	QSharedPointer<Camera> getCamera();
	QMap<QString, Model>* getModels(); //返回模型指针
	STATE getState();
	QString getSceneName();

	void clearModels();
	void copyModel(Model* model);
	void removeModel(Model* model);
	void pasteModel(QPoint pos);
	void pasteModel(QVector3D pos);
	void copyModel(const QString& name);
	void lookAtModel(const QString& name);
	void addRevertModel(const ACITIONTYPE& action, const Model& model, const QString& name);

signals:
	void updateList(QMap<QString, Model>* models, Model* model);
	void Info(QString info);
	void Error(QString error);
	void Clear();
	void sendEditModel(Model* model);

public slots:
	Model* removeModelByName(const QString& name);
	void pasteByName(const QString& name);
	void rename(const QString& oldname, const QString& newname);
	QString addModel(const QString& path, const QString& modelName = "", bool isCopy = false, bool isLight = false, bool isLoadScene=false);
	void getEditModel(const QString& name);
	void revertAction(); //撤回

private:

	QSharedPointer<ModelLoader> modelLoader; //模型加载器
	QMap<QString, Model> models; //模型
	QSharedPointer<Camera> camera; //摄像机
	QSharedPointer<Skybox> skybox; //天空盒
	QString sceneFileName; //场景文件名
	QString sceneName; //场景名
	QStack<QPair<QPair<ACITIONTYPE, Model>,QString>> revertActions;
	STATE state; //当前编辑状态

	int width; 
	int height; //窗口大小

	Model* modelToCopy;

	static QSharedPointer<SceneManager> instance;
	SceneManager();
	~SceneManager() = default;

	QJsonObject toJsonObeject();
	bool dealDifference();

};

#endif 

