#include "Transform.h"

const QVector3D x = QVector3D(1.0f, 0.0f, 0.0f);
const QVector3D y = QVector3D(0.0f, 1.0f, 0.0f);
const QVector3D z = QVector3D(0.0f, 0.0f, 1.0f);

Transform::Transform()
{
	scaleX = scaleY = scaleZ = 1.0f;
	rotationX = rotationY = rotationZ = 0.0f;
	translationX = translationY = translationZ = 0.0f;
	originPos = {0.0f,0.0f,0.0f};
	model.translate(originPos);
}

QMatrix4x4 Transform::getModel()
{
	model = QMatrix4x4();
	model.translate(originPos);
	model.scale(scaleX, scaleY, scaleZ);
	model.rotate(rotationX, x);
	model.rotate(rotationY, y);
	model.rotate(rotationZ, z);
	model.translate(translationX, translationY, translationZ);
	return model;
}

void Transform::setOriginPos(const QVector3D& pos)
{
	originPos = pos;
	model.translate(originPos);
}

