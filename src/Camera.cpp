#include "Camera.h"

QSharedPointer<Camera> Camera::instance = nullptr;

QSharedPointer<Camera>& Camera::GetInstance()
{
	if (instance.isNull())
		instance = QSharedPointer<Camera>(new Camera(),Camera::destory);
	return instance;
}

void Camera::destory(Camera* camera) {
	delete camera;
}