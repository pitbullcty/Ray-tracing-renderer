#include "EditorRenderer.h"

#include <cmath> //std::tan: gizmo自适应尺寸
#include <QVector4D>

QSharedPointer<EditorRenderer> EditorRenderer::instance = nullptr;
const float PI = 3.14159f;
bool testing = true;	

int renderIndex[][2] = { {0, 1},{2, 3},{4, 5},{6, 7},
	{0, 2},{1, 3},{4, 6},{5, 7},
	{0, 4},{1, 5},{7, 3},{2, 6} };
//AABB渲染索引

void EditorRenderer::destory(EditorRenderer* editorRenderer)
{
	editorRenderer->models = nullptr;
	editorRenderer->selected = nullptr;
	editorRenderer->selected = nullptr;
	delete editorRenderer;
}

void EditorRenderer::destoryData()
{
	skyboxVBO.destroy();
	AABBVBO.destroy();
	skyboxVAO.destroy();
	AABBVAO.destroy();
}

QSharedPointer<EditorRenderer>& EditorRenderer::GetInstance(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height)
{
	if (instance.isNull())
		instance = QSharedPointer<EditorRenderer>(new EditorRenderer(_shaderProgram, _functions, width, height), EditorRenderer::destory);
	return instance;
}

EditorRenderer::EditorRenderer(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height) :
	Renderer(_shaderProgram, _functions, width, height),
	skyboxVBO(QOpenGLBuffer::VertexBuffer),
	AABBVBO(QOpenGLBuffer::VertexBuffer),
	selected(nullptr),models(nullptr)
{
	gizmo = QSharedPointer<Gizmo>(new Gizmo(functions, shaderProgram["gizmo"]));
	//gizmo屏幕尺寸系数: 轴长约占地平线方向视野高度的1.5*0.15=22%(原来5.0时高达75%,会糊住整个画面)
	gizmo->setScale(1.5f);
	gizmo->setType(MOVE);
	gizmo->setEditModel(nullptr);
	skyboxVBO.create();
	AABBVBO.create();
	projection.perspective(getCamera()->getZoom(), width / (float)height, 0.1f, 500.0f);
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
	shaderProgram["model"]->setUniformValue("cameraPos", getCamera()->getPos()); //简单光照需要相机位置
	for (auto it = models->begin(); it != models->end(); it++) {
		auto& model = it.value();
		shaderProgram["model"]->bind();
		if (!model.isLight())
			shaderProgram["model"]->setUniformValue("color", model.modelMaterial.baseColor);
		else
			shaderProgram["model"]->setUniformValue("color", model.lightMaterial.baseColor);
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
	shaderProgram["skybox"]->setUniformValue("skybox", 0);
	QOpenGLVertexArrayObject::Binder binder(&skyboxVAO);
	skyboxVBO.bind();
	skyboxVBO.allocate(skybox->vertices.data(), (unsigned int)skybox->vertices.size() * sizeof(float));
	shaderProgram["skybox"]->enableAttributeArray(0);
	shaderProgram["skybox"]->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 3);
	shaderProgram["skybox"]->release();
}

void EditorRenderer::renderSkybox()
{
	shaderProgram["skybox"]->bind();
	shaderProgram["skybox"]->setUniformValue("projection", projection);
	shaderProgram["skybox"]->setUniformValue("view", camera->getView());
	functions->glActiveTexture(GL_TEXTURE0);
	functions->glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	functions->glDepthFunc(GL_LEQUAL); //一定要设置
	QOpenGLVertexArrayObject::Binder binder(&skyboxVAO);
	functions->glDrawArrays(GL_TRIANGLES, 0, 36);
	skyboxVBO.release();
	shaderProgram["skybox"]->release();
}

void EditorRenderer::updateGizmoScale()
{
	if (!selected) return;

	//世界空间包围盒(局部bound经transform变换后的boundDetect)
	AABB& worldBound = selected->getDectionBound();
	QVector3D extent = worldBound.maxpos - worldBound.minpos;
	if (extent.length() <= 1e-8f) return;
	const QVector3D center = worldBound.getCenter();
	const float radius = 0.5f * extent.length(); //包围球半径(用对角线近似,与朝向无关)

	//物体中心在相机空间中的深度(透视投影的w就是相机空间-z)
	const QVector4D centerView = camera->getView() * QVector4D(center, 1.0f);
	const float depth = qMax(0.001f, -centerView.z());
	const float tanHalf = std::tan(getCamera()->getZoom() * 0.5f * PI / 180.0f); //getZoom()返回fovy(度)
	const float heightPx = qMax(1.0f, (float)height);
	//物体半径在屏幕上的像素尺寸
	const float radiusPx = radius / (depth * tanHalf) * heightPx * 0.5f;
	//目标轴长: 略大于物体半径(方便抓取), 同时限制最小可抓取尺寸与最大占比, 避免小模型看不见/大模型糊屏
	const float axisPx = qBound(45.0f, radiusPx * 1.6f, heightPx * 0.32f);

	//库内 ComputeScreenFactor() = scale*0.15*depth, 换算到像素: axisPx ≈ 0.075*scale*height/tanHalf
	const float scale = axisPx * tanHalf / (0.075f * heightPx);
	gizmo->setScale(qBound(0.35f, scale, 8.0f));
}

void EditorRenderer::renderGizmo()
{
	if (!selected) return;
	updateGizmoScale(); //gizmo尺寸跟随物体屏幕大小
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
		memcpy(data, AABBdata[renderIndex[i][0]].data(), sizeof(float) * 7);
		memcpy(data + 7, AABBdata[renderIndex[i][1]].data(), sizeof(float) * 7);
		AABBVBO.bind();
		AABBVBO.allocate(data, sizeof(data));
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
	GLint major, minor;
	functions->glGetIntegerv(GL_MAJOR_VERSION, &major); //gl大版本（整型）
	functions->glGetIntegerv(GL_MINOR_VERSION, &minor); //gl小版本（整型）5
}

QSharedPointer<Gizmo> EditorRenderer::getGizmo()
{
	return gizmo;
}

void EditorRenderer::setModels(QMap<QString, Model>* _models)
{
	this->models = _models;
}
