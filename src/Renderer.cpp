#include "Renderer.h"

QSharedPointer<Renderer> Renderer::instance = nullptr;
const float PI = 3.14159f;

int renderIndex[][2] ={{0, 1},{2, 3},{4, 5},{6, 7},{0, 2},{1, 3},{4, 6},{5, 7},{0, 4},{1, 5},{7, 3},{2, 6}};
//AABB渲染顺序

void Renderer::destory(Renderer* renderer)
{
	renderer->modelVAO.destroy();
	renderer->modelVBO.destroy();
	renderer->modelEBO.destroy();
	renderer->models = nullptr;
	renderer->selected = nullptr;
	delete renderer;
}

QSharedPointer<Renderer>& Renderer::GetInstance(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height)
{
	if (instance.isNull())
		instance = QSharedPointer<Renderer>(new Renderer(_shaderProgram, _functions, width, height), Renderer::destory);
	return instance;
}

Renderer::Renderer(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height) :
	shaderProgram(_shaderProgram), 
	functions(_functions), 
	modelVBO(QOpenGLBuffer::VertexBuffer),
	modelEBO(QOpenGLBuffer::IndexBuffer), 
	skyboxVBO(QOpenGLBuffer::VertexBuffer),
	AABBVBO(QOpenGLBuffer::VertexBuffer),
	width(width), 
	height(height),
	selected(nullptr)
{
	models = nullptr;
	camera = Camera::GetInstance();
	skybox = Skybox::GetInstance();
	gizmo = QSharedPointer<Gizmo>(new Gizmo(functions, shaderProgram["gizmo"]));
	gizmo->setScale(5.0f);
	gizmo->setType(MOVE);
	gizmo->setEditModel(nullptr);
	modelVBO.create();
	skyboxVBO.create();
	AABBVBO.create();
	modelEBO.create();
	projection.perspective(getCamera()->getZoom(), width / (float)height, 0.1f, 500.0f);
}


void Renderer::setModels(QMap<QString, Model>* _models)
{
	
	models = _models;
}

void Renderer::setSelected(Model* model)
{
	this->selected = model;
}

void Renderer::renderModels()
{
	shaderProgram["model"]->bind();
	functions->glEnable(GL_DEPTH_TEST);
	functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	projection = QMatrix4x4();
	projection.perspective(getCamera()->getZoom(), width / (float)height, 0.1f, 500.0f);
	shaderProgram["model"]->setUniformValue("projection", projection);
	shaderProgram["model"]->setUniformValue("view", getCamera()->getView());
	for (auto it = models->begin(); it != models->end(); it++) {
		auto& model = it.value();
		shaderProgram["model"]->bind();
		shaderProgram["model"]->setUniformValue("model", model.transform.getModel());
		for (auto& mesh :model.getMeshes()) {
			renderMesh(mesh);
		}
	}
	shaderProgram["model"]->release();
	
}

void Renderer::renderModel(const QString& name)
{
	auto& model = (*models)[name];
	shaderProgram["model"]->bind();
	projection = QMatrix4x4();
	projection.perspective(getCamera()->getZoom(), width / (float)height, 0.1f, 500.0f);
	shaderProgram["model"]->setUniformValue("projection", projection);
	shaderProgram["model"]->setUniformValue("view", getCamera()->getView());
	shaderProgram["model"]->setUniformValue("model", model.transform.getModel());
	for (auto& mesh : model.getMeshes()) {
		renderMesh(mesh);
	}
	shaderProgram["model"]->release();
}

void Renderer::renderMesh(const Mesh& mesh)
{
	shaderProgram["model"]->bind();
	QOpenGLVertexArrayObject::Binder binder(&modelVAO);
	modelVBO.bind();
	modelVBO.allocate(mesh.vertices.data(), mesh.vertices.size() * sizeof(Vertex));

	modelEBO.bind();
	modelEBO.allocate(mesh.indices.data(), mesh.indices.size() * sizeof(unsigned int));

	shaderProgram["model"]->enableAttributeArray(0);
	shaderProgram["model"]->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));

	shaderProgram["model"]->enableAttributeArray(1);
	shaderProgram["model"]->setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));

	shaderProgram["model"]->enableAttributeArray(2);
	shaderProgram["model"]->setAttributeBuffer(2, GL_FLOAT, offsetof(Vertex, texCoords), 2, sizeof(Vertex));

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1; //各类材质数量

	for (unsigned int i = 0; i < mesh.textures.size(); i++)
	{
		QString number;
		QString name = mesh.textures[i].type;
		if (name == "texture_diffuse")
			number = QString::number(diffuseNr++);
		else if (name == "texture_specular")
			number = QString::number(specularNr++);
		else if (name == "texture_normal")
			number = QString::number(normalNr++);
		else if (name == "texture_height")
			number = QString::number(heightNr++);
		mesh.textures[i].texture->bind(i);
		shaderProgram["model"]->setUniformValue((name + number).toStdString().c_str(), i); //传输不同材质值
	}
	// 绘制网格
	functions->glDrawElements(GL_TRIANGLES, (unsigned int)mesh.indices.size(), GL_UNSIGNED_INT, 0);
	shaderProgram["model"]->release();
	modelVBO.release();
	modelEBO.release();
}



