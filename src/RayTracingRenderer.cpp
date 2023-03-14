#include "RayTracingRenderer.h"

QSharedPointer<RayTracingRenderer> RayTracingRenderer::instance = nullptr;

void RayTracingRenderer::destory(RayTracingRenderer* rayTracingRenderer)
{
	delete rayTracingRenderer;
}

RayTracingRenderer::RayTracingRenderer(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height) :
	Renderer(_shaderProgram, _functions, width, height),
	BVHbuffer(-1), BVHTexture(-1), frameCounter(0), materialBuffer(-1),
	triangleBuffer(-1), triangleTexture(-1),
	materialTexture(-1), lightsTexture(-1), textureMapsArrayTex(-1),
	accumFBO(-1),accumTexture(-1),pathTracingFBO(-1),pathTracingTexture(-1),
	pathTraceVBO(QOpenGLBuffer::VertexBuffer), accumVBO(QOpenGLBuffer::VertexBuffer),
	outputVBO(QOpenGLBuffer::VertexBuffer)
{
	outputVBO.create();
	accumVBO.create();
	pathTraceVBO.create();
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
	functions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
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
	shaderProgram["pathTracing"]->setUniformValue("width", width);
	shaderProgram["pathTracing"]->setUniformValue("height", height);
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
	functions->glBindTexture(GL_TEXTURE_2D, hdr);

	functions->glActiveTexture(GL_TEXTURE1);
	functions->glBindTexture(GL_TEXTURE_BUFFER, BVHTexture);

	functions->glActiveTexture(GL_TEXTURE2);
	functions->glBindTexture(GL_TEXTURE_BUFFER, triangleTexture);

	functions->glActiveTexture(GL_TEXTURE3);
	functions->glBindTexture(GL_TEXTURE_BUFFER, materialTexture);

	functions->glActiveTexture(GL_TEXTURE4);
	functions->glBindTexture(GL_TEXTURE_2D, lightsTexture);

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

	functions->glDeleteTextures(1, &lightsTexture);
	functions->glDeleteTextures(1, &textureMapsArrayTex); //删除材质以及缓冲区
	
}


void RayTracingRenderer::sendDataToGPU()
{
	
	auto& data = DataBuilder::GetInstance()->getData(); //获取渲染数据

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
	
	functions->glGenTextures(1, &lightsTexture);
	functions->glBindTexture(GL_TEXTURE_2D, lightsTexture);
	functions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, (sizeof(LightEncoded) / sizeof(QVector3D)) * data.encodedLight.size(), 1, 0, GL_RGB, GL_FLOAT, data.encodedLight.data());
	functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	functions->glBindTexture(GL_TEXTURE_2D, 0);
	
	functions->glGenTextures(1, &textureMapsArrayTex);
	functions->glBindTexture(GL_TEXTURE_2D_ARRAY, textureMapsArrayTex);
	functions->glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, Texturesize, Texturesize, data.encodedTexture.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data.encodedTexture.data());
	functions->glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	functions->glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	functions->glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	

}

void RayTracingRenderer::initFBOs()
{
	HDRLoaderResult hdrRes;
	bool r = HDRLoader::load("F://Ray-tracing-renderer//resources//hdr//pizzo_pernice_puresky_4k.hdr", hdrRes);
	hdr = generateAttachment(hdrRes.width, hdrRes.height);
	functions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, hdrRes.width, hdrRes.height, 0, GL_RGB, GL_FLOAT, hdrRes.cols);

	accumTexture = generateAttachment(width, height);
	pathTracingTexture = generateAttachment(width, height);

	functions->glGenFramebuffers(1, &pathTracingFBO);
	functions->glBindFramebuffer(GL_FRAMEBUFFER, pathTracingFBO);
	functions->glBindTexture(GL_TEXTURE_2D, pathTracingTexture);
	functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pathTracingTexture, 0);


	functions->glGenFramebuffers(1, &accumFBO);
	functions->glBindFramebuffer(GL_FRAMEBUFFER, accumFBO);
	functions->glBindTexture(GL_TEXTURE_2D, accumTexture);
	functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumTexture, 0);


	functions->glBindTexture(GL_TEXTURE_2D, 0);
	functions->glBindFramebuffer(GL_FRAMEBUFFER, 0); //解绑

	bindVAO();

}

void RayTracingRenderer::render()
{

	functions->glViewport(0, 0, width, height);
	functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	bindTexture();
	
	pathTracingVAO.bind();
	shaderProgram["pathTracing"]->bind();
	shaderProgram["pathTracing"]->setUniformValue("hdrMap", 0);
	shaderProgram["pathTracing"]->setUniformValue("BVHnodes", 1);
	shaderProgram["pathTracing"]->setUniformValue("triangles", 2);
	shaderProgram["pathTracing"]->setUniformValue("materials", 3);
	shaderProgram["pathTracing"]->setUniformValue("lights", 4);
	shaderProgram["pathTracing"]->setUniformValue("lastFrame", 7);
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
	functions->glBindFramebuffer(GL_FRAMEBUFFER, accumFBO);
	shaderProgram["accum"]->setUniformValue("lastFrame", 6);
	functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	functions->glDrawArrays(GL_TRIANGLES, 0, 6);
	shaderProgram["accum"]->release();
	functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
	accumVAO.release();
	
	
	outputVAO.bind();
	shaderProgram["output"]->bind();
	functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
	shaderProgram["output"]->setUniformValue("lastFrame", 7);
	shaderProgram["output"]->setUniformValue("needToneMapping", true);
	functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	functions->glDrawArrays(GL_TRIANGLES, 0, 6);
	shaderProgram["output"]->release();
	outputVAO.release();

}

void RayTracingRenderer::clearFrameCounter()
{
	frameCounter = 0;
}

void RayTracingRenderer::resize(int w, int h)
{
	frameCounter = 0;
	width = w;
	height = h;
	destoryFBOs();
	initFBOs();
}