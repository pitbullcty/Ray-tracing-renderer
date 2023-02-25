#ifndef __MODEL__
#define __MODEL__

#include "Mesh.h"
#include "Transform.h"

class Model {
public:
	Model(const QVector<QSharedPointer<Mesh>>& _meshes, QVector3D _center = QVector3D(0.0f,0.0f,0.0f));
	Model(const QVector<QSharedPointer<Mesh>>& _meshes, const QString& _path, QVector3D _center = QVector3D(0.0f, 0.0f, 0.0f));
	Model();
	~Model();
	void destroyTextures();

	void setCopy(Model* copy, bool needUpdate = true);
	Model* getCopy();
	bool isCopy(); 

	QVector<QSharedPointer<Mesh>>& getMeshes();
	QVector3D getCenter() const;
	QString getPath() const;

	AABB& getBound();
	AABB& getDectionBound();
	void updateBound();

	QJsonObject toJson();
	void prase(QJsonObject transform);
	void setData(const QVector<QSharedPointer<Mesh>>& _meshes, const QString& _path, QVector3D _center);

	Transform transform; //变换组件


private:
	QString path;
	QVector<QSharedPointer<Mesh>> meshes; //纹理
	QVector3D center; //模型重心

	Model* data; //指向正确数据的指针

	AABB bound; //用来显示的bound
	AABB boundDetect; //用来碰撞计算的Bound
	
};



#endif 