#include "Camera.h"

QSharedPointer<Camera> Camera::instance = nullptr;

static float radians(float angle) {
	return angle * M_PI / 180;
}

QSharedPointer<Camera>& Camera::GetInstance()
{
	if (instance.isNull())
		instance = QSharedPointer<Camera>(new Camera(),Camera::destory);
	return instance;
}

void Camera::destory(Camera* camera) {
	delete camera;
}

Camera::Camera(const QVector3D& pos, const QVector3D& up):pos(pos), worldUP(up),direction(-pos.normalized()),up(up),right(QVector3D::crossProduct(direction, worldUP).normalized()),
yaw(-90.0f),pitch(0.0f),speed(10.0f),sensitivity(0.1f),zoom(30.0f) //初始化值
{
}


QMatrix4x4 Camera::getView()
{
    QMatrix4x4 view;
    view.lookAt(pos, pos + direction, up); //调用lookat函数
	return view;
}

void Camera::setSensitivity(float _sensitivity)
{
	sensitivity = _sensitivity;
}

void Camera::setSpeed(float _speed)
{
	speed = _speed;
}

float Camera::getZoom()
{
    return zoom;
}

void Camera::addKey(Qt::Key key)
{
    keys.insert(key);
}

void Camera::removeKey(Qt::Key key)
{
    keys.remove(key);
}

void Camera::setPos(const QVector3D& _pos)
{
    pos = _pos;
}

void Camera::processKeyboard(float deltaTime)
{
    for (auto& key : keys) {
        processKeyboard(key, deltaTime);
    }
}

/*
* 处理键盘
*/
void Camera::processKeyboard(Qt::Key key, float deltaTime)
{
    float velocity = speed * deltaTime;
    if (key == Qt::Key_W)
        pos += direction * velocity;
    if (key == Qt::Key_S)
        pos -= direction * velocity;
    if (key == Qt::Key_A)
        pos -= right * velocity;
    if (key == Qt::Key_D)
        pos += right * velocity;
    if (key == Qt::Key_Space)
        pos += worldUP * velocity;
    if (key == Qt::Key_Shift)
        pos -= worldUP * velocity;
}

/*
* 处理鼠标移动事件
*/
void Camera::processMouseMovement(float xoffset, float yoffset)
{
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;


    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f; //限制上下翻转范围
    updateState();
}

/*
* 处理滚轮事件
*/
void Camera::processMouseScroll(float yoffset)
{
    if (zoom >= 1.0f && zoom <= 45.0f)
        zoom -= yoffset;
    if (zoom > 45.0f)
        zoom = 45.0f;
    if (zoom < 1.0f)
        zoom = 1.0f;
}

/*
* 更新摄像机状态
*/
void Camera::updateState()
{
	QVector3D newDirection;
	newDirection.setX(cos((radians(yaw))) * cos(radians(pitch)));
	newDirection.setY(sin(radians(pitch)));
	newDirection.setZ(sin(radians(yaw)) * cos(radians(pitch)));
	direction = newDirection.normalized();
	right = QVector3D::crossProduct(direction, worldUP).normalized();
	up = QVector3D::crossProduct(right, direction).normalized();
}

