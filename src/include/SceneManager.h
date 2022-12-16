#ifndef __SCENE_MANAGER__
#define __SCENE_MANAGER__

#include"Model.h"
#include "Camera.h"
#include <QMap>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

//使用单例模式
class SceneManager {
public:
	void addModel(QString name, Model model); //添加模型
	void renderModels();
	void destoryTexture();

	static void destory(SceneManager* sceneManager);
	
	static QSharedPointer<SceneManager>& GetInstance(QOpenGLShaderProgram* _shaderProgram, QOpenGLFunctions* _functions);

private:
	static QSharedPointer<SceneManager> instance;

	//OpenGl上下文相关
	QOpenGLShaderProgram* shaderProgram;
	QOpenGLFunctions* functions;
	QOpenGLBuffer VBO,EBO;
	QOpenGLVertexArrayObject VAO;

	QMap<QString, Model> models; //模型
	QSharedPointer<Camera> camera; //摄像机

	SceneManager(QOpenGLShaderProgram* _shaderProgram, QOpenGLFunctions* _functions);
	~SceneManager() = default;
	void renderMesh(Mesh mesh);
	void renderTexture(Mesh mesh);

};

#endif
