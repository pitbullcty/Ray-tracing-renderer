#ifndef __BVH_BUILDER__
#define __BVH_BUILDER__

#include <QSharedPointer>
#include "Model.h"
#include <QtConcurrent/qtconcurrentrun.h>
#include "Console.h"

enum STRATEGY {
	MAXDIM,
	ALLAXIS
};//SAH策略

class BVHBuilder {
public:
	static QSharedPointer<BVHBuilder>& GetInstance(QMap<QString, Model>* models);
	static void destory(BVHBuilder* builder);
	void buildBVH();

private:
	QMap<QString, Model>* models;
	static QSharedPointer<BVHBuilder> instance;
	BVHBuilder(QMap<QString, Model>* models);
	~BVHBuilder() = default;

	int findSplitAxis(Mesh& mesh, int l, int r, BVHNode& node, float& cost); //找到最佳分割轴
	int buildMeshBVH(Mesh& mesh, int l, int r, int maxCount, int parent = -2, STRATEGY stratgey = MAXDIM);
	void buildTriangle(Mesh& mesh);
};

#endif 
