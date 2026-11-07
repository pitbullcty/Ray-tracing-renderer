#ifndef __GIZMO__
#define __GIZMO__

#include "thirdparty/libgizmo/IGizmo.h"
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include "src/Data/Model.h"

enum GIZMOTYPE {
	MOVE,
	ROTATE,
	SCALE
};

class Gizmo:public QObject{

	Q_OBJECT

public:
	Gizmo(QOpenGLExtraFunctions* _functions, QOpenGLShaderProgram* _program);
	Gizmo() = default;
	~Gizmo();

	void setSize(int w, int h);
	void setScale(float scale);
	void setCamera(const QMatrix4x4& view, const QMatrix4x4& proj);
	

	void mouseMove(int x, int y);
	bool mouseDown(int x, int y);
	void mouseUp(int x, int y);

	bool isChange(const QMatrix4x4& modelMatrix);
	void Draw();

	GIZMOTYPE getType() const { return type; } //当前工具(移动/旋转/缩放)
	IGizmo::LOCATION getLocation() const { return location; } //当前坐标系(世界/局部)

signals:
	void sendChanged();
	void sendCheckedTransformButton(int id);
	void sendCheckedLocationButton(int id);

public slots:
	void setEditModel(Model* model);
	void setType(GIZMOTYPE type);
	void setLocation(IGizmo::LOCATION location);

private:
	Model* model; //挂载的模型
	IGizmo* gizmo;
	GIZMOTYPE type = MOVE; //当前工具,供提示栏显示
	IGizmo::LOCATION location = IGizmo::LOCATION::LOCATE_WORLD; //当前坐标系
	float  gizmoModel[16], gizmoView[16], gizmoProj[16]; //gizmo观察、投影矩阵
	IGizmo* gizmoMove, * gizmoRotate, *gizmoScale; //相关gizmo组件
	void applyToModel(); //将结果返回模型
	bool checkScale();  //检查模型矩阵
	void setModelMatrix(const QMatrix4x4& model);
};


#endif