#ifndef __CAMERA__
#define __CAMERA__

#include <QSharedPointer>

class Camera {
public:
	static QSharedPointer<Camera>& GetInstance();
	static void destory(Camera* camera);

private:
	static QSharedPointer<Camera> instance;
	~Camera() = default;
	Camera() = default;

};

#endif 
