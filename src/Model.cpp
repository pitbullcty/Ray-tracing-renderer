#include "Model.h"

QVector<Mesh> Model::getMeshes()
{
    return meshes;
}

void Model::destroyTextures()
{
    for (auto& mesh : meshes) {
        mesh.destoryTextures();
    }
}

Model::Model(const QVector<Mesh>& _meshes) :meshes(_meshes) {};