#include "Model.h"

QVector<QSharedPointer<Mesh>>& Model::getMeshes()
{
    return meshes;
}

QVector3D Model::getCenter() const
{
    return center;
}

QString Model::getPath() const
{
    return path;
}

AABB& Model::getBound()
{
    return bound;
}

AABB& Model::getDectionBound()
{
    return boundDetect;
}

void Model::updateBound()
{
    auto model = transform.getModel();

    boundDetect.minpos = model.map(bound.minpos);
    boundDetect.maxpos = model.map(bound.maxpos);
}

QJsonObject Model::toJson()
{
    QJsonObject model;
    model.insert("modelPath", path);
    model.insert("transform", transform.toJson());
    if (type == LIGHT) {
        model.insert("material", lightMaterial.toJson());
    }
    else {
        model.insert("material", modelMaterial.toJson());
    }
    return model;
}

void Model::prase(const QJsonObject& transform, const QJsonObject& material)
{
    this->transform.prase(transform);
    if (type == NORMAL) modelMaterial.prase(material);
    else lightMaterial.prase(material);
    updateBound();
}

void Model::setData(const QVector<QSharedPointer<Mesh>>& _meshes, const QString& _path, QVector3D _center)
{
    meshes = _meshes;
    path = _path;
    center = _center;
    data = nullptr;
}

void Model::destroyTextures()
{
    if (meshes.isEmpty()) return;
    for (auto& mesh : meshes) {
        mesh->destoryTextures();
    }
}

void Model::setCopy(Model* copy, bool needUpdate)
{
    data = copy;
    bound = copy->getBound();
    center = copy->getCenter(); //复制bound以及center
    path = copy->getPath();
    type = copy->getType();
    if (needUpdate) {
        transform.reSet();
        updateBound();
    }
}

void Model::copyFrom(Model* copy)
{
    transform = copy->transform;
    if (type == NORMAL) modelMaterial = copy->modelMaterial;
    else lightMaterial = copy->lightMaterial;
}

void Model::setType(MODELTYPE type)
{
    this->type = type;
}

Model* Model::getCopy()
{
    return data;
}

MODELTYPE Model::getType()
{
    return type;
}

bool Model::isCopy()
{
    return data ? true : false;
}

Model::Model(const QVector<QSharedPointer<Mesh>>& _meshes, QVector3D _center) :
    meshes(_meshes),
    center(_center), 
    data(nullptr),
    type(NORMAL)
{
    
}

Model::Model(const QVector<QSharedPointer<Mesh>>& _meshes, const QString& _path, QVector3D _center):
    meshes(_meshes), 
    center(_center),
    path(_path),
    data(nullptr),
    type(NORMAL)
{

}

Model::Model():
    data(nullptr),
    type(NORMAL)
{
}

Model::~Model()
{
    data = nullptr;
}
