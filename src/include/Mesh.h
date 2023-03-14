#ifndef __MESH__
#define __MESH__

#include <QString>
#include "BVH.h"
#include <QOpenGLTexture>
#include <QSharedPointer>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <algorithm>
/*
* 顶点类
*/
struct Vertex
{
	QVector3D pos; //坐标
	QVector3D normal; //法线
	QVector2D texCoords; //材质坐标
	Vertex& map(const QMatrix4x4& model) {
		pos = model.map(pos);
		normal = (model.inverted().transposed() * QVector4D(normal, 0.0)).toVector3D().normalized();
		return *this;
	}
};

/*
* 材质类
*/
struct Texture {
	QSharedPointer<QOpenGLTexture> texture;
	QString type; //材质类型
	int id; //材质生成的id
	QString path;  //材质路径
};


/*
* 网格类
*/

class Mesh {

public:

	QVector<Vertex> vertices;               //顶点数据
	QVector<unsigned int> indices;          //索引数组
	QVector<Texture> textures;             //纹理数据

	QVector3D center; //中心
	AABB bound; //mesh的碰撞箱
	bool isInit; //mesh是否初始化

	QOpenGLVertexArrayObject VAO;
	QOpenGLBuffer VBO, EBO;

	Mesh(QOpenGLExtraFunctions* functions, QOpenGLShaderProgram* program);
	~Mesh();
	void setUp();
	void render();
	void destoryTextures();


private:

	QOpenGLExtraFunctions* functions;
	QOpenGLShaderProgram* shaderProgram;
};



#endif 
