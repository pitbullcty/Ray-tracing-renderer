#include "Model.h"

QVector<Mesh> Model::getMeshes()
{
    return meshes;
}

QVector3D Model::getCenter()
{
    return center;
}

void Model::destroyTextures()
{
    for (auto& mesh : meshes) {
        mesh.destoryTextures();
    }
}

Model::Model(const QVector<Mesh>& _meshes, QVector3D _center, QVector3D _originPos) :meshes(_meshes),center(_center) {
    transform.setOriginPos(_originPos);
};