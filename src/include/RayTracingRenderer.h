#ifndef _RAY_TRACING_RENDERER_
#define _RAY_TRACING_RENDERER_

#include "Renderer.h"
#include "DataBuilder.h"
#include "thirdparty/hdrloader/hdrloader.h"

class RayTracingRenderer :public Renderer {
public:

	void initFBOs();
	void render();
	void clearFrameCounter();
	virtual void destoryData();
	virtual void resize(int w, int h);

	static QSharedPointer<RayTracingRenderer>& GetInstance(QMap<QString, QOpenGLShaderProgram*> _shaderProgram,
		QOpenGLExtraFunctions* _functions, int width, int height);

	static void destory(RayTracingRenderer* rayTracingRenderer);

public slots:
	void sendDataToGPU();  //收到信号发送编码好数据至gpu端

private:
	
	unsigned int frameCounter; //帧计数器
	unsigned int hdr; //hdr贴图

	QOpenGLBuffer pathTraceVBO;
	QOpenGLVertexArrayObject pathTracingVAO;

	QOpenGLBuffer accumVBO;
	QOpenGLVertexArrayObject accumVAO;

	QOpenGLBuffer outputVBO;
	QOpenGLVertexArrayObject outputVAO;  //shader使用的VAO以及VBO

	unsigned int pathTracingFBO, accumFBO;
	unsigned int pathTracingTexture, accumTexture; //用到的FBO以及颜色附件

	unsigned int BVHbuffer;
	unsigned int BVHTexture; 

	unsigned int triangleBuffer;
	unsigned int triangleTexture; //三角形材质缓存

	unsigned int materialBuffer; 
	unsigned int materialTexture; //材质

	unsigned int lightsTexture; //灯光
	unsigned int textureMapsArrayTex; //贴图

	static QSharedPointer<RayTracingRenderer> instance;

	unsigned int generateAttachment(int w, int h);

	void bindVAO();
	void bindTexture();

	void destoryFBOs();
	void destoryVAO();
	void destoryTexture(); //资源销毁

	RayTracingRenderer(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height);
	~RayTracingRenderer() = default;
};

#endif // !_RAY_TRACING_RENDERER
