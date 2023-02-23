#include "Model.h"

QVector<Mesh>& Model::getMeshes()
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
    return model;
}

void Model::prase(QJsonObject transform)
{
    this->transform.translationX = transform["translationX"].toVariant().toFloat();
    this->transform.translationY = transform["translationY"].toVariant().toFloat();
    this->transform.translationZ = transform["translationZ"].toVariant().toFloat();
    this->transform.rotationX = transform["rotationX"].toVariant().toFloat();
    this->transform.rotationY = transform["rotationY"].toVariant().toFloat();
    this->transform.rotationZ = transform["rotationZ"].toVariant().toFloat();
    this->transform.scaleX = transform["scaleX"].toVariant().toFloat();
    this->transform.scaleY = transform["scaleY"].toVariant().toFloat();
    this->transform.scaleZ = transform["scaleZ"].toVariant().toFloat();
    this->transform.calcModel();
    updateBound();
}

void Model::setData(const QVector<Mesh>& _meshes, const QString& _path, QVector3D _center)
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
        mesh.destoryTextures();
    }
}

void Model::setCopy(Model* copy, bool needUpdate)
{
    data = copy;
    bound = copy->getBound();
    center = copy->getCenter(); //复制bound以及center
    path = copy->getPath();
    if (needUpdate) {
        transform.reSet();
        updateBound();
    }
}

Model* Model::getCopy()
{
    return data;
}

bool Model::isCopy()
{
    return data ? true : false;
}

Model::Model(const QVector<Mesh>& _meshes, QVector3D _center) :meshes(_meshes),center(_center), data(nullptr)
{
    
}

Model::Model(const QVector<Mesh>& _meshes, const QString& _path, QVector3D _center):meshes(_meshes), center(_center), path(_path), data(nullptr)
{

}

Model::Model():
    data(nullptr)
{
}

Model::~Model()
{
    data = nullptr;
}
