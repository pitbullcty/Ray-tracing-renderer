#include "RayTracingRenderer.h"

QSharedPointer<RayTracingRenderer> RayTracingRenderer::instance = nullptr;

void RayTracingRenderer::destory(RayTracingRenderer* rayTracingRenderer)
{
	delete rayTracingRenderer;
}

RayTracingRenderer::RayTracingRenderer(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height) :
	Renderer(_shaderProgram, _functions, width, height),
	BVHbuffer(-1), BVHTexture(-1),
	triangleBuffer(-1), triangleTexture(-1),
	materialBuffer(-1), materialTexture(-1),
	textureMapsArrayTex(-1),
	accumFBO(-1), accumTexture(-1),
	outputFBO(-1), outputTexture(-1),
	pathTracingFBO(-1), pathTracingTexture(-1),
	pathTraceVBO(QOpenGLBuffer::VertexBuffer),
	accumVBO(QOpenGLBuffer::VertexBuffer),
	outputVBO(QOpenGLBuffer::VertexBuffer),
	isResized(false),
	isSaving(false),
	hasData(false),
	isRealTimeDenoising(false),
	frameCounter(0),
	quality(50),
	denoiserStep(40)
{
	outputVBO.create();
	accumVBO.create();
	pathTraceVBO.create();
}

RayTracingRenderer::~RayTracingRenderer()
{
	
}

QSharedPointer<RayTracingRenderer>& RayTracingRenderer::GetInstance(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height)
{
	if (instance.isNull())
		instance = QSharedPointer<RayTracingRenderer>(new RayTracingRenderer(_shaderProgram, _functions, width, height), RayTracingRenderer::destory);
	return instance;
}

unsigned int RayTracingRenderer::generateAttachment(int w, int h)
{
	unsigned int id;
	functions->glGenTextures(1, &id);
	functions->glBindTexture(GL_TEXTURE_2D, id);
	functions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
	functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return id;
}


void RayTracingRenderer::bindVAO()
{
	QVector<float> quadData = { 
		-1, -1, 0, 1, -1, 0, 
		-1, 1, 0, 1, 1, 0, 
		-1, 1, 0, 1, -1, 0
	}; //长方形数据

	shaderProgram["pathTracing"]->bind();
	QOpenGLVertexArrayObject::Binder pathTracingBinder(&pathTracingVAO);
	pathTraceVBO.bind();
	pathTraceVBO.allocate(quadData.data(), (unsigned int)quadData.size() * sizeof(float));
	shaderProgram["pathTracing"]->enableAttributeArray(0);
	shaderProgram["pathTracing"]->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float)*3);
	shaderProgram["pathTracing"]->release();

	shaderProgram["accum"]->bind();
	QOpenGLVertexArrayObject::Binder accumBinder(&accumVAO);
	accumVBO.bind();
	accumVBO.allocate(quadData.data(), (unsigned int)quadData.size() * sizeof(float));
	shaderProgram["accum"]->enableAttributeArray(0);
	shaderProgram["accum"]->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 3);
	shaderProgram["accum"]->release();

	shaderProgram["output"]->bind();
	QOpenGLVertexArrayObject::Binder outputBinder(&outputVAO);
	outputVBO.bind();
	outputVBO.allocate(quadData.data(), (unsigned int)quadData.size() * sizeof(float));
	shaderProgram["output"]->enableAttributeArray(0);
	shaderProgram["output"]->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 3);
	shaderProgram["output"]->release();

}

void RayTracingRenderer::bindTexture()
{
	functions->glActiveTexture(GL_TEXTURE0);
	functions->glBindTexture(GL_TEXTURE_2D, envCubemap);

	functions->glActiveTexture(GL_TEXTURE1);
	functions->glBindTexture(GL_TEXTURE_BUFFER, BVHTexture);

	functions->glActiveTexture(GL_TEXTURE2);
	functions->glBindTexture(GL_TEXTURE_BUFFER, triangleTexture);

	functions->glActiveTexture(GL_TEXTURE3);
	functions->glBindTexture(GL_TEXTURE_BUFFER, materialTexture);

	functions->glActiveTexture(GL_TEXTURE5);
	functions->glBindTexture(GL_TEXTURE_2D_ARRAY, textureMapsArrayTex);

	functions->glActiveTexture(GL_TEXTURE6);
	functions->glBindTexture(GL_TEXTURE_2D, pathTracingTexture);

	functions->glActiveTexture(GL_TEXTURE7);
	functions->glBindTexture(GL_TEXTURE_2D, accumTexture); //绑定纹理单元


	functions->glActiveTexture(GL_TEXTURE0);
}

