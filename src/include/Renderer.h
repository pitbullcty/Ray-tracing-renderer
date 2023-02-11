#ifndef __RENDERER__
#define __RENDERER__

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
class Renderer {
public:
	void setModels(QMap<QString, Model>* _models); //设置渲染模型指针

	void renderModels();
	void renderModel(const QString& name); //渲染单个模型
	void initSkybox();
	void renderSkybox();
	void renderGizmo();
	void resize(int w, int h);

	QSharedPointer<Gizmo> getGizmo();
	QSharedPointer<Camera> getCamera();

	static void destory(Renderer* sceneManager);
	
	static QSharedPointer<Renderer>& GetInstance(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions
	, int width, int height);

private:
	static QSharedPointer<Renderer> instance;

	//OpenGl上下文相关
	QMap<QString, QOpenGLShaderProgram*> shaderProgram;
	QOpenGLExtraFunctions* functions;
	QOpenGLBuffer VBO,EBO;
	QOpenGLVertexArrayObject VAO;
	unsigned int envCubemap;

	int width;
	int height; //窗口相关参数

	QMatrix4x4 projection; //投影矩阵

	QMap<QString, Model>* models; //模型
	QSharedPointer<Camera> camera; //摄像机
	QSharedPointer<Skybox> skybox; //天空盒
	QSharedPointer<Gizmo> gizmo; //gizmo控件

	Renderer(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height);
	~Renderer() = default;
	void renderMesh(const Mesh& mesh);
	void renderTexture(const Mesh& mesh);
	void renderCube(QOpenGLShaderProgram* shaderProgram);

};

#endif
