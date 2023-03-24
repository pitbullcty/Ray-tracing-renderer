#ifndef __EDITOR_RENDERER__
#define __EDITOR_RENDERER__

#include "Renderer.h"

//使用单例模式
class EditorRenderer : public Renderer {

public:

	Model* getSelected();

	void initSkybox();
	void renderModels();
	void renderSkybox();
	void renderGizmo();
	void renderAABB();

	virtual void resize(int w, int h);

	QSharedPointer<Gizmo> getGizmo();
	void setModels(QMap<QString, Model>* _models); //设置渲染模型指针

	static void destory(EditorRenderer* editorRenderer);
	virtual void destoryData();

	static QSharedPointer<EditorRenderer>& GetInstance(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, 
		QOpenGLExtraFunctions* _functions, int width, int height);

public slots:
	void setSelected(Model* model);

private:
	static QSharedPointer<EditorRenderer> instance;

	QOpenGLBuffer skyboxVBO;
	QOpenGLBuffer AABBVBO;

	QOpenGLVertexArrayObject skyboxVAO;
	QOpenGLVertexArrayObject AABBVAO;

	QSharedPointer<Gizmo> gizmo; //gizmo控件

	Model* selected; //被选中的物体
	QMap<QString, Model>* models; //模型

	EditorRenderer(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height);
	~EditorRenderer() = default;
};

#endif