void RayTracingRenderer::destoryData()
{
	destoryTexture();
	destoryFBOs();
	destoryVAO();
}

void RayTracingRenderer::destoryFBOs()
{
	functions->glBindTexture(GL_TEXTURE_2D, 0);

	functions->glDeleteTextures(1, &pathTracingTexture);
	functions->glDeleteTextures(1, &accumTexture); //删除材质和FBO

	functions->glDeleteFramebuffers(1, &pathTracingFBO);
	functions->glDeleteFramebuffers(1, &accumFBO);
	
}

void RayTracingRenderer::destoryVAO()
{
	pathTracingVAO.destroy();
	pathTraceVBO.destroy();
	accumVAO.destroy();
	accumVBO.destroy();
	outputVAO.destroy();
	outputVBO.destroy();
}

void RayTracingRenderer::destoryTexture()
{
	functions->glDeleteTextures(1, &BVHTexture);
	functions->glDeleteBuffers(1, &BVHbuffer);

	functions->glDeleteTextures(1, &triangleTexture);
	functions->glDeleteBuffers(1, &triangleBuffer);

	functions->glDeleteTextures(1, &materialTexture);
	functions->glDeleteBuffers(1, &materialBuffer);

	functions->glDeleteTextures(1, &textureMapsArrayTex); //删除材质以及缓冲区
	
}

unsigned int RayTracingRenderer::getFrameCounter()
{
	return frameCounter;
}

void RayTracingRenderer::clearFrameCounter()
{
	frameCounter = 0;
}

void RayTracingRenderer::resize(int w, int h)
{
	frameCounter = 0;
	isResized = true;
}

void RayTracingRenderer::resizeFBO()
{
	if (isResized) {
		int viewport[4];
		functions->glGetIntegerv(GL_VIEWPORT, viewport);
		width = viewport[2];
		height = viewport[3];

		denoiserInputBuffer.resize(width * height);
		denoiserOutputBuffer.resize(width * height); //根据FBO大小创建缓冲区

		destoryFBOs();
		initFBOs();
		isResized = false;
	}
}


void RayTracingRenderer::sendDataToGPU(bool needSend)
{
	if (needSend) {
		QThreadPool* global = QThreadPool::globalInstance();
		if (global->activeThreadCount() != 0) {
			global->clear() ;
			QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, true, false);
			return;
		} //如果线程池还有耗时任务,则取消重新构建
	}

	auto& data = DataBuilder::GetInstance()->getData(); //获取渲染数据
	hasData = !data.encodedTriangles.isEmpty();

	destoryTexture();
	frameCounter = 0;

	functions->glGenBuffers(1, &BVHbuffer);
	functions->glBindBuffer(GL_TEXTURE_BUFFER, BVHbuffer);
	functions->glBufferData(GL_TEXTURE_BUFFER,sizeof(BVHNodeEncoded) * (unsigned int)data.encodedBVH.size(),data.encodedBVH.data(), GL_STATIC_DRAW);
	functions->glGenTextures(1, &BVHTexture);
	functions->glBindTexture(GL_TEXTURE_BUFFER, BVHTexture);
	functions->glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, BVHbuffer); //发送BVH数据
	functions->glBindBuffer(GL_TEXTURE_BUFFER, 0);


	functions->glGenBuffers(1, &triangleBuffer);
	functions->glBindBuffer(GL_TEXTURE_BUFFER, triangleBuffer);
    functions->glBufferData(GL_TEXTURE_BUFFER, sizeof(TriangleEncoded) * (unsigned int)data.encodedTriangles.size(), data.encodedTriangles.data(), GL_STATIC_DRAW);
	functions->glGenTextures(1, &triangleTexture);
	functions->glBindTexture(GL_TEXTURE_BUFFER, triangleTexture);
	functions->glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, triangleBuffer); //发送三角形数据
	functions->glBindBuffer(GL_TEXTURE_BUFFER, 0);

	functions->glGenBuffers(1, &materialBuffer);
	functions->glBindBuffer(GL_TEXTURE_BUFFER, materialBuffer);
	functions->glBufferData(GL_TEXTURE_BUFFER, sizeof(MaterialEncoded) * (unsigned int)data.encodedMaterials.size(), data.encodedMaterials.data(), GL_STATIC_DRAW);
	functions->glGenTextures(1, &materialTexture);
	functions->glBindTexture(GL_TEXTURE_BUFFER, materialTexture);
	functions->glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, materialBuffer); 
	functions->glBindBuffer(GL_TEXTURE_BUFFER, 0);//发送材质数据
	
 	functions->glGenTextures(1, &textureMapsArrayTex);
	functions->glBindTexture(GL_TEXTURE_2D_ARRAY, textureMapsArrayTex);
	functions->glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, Texturesize, Texturesize, data.encodedTexture.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	functions->glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	functions->glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	functions->glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	functions->glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	for (int i = 0; i < data.encodedTexture.size(); ++i)
	{
		functions->glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, Texturesize, Texturesize, 1, GL_RGBA, GL_UNSIGNED_BYTE, data.encodedTexture[i].bits());
	}
	functions->glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	
}

