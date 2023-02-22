#ifndef __BVH__
#define __BVH__

#include <QVector3D>
#include <QVector>
#include <limits.h>

constexpr auto MAX = std::numeric_limits<float>::max();
constexpr auto MIN = -std::numeric_limits<float>::max();


enum DIM {
	AXIS_X,
	AXIS_Y,
	AXIS_Z
};

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

	AABB(const QVector3D& minpos = { MAX,MAX,MAX }, const QVector3D& maxpos = {MIN,MIN,MIN}) :minpos(minpos), maxpos(maxpos) {};

	float getSurfaceArea() const {
		float x = maxpos.x() - minpos.x();
		float y = maxpos.y() - minpos.y();
		float z = maxpos.z() - minpos.z();
		return 2 * (x * y + y * z + z * x);
	}

	AABB& Union(const AABB& other) {
		float minx = std::min(minpos.x(), other.minpos.x());
		float miny = std::min(minpos.y(), other.minpos.y());
		float minz = std::min(minpos.z(), other.minpos.z());

		float maxx = std::max(maxpos.x(), other.maxpos.x());
		float maxy = std::max(maxpos.y(), other.maxpos.y());
		float maxz = std::max(maxpos.z(), other.maxpos.z());

		minpos = {minx, miny, minz};
		maxpos = {maxx, maxy, maxz};

		return *this;
	}

	AABB& Union(const QVector3D& other) {
		float minx = std::min(minpos.x(), other.x());
		float miny = std::min(minpos.y(), other.y());
		float minz = std::min(minpos.z(), other.z());

		float maxx = std::max(maxpos.x(), other.x());
		float maxy = std::max(maxpos.y(), other.y());
		float maxz = std::max(maxpos.z(), other.z());

		minpos = { minx, miny, minz };
		maxpos = { maxx, maxy, maxz };

		return *this;
	}

	QVector3D getCenter() const {
		return (minpos + maxpos) / 2;
	}

	DIM getMaxDim() const {
		float lenx = maxpos.x() - minpos.x();
		float leny = maxpos.y() - minpos.y();
		float lenz = maxpos.z() - minpos.z();
		float max = std::max(lenx, std::max(leny, lenz));
		if (max == lenx) return AXIS_X;
		else if (max == leny) return AXIS_Y;
		else return AXIS_Z;
	}

	 QVector<QVector<float>> generateAABBData() {
		float minx = minpos.x();
		float miny = minpos.y();
		float minz = minpos.z();

		float maxx = maxpos.x();
		float maxy = maxpos.y();
		float maxz = maxpos.z();

		QVector<float> data1 = { minx, miny, minz,0.0f,1.0f,0.0f,1.0f };
		QVector<float> data2 = { maxx, miny, minz,0.0f,1.0f,0.0f,1.0f };
		QVector<float> data3 = { minx, maxy, minz,0.0f,1.0f,0.0f,1.0f };
		QVector<float> data4 = { maxx, maxy, minz,0.0f,1.0f,0.0f,1.0f };
		QVector<float> data5 = { minx, miny, maxz,0.0f,1.0f,0.0f,1.0f };
		QVector<float> data6 = { maxx, miny, maxz,0.0f,1.0f,0.0f,1.0f };
		QVector<float> data7 = { minx, maxy, maxz,0.0f,1.0f,0.0f,1.0f };
		QVector<float> data8 = { maxx, maxy, maxz,0.0f,1.0f,0.0f,1.0f };

		QVector<QVector<float>> data = {data1, data2, data3, data4, data5, data6, data7, data8}; //生成长方形数据
		
		return data;
	}
};

struct Triangle
{
	QVector3D a;
	QVector3D b;
	QVector3D c;
	AABB bound;
	Triangle() = default;
	Triangle(const QVector3D& a, const QVector3D& b, const QVector3D& c) :a(a), b(b), c(c) {
		float minx = std::min(a.x(), std::min(b.x(), c.x()));
		float miny = std::min(a.y(), std::min(b.y(), c.y()));
		float minz = std::min(a.z(), std::min(b.z(), c.z()));

		float maxx = std::max(a.x(), std::max(b.x(), c.x()));
		float maxy = std::max(a.y(), std::max(b.y(), c.y()));
		float maxz = std::max(a.z(), std::max(b.z(), c.z()));

		bound.maxpos = { maxx,maxy,maxz };
		bound.minpos = { minx,miny,minz };
	};
	QVector3D getCenter() const {
		return (a + b + c) / 3;
	}
};

struct BVHNode
{
	int parent;
	int lChild, rChild;
	bool isLeaf;
	int n; //叶子结点三角形数量
	int index; //叶子结点在三角形中索引
	AABB bound; //节点包围盒
};

class BVH {
public:
	QVector<BVHNode> nodes;
	QVector<int> hitInfo; //记录节点碰撞信息
	BVH() = default;
};

#endif