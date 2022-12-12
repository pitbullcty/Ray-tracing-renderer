#ifndef __MESH__
#define __MESH__

#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLTexture>

/*
* 顶点类
*/
struct Vertex
{
	QVector3D pos; //坐标
	QVector3D normal; //法线
	QVector2D texCoords; //材质坐标
	Vertex(QVector3D _pos, QVector3D _normal, QVector2D _texCoords) :pos(_pos),normal(_normal),texCoords(_texCoords) {};
	Vertex() {};
};

/*
* 材质类
*/
struct Texture {
	QOpenGLTexture texture; //材质
	QString type; //材质类型
	QString path;  //材质路径
	Texture() :texture(QOpenGLTexture::Target2D) {
		texture.create();
		texture.setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
		texture.setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat); //指定纹理轴和环绕方式
		texture.setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear); //设置纹理过滤方式
	}
};


/*
* 网格类
*/
struct Mesh {
	QVector<Vertex> vertices;               //顶点数据
	QVector<int> indices;          //索引数组
	QVector<Texture> textures;             //纹理数据
	Mesh(QVector<Vertex> _vertices, QVector<int> _indices, QVector<Texture> _textures) :vertices(_vertices), indices(_indices), textures(_textures) {};
};



#endif 