void RayTracingRenderer::initFBOs()
{
	accumTexture = generateAttachment(width, height);
	pathTracingTexture = generateAttachment(width, height);
	outputTexture = generateAttachment(width, height);

	functions->glGenFramebuffers(1, &pathTracingFBO);
	functions->glBindFramebuffer(GL_FRAMEBUFFER, pathTracingFBO);
	functions->glBindTexture(GL_TEXTURE_2D, pathTracingTexture);
	functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pathTracingTexture, 0);

	functions->glGenFramebuffers(1, &accumFBO);
	functions->glBindFramebuffer(GL_FRAMEBUFFER, accumFBO);
	functions->glBindTexture(GL_TEXTURE_2D, accumTexture);
	functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumTexture, 0);

	functions->glGenFramebuffers(1, &outputFBO);
	functions->glBindFramebuffer(GL_FRAMEBUFFER, outputFBO);
	functions->glBindTexture(GL_TEXTURE_2D, outputTexture);
	functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture, 0);

	functions->glBindTexture(GL_TEXTURE_2D, 0);
	functions->glBindFramebuffer(GL_FRAMEBUFFER, 0); //解绑

	bindVAO();

}

void RayTracingRenderer::render()
{
	
	resizeFBO(); //判断是否需要变换FBO大小
	bindTexture();

	projection = QMatrix4x4();
	projection.perspective(getCamera()->getZoom(), width / (float)height, 0.1f, 500.0f);

	pathTracingVAO.bind();
	shaderProgram["pathTracing"]->bind();
	shaderProgram["pathTracing"]->setUniformValue("hasData", hasData);
	shaderProgram["pathTracing"]->setUniformValue("cubemap", 0);
	shaderProgram["pathTracing"]->setUniformValue("BVHnodes", 1);
	shaderProgram["pathTracing"]->setUniformValue("triangles", 2);
	shaderProgram["pathTracing"]->setUniformValue("materials", 3);
	shaderProgram["pathTracing"]->setUniformValue("textureMapsArrayTex", 5);
	shaderProgram["pathTracing"]->setUniformValue("lastFrame", 7);
	shaderProgram["pathTracing"]->setUniformValue("width", width);
	shaderProgram["pathTracing"]->setUniformValue("height", height);
	shaderProgram["pathTracing"]->setUniformValue("projection", projection.inverted());
	shaderProgram["pathTracing"]->setUniformValue("eye", camera->getPos());
	shaderProgram["pathTracing"]->setUniformValue("cameraRotate", camera->getView().inverted());
	auto location = shaderProgram["pathTracing"]->programId();
	functions->glUniform1ui(functions->glGetUniformLocation(location,"frameCounter"), frameCounter++);

	functions->glBindFramebuffer(GL_FRAMEBUFFER, pathTracingFBO);
	functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	functions->glDrawArrays(GL_TRIANGLES, 0, 6);

	shaderProgram["pathTracing"]->release();
	functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
	pathTracingVAO.release();

	accumVAO.bind();
	shaderProgram["accum"]->bind();
	shaderProgram["accum"]->setUniformValue("lastFrame", 6);

	functions->glBindFramebuffer(GL_FRAMEBUFFER, accumFBO);
	functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	functions->glDrawArrays(GL_TRIANGLES, 0, 6);

	shaderProgram["accum"]->release();
	functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
	accumVAO.release();

	if (isRealTimeDenoising && frameCounter % denoiserStep == 0) {
		denoise(accumFBO, accumTexture, true);
	} //达到要求则降噪

	outputVAO.bind();
	shaderProgram["output"]->bind();
	shaderProgram["output"]->setUniformValue("lastFrame", 7);

	functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
	functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	functions->glDrawArrays(GL_TRIANGLES, 0, 6); //显示到屏幕

	shaderProgram["output"]->release();
	outputVAO.release();

	outputVAO.bind();
	shaderProgram["output"]->bind();
	shaderProgram["output"]->setUniformValue("lastFrame", 7);
	functions->glBindFramebuffer(GL_FRAMEBUFFER, outputFBO);
	functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	functions->glDrawArrays(GL_TRIANGLES, 0, 6); //绘制到outputFBO保存结果

	shaderProgram["output"]->release();
	functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
	outputVAO.release();

	if (isSaving) {
		saveRenderResult();
		isSaving = false;
	}
	
}

