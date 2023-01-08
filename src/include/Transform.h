#ifndef __TRANSFORM__
#define __TRANSFORM__

#include<QVector3D>
#include <QMatrix4x4>

class Transform {
public:
	QVector3D originPos; //导入的原始位置
	float scaleX;
	float scaleY;
	float scaleZ; //放大系数
	float rotationX;
	float rotationY;
	float rotationZ; //旋转角度
	float translationX;
	float translationY;
	float translationZ; //模型平移
	Transform();
	~Transform() = default;
	QMatrix4x4 getModel();
	void setOriginPos(const QVector3D& pos);
	
private:
	QMatrix4x4 model; //模型矩阵
	
};

#endif 