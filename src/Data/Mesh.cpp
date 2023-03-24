#include "Mesh.h"

Mesh::Mesh(QOpenGLExtraFunctions* functions, QOpenGLShaderProgram* program) :
	VBO(QOpenGLBuffer::VertexBuffer),
	EBO(QOpenGLBuffer::IndexBuffer),
	isInit(false)
{
	VBO.create();
	EBO.create();
	this->functions = functions;
	this->shaderProgram = program;
}

Mesh::~Mesh()
{
	VAO.destroy();
	VBO.destroy();
	EBO.destroy();
	functions = nullptr;
	shaderProgram = nullptr;
}

void Mesh::destoryTextures()
{
	for (auto& texture : textures) {
		texture.texture->destroy();
	}
}


void Mesh::setUp()
{
	shaderProgram->bind();
	
	QOpenGLVertexArrayObject::Binder binder(&VAO);

	VBO.bind();
	VBO.allocate(vertices.data(), (unsigned int)vertices.size() * sizeof(Vertex));

	EBO.bind();
	EBO.allocate(indices.data(), (unsigned int)indices.size() * sizeof(unsigned int));

	shaderProgram->enableAttributeArray(0);
	shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));

	shaderProgram->enableAttributeArray(1);
	shaderProgram->setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));

	shaderProgram->enableAttributeArray(2);
	shaderProgram->setAttributeBuffer(2, GL_FLOAT, offsetof(Vertex, texCoords), 2, sizeof(Vertex));

	shaderProgram->release();
	VAO.release();
	isInit = true;
}

void Mesh::render()
{
	shaderProgram->bind();
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1; //各类材质数量

	if(textures.isEmpty()) 
		shaderProgram->setUniformValue("hasTexture", false);
	else
		shaderProgram->setUniformValue("hasTexture", true);

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		QString number;
		QString name = textures[i].type;
		if (name == "texture_diffuse")
			number = QString::number(diffuseNr++);
		else if (name == "texture_metalness")
			number = QString::number(specularNr++);
		else if (name == "texture_normal")
			number = QString::number(normalNr++);
		else if (name == "texture_emissive")
			number = QString::number(heightNr++);
		textures[i].texture->bind(i);
		shaderProgram->setUniformValue((name + number).toStdString().c_str(), i); //传输不同材质值
	}
	// 绘制网格
	QOpenGLVertexArrayObject::Binder binder(&VAO);
	functions->glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
	shaderProgram->release();

}

