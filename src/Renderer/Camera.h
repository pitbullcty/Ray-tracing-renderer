#ifndef __CAMERA__
#define __CAMERA__

#include <QSharedPointer>
#include <QVector3D>
#include <QMatrix4x4>
#include <QJsonObject>

class Camera {
public:
	static QSharedPointer<Camera>& GetInstance();
	static void destory(Camera* camera);

    QMatrix4x4 getView();
    void setSensitivity(float _sensitivity);
    void setSpeed(float _speed);
    void setPos(const QVector3D& _pos);

    float getZoom();
    QVector3D getPos() const;

    void addKey(Qt::Key key);
    void removeKey(Qt::Key key);
    void processKeyboard(float deltaTime);
    void processKeyboard(Qt::Key key, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);
    void processMouseScroll(float yoffset);
    void updateState();
    void reSet();
    QVector3D getDirection();
    QJsonObject toJson();
    void prase(const QJsonObject& camera);

private:
	static QSharedPointer<Camera> instance;
    QSet<Qt::Key> keys;   //存放按键合集

    float yaw;                  //偏航角
    float pitch;                //俯视角
    float sensitivity;          //鼠标灵敏度
    float zoom;               //缩放

    QVector3D pos;        //摄像机初始位置
    QVector3D worldUP;    //世界的上向量

    QVector3D direction;  //摄像机方向
    QVector3D right;      //摄像机右向量，摄像机正x轴方向
    QVector3D up;         //摄像机上向量

    float speed;    //控制移动速度

	~Camera() = default;
    Camera(const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D& up=QVector3D(0.0f,1.0f,0.0f));

};

#endif 
