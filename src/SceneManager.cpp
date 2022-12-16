#include "SceneManager.h"

QSharedPointer<SceneManager> SceneManager::instance = nullptr;

void SceneManager::destory(SceneManager* sceneManager)
{
    sceneManager->VAO.destroy();
    sceneManager->VBO.destroy();
    sceneManager->EBO.destroy();
    delete sceneManager;
}

 QSharedPointer<SceneManager>& SceneManager::GetInstance(QOpenGLShaderProgram* _shaderProgram, QOpenGLFunctions* _functions)
{
    if (instance.isNull())
        instance = QSharedPointer<SceneManager>(new SceneManager(_shaderProgram, _functions), SceneManager::destory);
    return instance;
}

SceneManager::SceneManager(QOpenGLShaderProgram* _shaderProgram, QOpenGLFunctions* _functions) :
	shaderProgram(_shaderProgram),functions(_functions), VBO(QOpenGLBuffer::VertexBuffer)
    , EBO(QOpenGLBuffer::IndexBuffer)
{
    camera = Camera::GetInstance();
}


void SceneManager::addModel(QString name, Model model)
{
	models.insert(name, model);
}

void SceneManager::renderModels()
{
    shaderProgram->bind();
    for (auto it = models.begin(); it != models.end();it++) {
        for (auto& mesh : it.value().getMeshes()) {
            renderMesh(mesh);
            renderTexture(mesh);
        }
	}

}


void SceneManager::renderMesh(Mesh mesh)
{
    QOpenGLVertexArrayObject::Binder binder(&VAO);

    VBO.create();
    EBO.create();

    VBO.bind();
    VBO.allocate(mesh.vertices.data(), mesh.vertices.size() * sizeof(Vertex));

    EBO.bind();
    EBO.allocate(mesh.indices.data(), mesh.indices.size() * sizeof(unsigned int));

    shaderProgram->enableAttributeArray(0);
    shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));

    shaderProgram->enableAttributeArray(1);
    shaderProgram->setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));

    shaderProgram->enableAttributeArray(2);
    shaderProgram->setAttributeBuffer(2, GL_FLOAT, offsetof(Vertex, normal), 2, sizeof(Vertex));

}

void SceneManager::renderTexture(Mesh mesh)
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
        shaderProgram->setUniformValue((name + number).toStdString().c_str(), i); //传输不同材质值
    }
    // 绘制网格
    QOpenGLVertexArrayObject::Binder binder(&VAO);
    functions->glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void SceneManager::destoryTexture()
{
    for (auto it = models.begin(); it != models.end(); it++) {
        for (auto& mesh : it.value().getMeshes()) {
            for (auto& tex : mesh.textures) {
                tex.texture->destroy();
            }
        }
    }
}

