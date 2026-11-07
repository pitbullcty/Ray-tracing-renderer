#ifndef _RAY_TRACING_RENDERER_
#define _RAY_TRACING_RENDERER_

#include "Renderer.h"
#include "GPUBVHBuilder.h"
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

	RayTracingRenderer(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height);
	~RayTracingRenderer();

	static QSharedPointer<RayTracingRenderer>& GetInstance(QMap<QString, QOpenGLShaderProgram*> _shaderProgram,
		QOpenGLExtraFunctions* _functions, int width, int height);

	static void destory(RayTracingRenderer* rayTracingRenderer);

	//离屏渲染线程使用:独立模式下只渲染到outputFBO,不绘制到屏幕也不自动保存
	void setStandaloneMode(bool state);
	bool getIsStandalone();

	//离屏渲染线程使用:相机由界面线程快照传入,避免跨线程读取Camera单例
	void setCameraSnapshot(const QVector3D& pos, const QMatrix4x4& view, float zoom);

	//离屏渲染线程使用:直接上传一份渲染数据(不经过DataBuilder)
	void uploadRenderData(const RenderData& data, const QImage& background);

	//取outputFBO中的当前画面(降噪后),用于回传预览
	QImage grabResultImage();

	//实时降噪(GPU a-trous边缘保持滤波,无需读回CPU)
	void setRealTimeDenoise(bool state);
	bool getRealTimeDenoise() const;
	void setFireflyClamp(float value); //萤火虫抑制上限,<=0 关闭
	float getFireflyClamp() const;
	double getLastDenoiseMs() const;   //上一次GPU降噪耗时(ms)
	void setUseOIDNFinalDenoise(bool state); //最终出图是否使用OIDN(默认用GPU滤波)
	bool getUseOIDNFinalDenoise() const;

	//GPU动态构建BVH: 需要在GL线程中调用(需要当前GL上下文)
	void setUseGPUBVH(bool state);
	bool getUseGPUBVH() const;
	bool initGPUBVH(); //初始化GPU构建器,返回是否可用
	bool isGPUBVHReady() const;
	QString getGPUBVHStatus() const;
	double getLastGPUBVHMs() const; //上一次GPU构建耗时
	double getLastCPUBVHMs() const; //上一次CPU构建耗时
	int getGPUBVHTriangleCount() const;
	bool rebuildBVHNow(); //按当前模式重新构建BVH(设置面板"重建并对比"用,需在GL线程调用)
	void setPendingBVHRebuild(bool state);
	void setAutoRebuildBVH(bool state); //数据变化时是否自动重建GPU BVH
	bool getAutoRebuildBVH() const;

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

	bool isStandalone; //独立模式(后台离屏渲染线程使用)
	bool useCameraSnapshot; //使用外部传入的相机快照
	QVector3D snapshotPos;
	QMatrix4x4 snapshotView;
	float snapshotZoom = 45.0f;

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

	//降噪用的G-Buffer与中间缓冲
	unsigned int gNormalDepthTexture;  //xyz=世界法线, w=命中距离
	unsigned int gAlbedoTexture;       //rgb=反照率
	unsigned int momentsFBO, momentsTexture; //rgb=局部均值, a=亮度方差
	unsigned int denoiseFBO[2];
	unsigned int denoiseTexture[2];
	unsigned int denoiseResultTexture;
	QOpenGLBuffer denoiseVBO;
	QOpenGLVertexArrayObject denoiseVAO;

	bool realTimeDenoise = false;     //实时预览是否降噪
	float fireflyClampValue = 8.0f;   //萤火虫抑制上限
	double lastDenoiseMs = 0.0;       //GPU降噪耗时
	bool useOIDNFinalDenoise = false; //最终出图使用OIDN还是GPU滤波

	unsigned int BVHbuffer;
	unsigned int BVHTexture; 

	unsigned int triangleBuffer;
	unsigned int triangleTexture; //三角形材质缓存

	unsigned int materialBuffer; 
	unsigned int materialTexture; //材质

	unsigned int lightBuffer;
	unsigned int lightTexture; //灯光三角形CDF(重要性采样用)
	int lightTriangleCount = 0;
	float lightTotalArea = 0.0f;

	unsigned int backgroundTexture; //背景材质；

	unsigned int textureMapsArrayTex; //贴图

	QVector<QVector3D> denoiserInputBuffer, denoiserOutputBuffer; //降噪使用的缓冲区

	GPUBVHBuilder gpuBVHBuilder; //GPU动态构建BVH
	bool useGPUBVH = false; //是否启用GPU构建BVH
	bool gpuBVHReady = false; //GPU构建器是否已初始化
	bool bvhTextureFromGPU = false; //BVHTexture是否指向GPU构建出的节点缓冲
	bool pendingBVHRebuild = false; //下一帧是否重建BVH
	bool autoRebuildBVH = true; //数据变化时是否自动重建GPU BVH
	double lastGPUBVHMs = 0.0; //上一次GPU构建耗时
	int gpuBVHTriangleCount = 0; //上一次GPU构建处理的三角形数量

	bool isSavingSnapshot;
	int snapShotquality; 
	QString snapShotSavingPath; //快照相关参数

	static QSharedPointer<RayTracingRenderer> instance;

	unsigned int generateAttachment(int w, int h);
	unsigned int generateAttachment(const QImage& image);

	void denoise(unsigned int FBO, unsigned int texture, bool needWriteBack=false); //降噪

	//GPU a-trous 降噪: 返回降噪结果纹理(源数据不满足条件时原样返回)
	unsigned int runGPUDenoise(unsigned int sourceTexture, int iterations);
	void drawFullscreen(); //用降噪用的全屏四边形绘制
	unsigned int generateAttachmentRGBA(int w, int h); //带 alpha 通道的附件(方差)

	template<typename T>
	void getPixels(QVector<T>& pixels, unsigned int FBO, unsigned int texture, GLenum format, GLenum type);
	void saveFBO(const QString& path, unsigned int FBO, unsigned int texture, int quality = 50);

	void bindVAO();
	void bindTexture();

	void destoryFBOs();
	void destoryVAO();
	void destoryTexture(); //资源销毁

	bool buildGPUBVHTexture(); //用GPU构建BVH并让BVHTexture指向结果,成功返回true

	//渲染时使用的相机参数:独立模式下取快照,否则取Camera单例
	QVector3D renderCameraPos();
	QMatrix4x4 renderCameraView();
	float renderCameraZoom();
};

#endif // !_RAY_TRACING_RENDERER
