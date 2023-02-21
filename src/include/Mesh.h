#ifndef __MESH__
#define __MESH__

#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLTexture>
#include <QSharedPointer>
#include "BVH.h"
#include <omp.h>
#include <algorithm>
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

class Mesh {

public:

	QVector<Vertex> vertices;               //顶点数据
	QVector<unsigned int> indices;          //索引数组
	QVector<Texture> textures;             //纹理数据
	QVector<Triangle> triangles; //当前mesh的三角形
	QVector3D center; //中心
	BVH bvh; //当前mesh的BVH
	Mesh() = default;
	void destoryTextures();
};



#endif 
