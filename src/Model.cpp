#include "Model.h"

QVector<Mesh> Model::getMeshes()
{
    return meshes;
}

Model::Model(const QVector<Mesh>& _meshes) :meshes(_meshes) {};