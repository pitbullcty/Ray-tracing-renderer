#ifndef __RAY_TRACING_RENDERER__
#define __RAY_TRACING_RENDERER__

#include <QSharedPointer>
#include "Model.h"
#include "Console.h"
#include <QElapsedTimer>

enum STRATEGY {
	MAXDIM,
	ALLAXIS
};//SAH策略

class RayTracingRenderer:public QObject
{
	Q_OBJECT
public:
	static QSharedPointer<RayTracingRenderer>& GetInstance();
	static void destory(RayTracingRenderer* builder);

	void buildBVH();
	void setModels(QMap<QString, Model>* models);

signals:
	void Info(QString info);

private:
	QMap<QString, Model>* models;

	BVH bvh;
	QVector<Triangle> triangles;

	static QSharedPointer<RayTracingRenderer> instance;
	RayTracingRenderer();
	~RayTracingRenderer() = default;

	int buildBVHHelp(int l, int r, int maxCount, int parent = -2, STRATEGY stratgey = MAXDIM);
	int findSplitAxis(int l, int r, BVHNode& node, float& cost); //找到最佳分割轴
	void buildTriangles();
};

#endif 
