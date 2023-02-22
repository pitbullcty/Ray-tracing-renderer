#ifndef __MODEL__
#define __MODEL__

#include "Mesh.h"
#include "Transform.h"

class Model {
public:
	Model(const QVector<Mesh>& _meshes, QVector3D _center = QVector3D(0.0f,0.0f,0.0f));
	Model(const QVector<Mesh>& _meshes, const QString& _path, QVector3D _center = QVector3D(0.0f, 0.0f, 0.0f));
	Model();
	~Model() = default;
	void destroyTextures();

	void setCopy(Model* copy);
	Model* getCopy();
	bool isCopy(); 

	QVector<Mesh>& getMeshes();
	QVector3D getCenter() const;
	QString getPath() const;
	AABB& getBound();
	AABB& getDectionBound();
	void updateBound();

	QJsonObject toJson();
	void prase(QJsonObject transform);
	void setData(const QVector<Mesh>& _meshes, const QString& _path, QVector3D _center);

	Transform transform; //变换组件


private:
	QString path;
	QVector<Mesh> meshes; //纹理
	QVector3D center; //模型重心

	Model* data; //指向正确数据的指针

	AABB bound; //用来显示的bound
	AABB boundDetect; //用来碰撞计算的Bound
	
};



#endif 