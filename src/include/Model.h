#ifndef __MODEL__
#define __MODEL__

#include "Mesh.h"

class Model {
public:
	Model(const QVector<Mesh>& _meshes, QVector3D _center = QVector3D(0.0f,0.0f,0.0f));
	Model() = default;
	~Model() = default;
	QVector<Mesh> getMeshes();
	QVector3D getCenter();
	void destroyTextures();

private:
	QVector<Mesh> meshes; //纹理
	QVector3D center;

};



#endif 