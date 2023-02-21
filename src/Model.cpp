#include "Model.h"

QVector<Mesh>& Model::getMeshes()
{
    return meshes;
}

QVector3D Model::getCenter()
{
    return center;
}

QString Model::getPath()
{
    return path;
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
}

void Model::setData(const QVector<Mesh>& _meshes, const QString& _path, QVector3D _center)
{
    meshes = _meshes;
    path = _path;
    center = _center;
}

void Model::destroyTextures()
{
    for (auto& mesh : meshes) {
        mesh.destoryTextures();
    }
}

Model::Model(const QVector<Mesh>& _meshes, QVector3D _center) :meshes(_meshes),center(_center) 
{
    
}

Model::Model(const QVector<Mesh>& _meshes, const QString& _path, QVector3D _center):meshes(_meshes), center(_center), path(_path)
{

}