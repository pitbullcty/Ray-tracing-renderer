#ifndef __SCENE_MANAGER__
#define __SCENE_MANAGER__

#include"Model.h"
#include "Camera.h"
#include "Skybox.h"
#include "Gizmo.h"
#include <QMap>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

//使用单例模式
class SceneManager {
public:
	void addModel(const QString& name, Model model); //添加模型
	void clearModel(); //删除模型
	void renderModels();
	void renderModel(const QString& name); //渲染单个模型
	void destoryTexture();
	void initSkybox();
	void renderSkybox();
	void renderGizmo();
	void resize(int w, int h);
	QSharedPointer<Gizmo> getGizmo();
	QSharedPointer<Camera> getCamera();

	static void destory(SceneManager* sceneManager);
	
	static QSharedPointer<SceneManager>& GetInstance(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions
	, int width, int height);

private:
	static QSharedPointer<SceneManager> instance;

	//OpenGl上下文相关
	QMap<QString, QOpenGLShaderProgram*> shaderProgram;
	QOpenGLExtraFunctions* functions;
	QOpenGLBuffer VBO,EBO;
	QOpenGLVertexArrayObject VAO;
	unsigned int envCubemap;

	int width;
	int height; //窗口相关参数

	QMatrix4x4 projection; //投影矩阵

	QMap<QString, Model> models; //模型
	QSharedPointer<Camera> camera; //摄像机
	QSharedPointer<Skybox> skybox; //天空盒
	QSharedPointer<Gizmo> gizmo; //gizmo控件

	SceneManager(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height);
	~SceneManager() = default;
	void renderMesh(const Mesh& mesh);
	void renderTexture(const Mesh& mesh);
	void renderCube(QOpenGLShaderProgram* shaderProgram);

};

#endif