void Renderer::initSkybox()
{
	shaderProgram["skybox"]->bind();
	functions->glGenTextures(1, &envCubemap);
	functions->glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap); //生成立方体截图
	QString keys[6] = { "+x","-x","+y","-y","+z","-z"};
	int i = 0;
	for (auto& key:keys)
	{
		QImage tex(skybox->pathes[key]);
		tex = tex.convertToFormat(QImage::Format_RGB888);
		functions->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, tex.width(), tex.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, tex.bits()
		);
		i++;
	}
	functions->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	functions->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	functions->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	functions->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	functions->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	shaderProgram["skybox"]->setUniformValue("skybox", 0);

	QOpenGLVertexArrayObject::Binder binder(&skyboxVAO);
	skyboxVBO.bind();
	skyboxVBO.allocate(skybox->vertices.data(), skybox->vertices.size() * sizeof(float));
	shaderProgram["skybox"]->enableAttributeArray(0);
	shaderProgram["skybox"]->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 3);

	shaderProgram["skybox"]->release();
}

void Renderer::renderSkybox()
{
	shaderProgram["skybox"]->bind();
	shaderProgram["skybox"]->setUniformValue("projection", projection);
	shaderProgram["skybox"]->setUniformValue("view",camera->getView());
	functions->glActiveTexture(GL_TEXTURE0);
	functions->glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	functions->glDepthFunc(GL_LEQUAL); //一定要设置
	QOpenGLVertexArrayObject::Binder binder(&skyboxVAO);
	functions->glDrawArrays(GL_TRIANGLES, 0, 36);
	skyboxVBO.release();
	shaderProgram["skybox"]->release();
}

void Renderer::renderGizmo()
{
	auto view = camera->getView();
	shaderProgram["gizmo"]->bind();
	shaderProgram["gizmo"]->setUniformValue("projection", projection);
	shaderProgram["gizmo"]->setUniformValue("view", view);
	shaderProgram["gizmo"]->setUniformValue("model", QMatrix4x4());
	gizmo->setCamera(view, projection);
	gizmo->Draw();
	shaderProgram["gizmo"]->release();
}

void Renderer::renderAABB()
{
	if (!selected) return;

	auto view = camera->getView();
	shaderProgram["gizmo"]->bind();
	shaderProgram["gizmo"]->setUniformValue("projection", projection);
	shaderProgram["gizmo"]->setUniformValue("view", view);
	shaderProgram["gizmo"]->setUniformValue("model", selected->transform.getModel());
	auto AABBdata = selected->getBound().generateAABBData();
	float data[2 * 7];
	functions->glDisable(GL_DEPTH_TEST);
	QOpenGLVertexArrayObject::Binder binder(&AABBVAO);
	for (int i = 0; i < 12; i++) {
		memcpy(data, AABBdata[renderIndex[i][0]].data(), sizeof(float)*7);
		memcpy(data+7, AABBdata[renderIndex[i][1]].data(), sizeof(float)*7);
		AABBVBO.bind();
		AABBVBO.allocate(data,sizeof(data));
		shaderProgram["gizmo"]->enableAttributeArray(0);
		shaderProgram["gizmo"]->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 7);
		shaderProgram["gizmo"]->enableAttributeArray(1);
		shaderProgram["gizmo"]->setAttributeBuffer(1, GL_FLOAT, sizeof(float) * 3, 4, sizeof(float) * 7);
		functions->glDrawArrays(GL_LINES, 0, 2);
	}
	functions->glEnable(GL_DEPTH_TEST);
	AABBVBO.release();
	shaderProgram["gizmo"]->release();
}

void Renderer::resize(int w, int h)
{
	width = w;
	height = h;
	gizmo->setSize(w, h);
	projection.perspective(getCamera()->getZoom(), width / (float)height, 0.1f, 500.0f);
}

QSharedPointer<Gizmo> Renderer::getGizmo()
{
	return gizmo;
}


QSharedPointer<Camera> Renderer::getCamera()
{
	return camera;
}

