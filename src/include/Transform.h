#ifndef __TRANSFORM__
#define __TRANSFORM__

#include<QVector3D>
#include <QMatrix4x4>
#include <QJsonObject>

class Transform {
public:
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
	void calcModel(); //通过系数计算相应
	void reSet();
	QVector3D getTranslation();
	QMatrix4x4 getModel();  //获取模型
	void setModel(const QMatrix4x4& _model);  //设置模型,反过来计算对应系数
	QJsonObject toJson();
	
private:
	QMatrix4x4 model; //模型矩阵
	
};

#endif 