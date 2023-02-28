#include "Transform.h"
#include <cmath>

const QVector3D x = QVector3D(1.0f, 0.0f, 0.0f);
const QVector3D y = QVector3D(0.0f, 1.0f, 0.0f);
const QVector3D z = QVector3D(0.0f, 0.0f, 1.0f);
const float PI = 3.1415926f;

Transform::Transform()
{
	scaleX = scaleY = scaleZ = 1.0f;
	rotationX = rotationY = rotationZ = 0.0f;
	translationX = translationY = translationZ = 0.0f;
}

void Transform::calcModel()
{
	model = QMatrix4x4();
	model.translate(translationX, translationY, translationZ);
	model.rotate(rotationZ, z);
	model.rotate(rotationY, y);
	model.rotate(rotationX, x);
	model.scale(scaleX, scaleY, scaleZ);
}

void Transform::reSet()
{
	scaleX = scaleY = scaleZ = 1.0f;
	rotationX = rotationY = rotationZ = 0.0f;
	translationX = translationY = translationZ = 0.0f;
	calcModel();
}

QVector3D Transform::getTranslation()
{
	return QVector3D(translationX, translationY, translationZ);
}

QMatrix4x4 Transform::getModel()
{
	return model;
}

void Transform::setModel(const QMatrix4x4& _model)
{
	model = _model;
	                        
	float data[16];
	_model.copyDataTo(data);

	translationX = data[3], translationY = data[7], translationZ = data[11]; //计算平移

	scaleX = sqrt(data[0] * data[0] + data[4] * data[4] + data[8] * data[8]);
	scaleY = sqrt(data[1] * data[1] + data[5] * data[5] + data[9] * data[9]);
	scaleZ = sqrt(data[2] * data[2] + data[6] * data[6] + data[10] * data[10]);

	rotationX = atan2(data[9]/scaleY, data[10]/scaleZ);
	rotationY = atan2(-data[8], sqrt(data[0]*data[0]+data[4]*data[4]));
	rotationZ = atan2(data[4], data[0]);

	rotationX = rotationX * 180 / PI;
	rotationY = rotationY * 180 / PI;
	rotationZ = rotationZ * 180 / PI;  //通过矩阵逆向计算

}

QJsonObject Transform::toJson()
{
	QJsonObject transform;
	transform.insert("translationX", translationX);
	transform.insert("translationY", translationY);
	transform.insert("translationZ", translationZ);
	transform.insert("rotationX", rotationX);
	transform.insert("rotationY", rotationY);
	transform.insert("rotationZ", rotationZ);
	transform.insert("scaleX", scaleX);
	transform.insert("scaleY", scaleY);
	transform.insert("scaleZ", scaleZ);
	return transform;

}

void Transform::prase(const QJsonObject& transform)
{
	this->translationX = transform["translationX"].toVariant().toFloat();
	this->translationY = transform["translationY"].toVariant().toFloat();
	this->translationZ = transform["translationZ"].toVariant().toFloat();
	this->rotationX = transform["rotationX"].toVariant().toFloat();
	this->rotationY = transform["rotationY"].toVariant().toFloat();
	this->rotationZ = transform["rotationZ"].toVariant().toFloat();
	this->scaleX = transform["scaleX"].toVariant().toFloat();
	this->scaleY = transform["scaleY"].toVariant().toFloat();
	this->scaleZ = transform["scaleZ"].toVariant().toFloat();
	calcModel();
}