template<typename T>
void RayTracingRenderer::getPixels(QVector<T>& pixels, unsigned int FBO, unsigned int texture, GLenum format, GLenum type)
{
	functions->glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	functions->glBindTexture(GL_TEXTURE_2D, texture);
	functions->glReadPixels(0, 0, width, height, format, type, pixels.data());
	functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
	functions->glBindTexture(GL_TEXTURE_2D, 0);
}

void RayTracingRenderer::saveRenderResult()
{
	denoise(outputFBO, outputTexture, true);
	saveFBO(savePath, outputFBO, outputTexture, quality);
}

void RayTracingRenderer::setSavingParam(const QString& savePath, int quality)
{
	this->savePath = savePath;
	this->quality = quality;
	isSaving = true;
}

void RayTracingRenderer::saveFBO(const QString& path, unsigned int FBO, unsigned int texture, int quality)
{
	QVector<unsigned char> pixels(width * height * 4);
	getPixels<unsigned char>(pixels, FBO, texture, GL_RGBA, GL_UNSIGNED_BYTE);
	QImage image(pixels.data(), width, height, QImage::Format_RGBA8888);
	QFileInfo info(path);
	image.mirrored().save(path, info.suffix().toStdString().c_str(), quality);
}


void RayTracingRenderer::denoise(unsigned int FBO, unsigned int texture, bool needWriteBack)
{

	getPixels<QVector3D>(denoiserInputBuffer, FBO, texture, GL_RGB, GL_FLOAT); //读取数据
	
	oidn::DeviceRef device = oidn::newDevice();
	device.commit();

	oidn::FilterRef filter = device.newFilter("RT");
	
	filter.setImage("color", denoiserInputBuffer.data(), oidn::Format::Float3, width, height, 0, 0, 0);
	filter.setImage("output", denoiserOutputBuffer.data(), oidn::Format::Float3, width, height, 0, 0, 0);
	filter.set("hdr", false);
	filter.commit();

	filter.execute();  //执行降噪

	const char* errorMessage;
	if (device.getError(errorMessage) != oidn::Error::None)
		qDebug() << "Error: " << errorMessage;

	if (needWriteBack) {
		functions->glBindTexture(GL_TEXTURE_2D, texture);
		functions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGB, GL_FLOAT, denoiserOutputBuffer.data());
		functions->glBindTexture(GL_TEXTURE_2D, 0);
	}

}

