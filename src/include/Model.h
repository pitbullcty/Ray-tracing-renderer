#ifndef __MODEL__
#define __MODEL__

#include "Mesh.h"

class Model {
public:
	Model(const QVector<Mesh>& _meshes);
	Model() = default;
	~Model() = default;
	QVector<Mesh> getMeshes();

private:
	QVector<Mesh> meshes; //纹理

};



#endif 