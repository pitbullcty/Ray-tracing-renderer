#ifndef __MESH__
#define __MESH__

#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLTexture>
#include <QSharedPointer>

/*
* 顶点类
*/
struct Vertex
{
	QVector3D pos; //坐标
	QVector3D normal; //法线
	QVector2D texCoords; //材质坐标
};

/*
* 材质类
*/
struct Texture {
	QSharedPointer<QOpenGLTexture> texture;
	QString type; //材质类型
	QString path;  //材质路径
};


/*
* 网格类
*/
struct Mesh {
	QVector<Vertex> vertices;               //顶点数据
	QVector<unsigned int> indices;          //索引数组
	QVector<Texture> textures;             //纹理数据
	QVector3D center; //中心
	Mesh(const QVector<Vertex>& _vertices, const QVector<unsigned int>& _indices, const QVector<Texture>& _textures, const QVector3D& _center ) :vertices(_vertices), indices(_indices), textures(_textures),center(_center) {};
	void destoryTextures() {
		for (auto& texture : textures) {
			texture.texture->destroy();
		}
	}
};



#endif 
