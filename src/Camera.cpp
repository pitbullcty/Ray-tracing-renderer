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

Camera::Camera(const QVector3D& pos, const QVector3D& up):pos(pos), 
    worldUP(up),
    direction(-pos.normalized()),
    up(up),
    right(QVector3D::crossProduct(direction, worldUP).normalized()),
    yaw(-90.0f),
    pitch(0.0f),
    speed(10.0f),
    sensitivity(0.1f),
    zoom(30.0f) //初始化值
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

QVector3D Camera::getPos() const
{
    return pos;
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
	direction = getDirection().normalized();
	right = QVector3D::crossProduct(direction, worldUP).normalized();
	up = QVector3D::crossProduct(right, direction).normalized();
}

void Camera::reSet()
{
    pos = { 0,0,0 };
    worldUP = { 0,1,0 };
    direction = -pos.normalized();
    up = { 0,1,0 };
    right = QVector3D::crossProduct(direction, worldUP).normalized();
    yaw = -90.0f;
    pitch = 0.0f;
    speed = 10.0f;
    sensitivity = 0.1f;
    zoom = 30.0f;
}

QVector3D Camera::getDirection()
{
    return QVector3D{ -sin(radians(yaw)) * cos(radians(pitch)) , sin(radians(pitch)) ,cos((radians(yaw))) * cos(radians(pitch)) };
}

QJsonObject Camera::toJson()
{
    QJsonObject camera;
    camera.insert("x", pos.x());
    camera.insert("y", pos.y());
    camera.insert("z", pos.z());
    camera.insert("yaw", yaw);
    camera.insert("pitch", pitch);
    return camera;
}

void Camera::prase(const QJsonObject& camera)
{
    float x = camera["x"].toVariant().toFloat();
    float y = camera["y"].toVariant().toFloat();
    float z = camera["z"].toVariant().toFloat();
    pitch = camera["pitch"].toVariant().toFloat();
    yaw = camera["yaw"].toVariant().toFloat();
    setPos({ x,y,z });
    updateState();
}

