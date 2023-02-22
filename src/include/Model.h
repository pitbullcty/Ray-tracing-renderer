#ifndef __MODEL__
#define __MODEL__

#include "Mesh.h"
#include "Transform.h"

class Model {
public:
	Model(const QVector<Mesh>& _meshes, QVector3D _center = QVector3D(0.0f,0.0f,0.0f));
	Model(const QVector<Mesh>& _meshes, const QString& _path, QVector3D _center = QVector3D(0.0f, 0.0f, 0.0f));
	Model() = default;
	~Model() = default;
	void destroyTextures();

	QVector<Mesh>& getMeshes();
	QVector3D getCenter();
	QString getPath();
	AABB& getBound();

	QJsonObject toJson();
	void prase(QJsonObject transform);
	void setData(const QVector<Mesh>& _meshes, const QString& _path, QVector3D _center);
	Transform transform; //变换组件


private:
	QString path;
	QVector<Mesh> meshes; //纹理
	QVector3D center; //模型重心
	AABB bound; //整个模型的Bound
};



#endif 