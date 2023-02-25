#ifndef __RENDERER__
#define __RENDERER__

#include "Model.h"
#include "Camera.h"
#include "Skybox.h"
#include "Gizmo.h"
#include <QMap>


//使用单例模式
class EditorRenderer {
public:
	void setModels(QMap<QString, Model>* _models); //设置渲染模型指针
	void setSelected(Model* model);
	Model* getSelected();

	void renderModels();

	void initSkybox();
	void renderSkybox();

	void renderGizmo();
	void renderAABB();

	void resize(int w, int h);

	QSharedPointer<Gizmo> getGizmo();
	QSharedPointer<Camera> getCamera();

	static void destory(EditorRenderer* sceneManager);
	
	static QSharedPointer<EditorRenderer>& GetInstance(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions
	, int width, int height);

private:
	static QSharedPointer<EditorRenderer> instance;

	//OpenGl上下文相关
	QMap<QString, QOpenGLShaderProgram*> shaderProgram;
	QOpenGLExtraFunctions* functions;

	QOpenGLBuffer skyboxVBO;
	QOpenGLBuffer AABBVBO;

	QOpenGLVertexArrayObject skyboxVAO;
	QOpenGLVertexArrayObject AABBVAO;

	unsigned int envCubemap;

	int width;
	int height; //窗口相关参数

	QMatrix4x4 projection; //投影矩阵

	QMap<QString, Model>* models; //模型
	QSharedPointer<Camera> camera; //摄像机
	QSharedPointer<Skybox> skybox; //天空盒
	QSharedPointer<Gizmo> gizmo; //gizmo控件

	Model* selected; //被选中的物体

	EditorRenderer(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height);
	~EditorRenderer() = default;
};

#endif
