#ifndef __TRIANGLE__
#define __TRIANGLE__

#include <QVector3D>
#include "BVH.h"

struct TriangleEncoded {
	QVector3D a, b, c;    // 顶点坐标
	QVector3D n1, n2, n3;    // 顶点法线
	QVector3D texcoord1, texcoord2, texcoord3; //纹理坐标
	TriangleEncoded() = default;
}; //编码放入材质中的三角形

struct Triangle
{
	Vertex a, b, c; //三个顶点
	AABB bound;
	int meshIndex;
	int modelIndex;
	Triangle() = default;
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c, int meshIndex, int modelIndex) :
		a(a),
		b(b),
		c(c),
		meshIndex(meshIndex),
		modelIndex(modelIndex)
	{
		bound.maxpos = max(a.pos, max(b.pos, c.pos));
		bound.minpos = min(a.pos, min(b.pos, c.pos));
	};
	QVector3D getCenter() const {
		return (a.pos + b.pos + c.pos) / 3;
	}
};

#endif // !__TRIANGLE__
