﻿#ifndef _RAY_TRACING_RENDERER_
#define _RAY_TRACING_RENDERER_

#include "Renderer.h"
#include "src/Manager/DataBuilder.h"
#include <QFileInfo>
#include "OpenImageDenoise/oidn.hpp"
#include "RenderOption.h"

class RayTracingRenderer :public Renderer {
	Q_OBJECT

public:
	void render();

	void renderAccum();

	void saveRenderResult(const QString& path, int quality);
	void setSnapshotParam(const QString& savePath, int quality=50);

	unsigned int getFrameCounter();
	void clearFrameCounter();

	void resizeFBO();
	void initFBOs();

	virtual void destoryData();
	virtual void resize(int w, int h);

	RenderOption& getOption();

	void stopRender();

	void setRealTimeRendering(bool state);
	bool getIsRealTimeRendering();

	bool getIsOffScreenRendering();

	static QSharedPointer<RayTracingRenderer>& GetInstance(QMap<QString, QOpenGLShaderProgram*> _shaderProgram,
		QOpenGLExtraFunctions* _functions, int width, int height);

	static void destory(RayTracingRenderer* rayTracingRenderer);

signals:
	void Info(const QString& info, bool needProcess);

public slots:
	void sendDataToGPU(bool needSend);  //收到信号发送编码好数据至gpu端
	void setRenderOption(const RenderOption& option);

private:
	bool isResized;  //是否发生resize事件
	bool hasData; //是否有数据传输

	bool isOffScreenRendering; //开启离屏渲染
	bool isRealTimeRendering; //实时渲染
	int delayStep; //延迟步数

	QElapsedTimer timer; //计数器

	RenderOption option;

	QOpenGLBuffer pathTraceVBO;
	QOpenGLVertexArrayObject pathTracingVAO;

	QOpenGLBuffer accumVBO;
	QOpenGLVertexArrayObject accumVAO;

	QOpenGLBuffer outputVBO;
	QOpenGLVertexArrayObject outputVAO;  //shader使用的VAO以及VBO

	unsigned int pathTracingFBO, accumFBO, outputFBO;
	unsigned int pathTracingTexture, accumTexture, outputTexture; //用到的FBO以及颜色附件

	unsigned int BVHbuffer;
	unsigned int BVHTexture; 

	unsigned int triangleBuffer;
	unsigned int triangleTexture; //三角形材质缓存

	unsigned int materialBuffer; 
	unsigned int materialTexture; //材质

	unsigned int backgroundTexture; //背景材质；

	unsigned int textureMapsArrayTex; //贴图

	QVector<QVector3D> denoiserInputBuffer, denoiserOutputBuffer; //降噪使用的缓冲区

	bool isSavingSnapshot;
	int snapShotquality; 
	QString snapShotSavingPath; //快照相关参数

	static QSharedPointer<RayTracingRenderer> instance;

	unsigned int generateAttachment(int w, int h);
	unsigned int generateAttachment(const QImage& image);

	void denoise(unsigned int FBO, unsigned int texture, bool needWriteBack=false); //降噪

	template<typename T>
	void getPixels(QVector<T>& pixels, unsigned int FBO, unsigned int texture, GLenum format, GLenum type);
	void saveFBO(const QString& path, unsigned int FBO, unsigned int texture, int quality = 50);

	void bindVAO();
	void bindTexture();

	void destoryFBOs();
	void destoryVAO();
	void destoryTexture(); //资源销毁

	RayTracingRenderer(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height);
	~RayTracingRenderer();
};

#endif // !_RAY_TRACING_RENDERER
