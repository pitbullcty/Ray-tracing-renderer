#ifndef __BVH__
#define __BVH__

#include <QVector3D>
#include <QVector>

struct Ray
{
	QVector3D pos;
	QVector3D direction;

	Ray() = default;
	Ray(const QVector3D& pos, const QVector3D& direction) :pos(pos), direction(direction) {};

};

struct AABB {
	QVector3D minpos;
	QVector3D maxpos;

	AABB() = default;
	AABB(const QVector3D& minpos, const QVector3D& maxpos) :minpos(minpos), maxpos(maxpos) {};

	float getSurface() {
		float x = maxpos.x() - minpos.x();
		float y = maxpos.y() - minpos.y();
		float z = maxpos.z() - maxpos.z();
		return 2 * (x * y + y * z + z * x);
	}
};

struct BVHNode
{
	int parent;
	int lChild, rChild;
	bool isLeaf;
	AABB bound;
};

class BVH {
	QVector<BVHNode> nodes;
};

#endif