#ifndef __SCENE_MANAGER__
#define __SCENE_MANAGER__

#include"Model.h"
#include "Camera.h"
#include "Skybox.h"
#include <QMap>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

//使用单例模式
class SceneManager {
public:
	void addModel(QString name, Model model); //添加模型
	void clearModel(); //删除模型
	void renderModels();
	void destoryTexture();
	void initSkybox();
	void renderSkybox();
	QSharedPointer<Camera> getCamera();

	static void destory(SceneManager* sceneManager);
	
	static QSharedPointer<SceneManager>& GetInstance(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLFunctions* _functions
	, int width, int height);

private:
	static QSharedPointer<SceneManager> instance;

	//OpenGl上下文相关
	QMap<QString, QOpenGLShaderProgram*> shaderProgram;
	QOpenGLFunctions* functions;
	QOpenGLBuffer VBO,EBO;
	QOpenGLVertexArrayObject VAO;
	unsigned int envCubemap;

	int width;
	int height; //viewpoint

	QMap<QString, Model> models; //模型
	QSharedPointer<Camera> camera; //摄像机
	QSharedPointer<Skybox> skybox;

	SceneManager(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLFunctions* _functions, int width, int height);
	~SceneManager() = default;
	void renderMesh(Mesh mesh);
	void renderTexture(Mesh mesh);
	void renderCube(QOpenGLShaderProgram* shaderProgram);

};

#endif
