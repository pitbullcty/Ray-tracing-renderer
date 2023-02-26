#include "EditorRenderer.h"

QSharedPointer<EditorRenderer> EditorRenderer::instance = nullptr;
const float PI = 3.14159f;

int renderIndex[][2] ={{0, 1},{2, 3},{4, 5},{6, 7},{0, 2},{1, 3},{4, 6},{5, 7},{0, 4},{1, 5},{7, 3},{2, 6}};
//AABB渲染顺序

void EditorRenderer::destory(EditorRenderer* renderer)
{
	renderer->models = nullptr;
	renderer->selected = nullptr;
	renderer->selected = nullptr;
	delete renderer;
}

QSharedPointer<EditorRenderer>& EditorRenderer::GetInstance(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height)
{
	if (instance.isNull())
		instance = QSharedPointer<EditorRenderer>(new EditorRenderer(_shaderProgram, _functions, width, height), EditorRenderer::destory);
	return instance;
}

EditorRenderer::EditorRenderer(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height) :
	shaderProgram(_shaderProgram), 
	functions(_functions), 
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
	skyboxVBO.create();
	AABBVBO.create();
	projection.perspective(getCamera()->getZoom(), width / (float)height, 0.1f, 500.0f);
}

void EditorRenderer::setModels(QMap<QString, Model>* _models)
{
	
	models = _models;
}

void EditorRenderer::setSelected(Model* model)
{
	this->selected = model;
	getGizmo()->setEditModel(model);
}

Model* EditorRenderer::getSelected()
{
	return selected;
}


void EditorRenderer::renderModels()
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
		QVector<QSharedPointer<Mesh>> meshes;
		if (model.isCopy()) {
			meshes = model.getCopy()->getMeshes();
		}
		else meshes = model.getMeshes();
		for (auto& mesh : meshes) {
			if (!mesh->isInit) {
				mesh->setUp();
			} //mesh未初始化则直接初始化
			mesh->render();
		}
	}
	shaderProgram["model"]->release();
}


void EditorRenderer::initSkybox()
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

void EditorRenderer::renderSkybox()
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

void EditorRenderer::renderGizmo()
{
	if (!selected) return;
	auto view = camera->getView();
	shaderProgram["gizmo"]->bind();
	shaderProgram["gizmo"]->setUniformValue("projection", projection);
	shaderProgram["gizmo"]->setUniformValue("view", view);
	shaderProgram["gizmo"]->setUniformValue("model", QMatrix4x4());
	gizmo->setCamera(view, projection);
	gizmo->Draw();
	shaderProgram["gizmo"]->release();
}

void EditorRenderer::renderAABB()
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

void EditorRenderer::resize(int w, int h)
{
	width = w;
	height = h;
	gizmo->setSize(w, h);
	projection.perspective(getCamera()->getZoom(), width / (float)height, 0.1f, 500.0f);
}

QSharedPointer<Gizmo> EditorRenderer::getGizmo()
{
	return gizmo;
}


QSharedPointer<Camera> EditorRenderer::getCamera()
{
	return camera;
}

