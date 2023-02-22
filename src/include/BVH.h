#ifndef __BVH__
#define __BVH__

#include <QVector3D>
#include <QVector>
#include <limits.h>
#include <QMatrix4x4>

constexpr auto MAX = std::numeric_limits<float>::max();
constexpr auto MIN = -std::numeric_limits<float>::max();

QVector3D max(const QVector3D& a, const QVector3D& b);
QVector3D min(const QVector3D& a, const QVector3D& b);

enum DIM {
	AXIS_X,
	AXIS_Y,
	AXIS_Z
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
		minpos = min(minpos, other.minpos);
		maxpos = max(maxpos, other.maxpos);
		return *this;
	}

	AABB& Union(const QVector3D& other) {
		minpos = min(minpos,other);
		maxpos = max(maxpos,other);
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
		bound.maxpos = max(a, max(b, c));
		bound.minpos = min(a, min(b, c));
	};
	QVector3D getCenter() const {
		return (a + b + c) / 3;
	}
};

struct Ray
{
	QVector3D pos;
	QVector3D direction;

	Ray() = default;
	Ray(const QVector3D& pos, const QVector3D& direction) :pos(pos), direction(direction) {};
	Ray(float x, float y, const QMatrix4x4& projection, const QMatrix4x4& view) {

		QVector3D ndc = { x, y,1.0f}; //还原至NDC坐标
		QVector4D clip = QVector4D(ndc, 1.0f); //裁剪前坐标

		QVector4D eye = projection.inverted() * clip; //眼睛坐标
		QVector4D world = view.inverted() * eye; //世界坐标

		if ((world.w()-0.0f)>1e-6) {
			world.setX(world.x() / world.w());
			world.setY(world.y() / world.w());
			world.setZ(world.z() / world.w());
		} //转换为标准坐标

		pos = QVector3D(world);
	};

	float hitAABB(const AABB& bound) {
		QVector3D in = (bound.minpos - pos) / direction;
		QVector3D out = (bound.maxpos - pos) / direction;

		QVector3D tmax = max(in, out);
		QVector3D tmin = min(in, out);

		float t1 = std::min(tmax.x(), std::min(tmax.y(), tmax.z()));
		float t0 = std::max(tmin.x(), std::max(tmin.y(), tmin.z()));

		return (t1 >= t0) ? ((t0 > 0.0) ? (t0) : (t1)) : (-1);

	} //计算和aabb相交的最近距离
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