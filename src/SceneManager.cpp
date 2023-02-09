#include "SceneManager.h"

QSharedPointer<SceneManager> SceneManager::instance = nullptr;
const float PI = 3.14159f;


void SceneManager::destory(SceneManager* sceneManager)
{
	sceneManager->VAO.destroy();
	sceneManager->VBO.destroy();
	sceneManager->EBO.destroy();
	delete sceneManager;
}

QSharedPointer<SceneManager>& SceneManager::GetInstance(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height)
{
	if (instance.isNull())
		instance = QSharedPointer<SceneManager>(new SceneManager(_shaderProgram, _functions, width, height), SceneManager::destory);
	return instance;
}

SceneManager::SceneManager(QMap<QString, QOpenGLShaderProgram*> _shaderProgram, QOpenGLExtraFunctions* _functions, int width, int height) :
	shaderProgram(_shaderProgram), functions(_functions), VBO(QOpenGLBuffer::VertexBuffer)
	, EBO(QOpenGLBuffer::IndexBuffer), width(width), height(height)
{
	camera = Camera::GetInstance();
	skybox = Skybox::GetInstance();
	gizmo = QSharedPointer<Gizmo>(new Gizmo(functions, shaderProgram["gizmo"]));
	gizmo->setScale(5.0f);
	gizmo->setType(MOVE);
	VBO.create();
	EBO.create();
	projection.perspective(getCamera()->getZoom(), width / (float)height, 0.1f, 500.0f);
}


void SceneManager::addModel(const QString& name, Model model)
{
	models.insert(name, model);
	gizmo->setEditModel(&models[name]);
}

void SceneManager::clearModel()
{
	destoryTexture();
	models.clear();
}

void SceneManager::renderModels()
{
	shaderProgram["model"]->bind();
	functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	projection = QMatrix4x4();
	projection.perspective(getCamera()->getZoom(), width / (float)height, 0.1f, 500.0f);
	shaderProgram["model"]->setUniformValue("projection", projection);
	shaderProgram["model"]->setUniformValue("view", getCamera()->getView());
	for (auto it = models.begin(); it != models.end(); it++) {
		Model model = it.value();
		shaderProgram["model"]->setUniformValue("model", model.transform.getModel());
		for (auto& mesh :model.getMeshes()) {
			renderMesh(mesh);
			renderTexture(mesh);
		}
	}
	shaderProgram["model"]->release();
}

void SceneManager::renderMesh(const Mesh& mesh)
{
	QOpenGLVertexArrayObject::Binder binder(&VAO);

	VBO.bind();

	VBO.allocate(mesh.vertices.data(), mesh.vertices.size() * sizeof(Vertex));

	EBO.bind();
	EBO.allocate(mesh.indices.data(), mesh.indices.size() * sizeof(unsigned int));

	shaderProgram["model"]->enableAttributeArray(0);
	shaderProgram["model"]->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));

	shaderProgram["model"]->enableAttributeArray(1);
	shaderProgram["model"]->setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));

	shaderProgram["model"]->enableAttributeArray(2);
	shaderProgram["model"]->setAttributeBuffer(2, GL_FLOAT, offsetof(Vertex, texCoords), 2, sizeof(Vertex));

}

void SceneManager::renderTexture(const Mesh& mesh)
{
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
	QOpenGLVertexArrayObject::Binder binder(&VAO);
	functions->glDrawElements(GL_TRIANGLES, (unsigned int)mesh.indices.size(), GL_UNSIGNED_INT, 0);
	VBO.release();
	EBO.release();
}


void SceneManager::destoryTexture()
{
	for (auto it = models.begin(); it != models.end(); it++) {
		it->destroyTextures();
	}
}

void SceneManager::initSkybox()
{
	shaderProgram["skybox"]->bind();
	functions->glGenTextures(1, &envCubemap);
	functions->glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap); //生成立方体截图
	QString keys[6] = { "+x","-x","+y","-y","+z","-z"};
	int i = 0;
	for (auto& key:keys)
	{
		QImage tex(skybox->path[key]);
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
	shaderProgram["skybox"]->release();
}

void SceneManager::renderSkybox()
{
	shaderProgram["skybox"]->bind();
	shaderProgram["skybox"]->setUniformValue("projection", projection);
	shaderProgram["skybox"]->setUniformValue("view",camera->getView());
	functions->glActiveTexture(GL_TEXTURE0);
	functions->glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	renderCube(shaderProgram["skybox"]);
	shaderProgram["skybox"]->release();
}

void SceneManager::renderGizmo()
{
	auto view = camera->getView();
	shaderProgram["gizmo"]->bind();
	shaderProgram["gizmo"]->setUniformValue("projection", projection);
	shaderProgram["gizmo"]->setUniformValue("view", view);
	gizmo->setCamera(view, projection);
	gizmo->Draw();
}

void SceneManager::resize(int w, int h)
{
	width = w;
	height = h;
	gizmo->setSize(w, h);
	projection.perspective(getCamera()->getZoom(), width / (float)height, 0.1f, 500.0f);
}

QSharedPointer<Gizmo> SceneManager::getGizmo()
{
	return gizmo;
}

void SceneManager::renderCube(QOpenGLShaderProgram* shaderProgram)
{
	QOpenGLVertexArrayObject::Binder binder(&VAO);
	VBO.bind();
	VBO.allocate(skybox->vertices.data(), skybox->vertices.size() * sizeof(float));
	shaderProgram->enableAttributeArray(0);
	shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 3);
	functions->glDrawArrays(GL_TRIANGLES, 0, 36);
	VBO.release();
}

QSharedPointer<Camera> SceneManager::getCamera()
{
	return camera;
}

