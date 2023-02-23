#include "BVH.h"

QVector3D max(const QVector3D& a, const QVector3D& b) {
	float maxx = std::max(a.x(), b.x());
	float maxy = std::max(a.y(), b.y());
	float maxz = std::max(a.z(), b.z());
	return { maxx, maxy,maxz };
}

QVector3D min(const QVector3D& a, const QVector3D& b) {
	float minx = std::min(a.x(), b.x());
	float miny = std::min(a.y(), b.y());
	float minz = std::min(a.z(), b.z());
	return { minx,miny,minz };
}