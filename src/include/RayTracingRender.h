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

class RayTracingRender:public QObject
{
	Q_OBJECT
public:
	static QSharedPointer<RayTracingRender>& GetInstance();
	static void destory(RayTracingRender* builder);

	void buildBVH();
	void setModels(QMap<QString, Model>* models);

signals:
	void Info(QString info);

private:
	QMap<QString, Model>* models;

	BVH bvh;
	QVector<Triangle> triangles;

	static QSharedPointer<RayTracingRender> instance;
	RayTracingRender();
	~RayTracingRender() = default;

	int buildBVHHelp(int l, int r, int maxCount, int parent = -2, STRATEGY stratgey = MAXDIM);
	int findSplitAxis(int l, int r, BVHNode& node, float& cost); //找到最佳分割轴
	void buildTriangles();
};

#endif 
