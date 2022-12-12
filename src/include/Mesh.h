#ifndef __MESH__
#define __MESH__

#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLTexture>

/*
* ������
*/
struct Vertex
{
	QVector3D pos; //����
	QVector3D normal; //����
	QVector2D texCoords; //��������
	Vertex(QVector3D _pos, QVector3D _normal, QVector2D _texCoords) :pos(_pos),normal(_normal),texCoords(_texCoords) {};
	Vertex() {};
};

/*
* ������
*/
struct Texture {
	QOpenGLTexture texture; //����
	QString type; //��������
	QString path;  //����·��
	Texture() :texture(QOpenGLTexture::Target2D) {
		texture.create();
		texture.setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
		texture.setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat); //ָ��������ͻ��Ʒ�ʽ
		texture.setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear); //����������˷�ʽ
	}
};


/*
* ������
*/
struct Mesh {
	QVector<Vertex> vertices;               //��������
	QVector<int> indices;          //��������
	QVector<Texture> textures;             //��������
	Mesh(QVector<Vertex> _vertices, QVector<int> _indices, QVector<Texture> _textures) :vertices(_vertices), indices(_indices), textures(_textures) {};
};



#endif 
