#ifndef __RENDERER__
#define __RENDERER__

#include "src/Data/Model.h"
#include "Camera.h"
#include "src/Data/Skybox.h"
#include "Gizmo.h"
#include <QMap>

class Renderer:public QObject {
	Q_OBJECT

public:
	QSharedPointer<Camera> getCamera();
	void initSkyboxTexture();
	virtual void resize(int w, int h) = 0;
	virtual void destoryData() = 0;
	Renderer(QMap<QString, QOpenGLShaderProgram*> shaderProgram, QOpenGLExtraFunctions* functions, int width, int height);

protected:
	QMap<QString, QOpenGLShaderProgram*> shaderProgram;
	QOpenGLExtraFunctions* functions;
	unsigned int envCubemap;

	int width;
	int height; //窗口相关参数

	QMatrix4x4 projection; //投影矩阵

	QSharedPointer<Camera> camera; //摄像机
	QSharedPointer<Skybox> skybox; //天空盒

};

#endif