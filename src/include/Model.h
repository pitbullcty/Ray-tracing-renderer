#ifndef __MODEL__
#define __MODEL__

#include "Mesh.h"
#include "Transform.h"

class Model {
public:
	Model(const QVector<Mesh>& _meshes, QVector3D _center = QVector3D(0.0f,0.0f,0.0f), QVector3D _originPos = QVector3D(0.0f, 0.0f, 0.0f));
	Model() = default;
	~Model() = default;
	QVector<Mesh> getMeshes();
	QVector3D getCenter();
	void destroyTextures();
	Transform transform; //变换组件

private:
	QVector<Mesh> meshes; //纹理
	QVector3D center; //模型重心
};



#endif 