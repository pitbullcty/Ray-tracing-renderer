#ifndef __DATA_BUILDER__
#define __DATA_BUILDER__

#include <QSharedPointer>
#include "src/Data/Model.h"
#include "src/Ui/Console.h"
#include "src/Data/Triangle.h"
#include <QElapsedTimer>
#include <QtConcurrent>

const int Texturesize = 1024;

enum STRATEGY {
	MAXDIM,
	ALLAXIS
};//SAH策略

struct RenderData {
	QVector<TriangleEncoded> encodedTriangles; //编码后顶点数据
	QVector<BVHNodeEncoded> encodedBVH; //编码后BVH
	QVector<LightEncoded> encodedLight; //编码后灯光数据
	QVector<MaterialEncoded> encodedMaterials; //编码后材质
	QVector<QImage> encodedTexture; //编码后的贴图数组
	void clear() {
		encodedTriangles.clear();
		encodedBVH.clear();
		encodedLight.clear();
		encodedMaterials.clear();
		encodedTexture.clear();
	}
};

class DataBuilder:public QObject
{
	Q_OBJECT
public:
	static QSharedPointer<DataBuilder>& GetInstance();
	static void destory(DataBuilder* builder);
	void buildData(bool needTips = true, bool needSend=true);
	void setModels(QMap<QString, Model>* models);
	RenderData& getData(); //返回渲染数据的引用

signals:
	void Info(QString info);
	void sendDataDone(bool needSend); //数据准备好信号

private:
	QMap<QString, Model>* models; //存放当前数据复制

	QVector<BVHNode> bvh; //未编码bvh树
	QVector<Triangle> triangles;  //未编码顶点数据
	QMap<int, QVector<QPair<QString,int>>> texInfo; //展平后贴图信息
	QMap<int, int> textureIndex; //用于记录复制信息
	QVector<ModelMaterial> modelMaterialsFlatten; //展平后材质

	RenderData data; //渲染数据

	static QSharedPointer<DataBuilder> instance;
	DataBuilder();
	~DataBuilder() = default;

	int buildBVHHelp(int l, int r, int maxCount, int parent = -2, STRATEGY stratgey = MAXDIM);
	int findSplitAxis(int l, int r, float& cost); //找到最佳分割轴
	void flattenAndBuildData();
	void encodeData();
};

#endif 
