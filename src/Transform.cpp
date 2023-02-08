#include "Transform.h"

const QVector3D x = QVector3D(1.0f, 0.0f, 0.0f);
const QVector3D y = QVector3D(0.0f, 1.0f, 0.0f);
const QVector3D z = QVector3D(0.0f, 0.0f, 1.0f);

Transform::Transform()
{
	scaleX = scaleY = scaleZ = 1.0f;
	rotationX = rotationY = rotationZ = 0.0f;
	translationX = translationY = translationZ = 0.0f;
}

void Transform::calcModel()
{
	model = QMatrix4x4();
	model.scale(scaleX, scaleY, scaleZ);
	model.rotate(rotationX, x);
	model.rotate(rotationY, y);
	model.rotate(rotationZ, z);
	model.translate(translationX, translationY, translationZ);
}

QMatrix4x4 Transform::getModel()
{
	return model;
}

void Transform::setModel(const QMatrix4x4& _model)
{
	model = _model;
}


