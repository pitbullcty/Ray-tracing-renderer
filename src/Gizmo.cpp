#include "Gizmo.h"

Gizmo::Gizmo(QOpenGLExtraFunctions* _functions, QOpenGLShaderProgram* _program) {
	gizmo = nullptr;
	model = nullptr;
	gizmoMove = CreateMoveGizmo(_functions, _program);
	gizmoRotate = CreateRotateGizmo(_functions, _program);
	gizmoScale = CreateScaleGizmo(_functions, _program);
	memset(gizmoModel, 0 ,sizeof(float)*16);
	memset(gizmoView, 0, sizeof(float) * 16);
	memset(gizmoProj, 0, sizeof(float) * 16);
}

Gizmo::~Gizmo() {
	delete gizmoMove;
	delete gizmoRotate;
	delete gizmoScale;
	gizmo = gizmoRotate = gizmoMove = gizmoScale = nullptr;
}

void Gizmo::setModelMatrix(const QMatrix4x4& model)
{
	model.transposed().copyDataTo(gizmoModel);
	gizmoMove->SetEditMatrix(gizmoModel);
	gizmoMove->SetLocation(IGizmo::LOCATE_WORLD);
	gizmoRotate->SetEditMatrix(gizmoModel);
	gizmoRotate->SetLocation(IGizmo::LOCATE_WORLD);
	gizmoScale->SetEditMatrix(gizmoModel);
	gizmoScale->SetLocation(IGizmo::LOCATE_WORLD);
}

void Gizmo::setEditModel(Model* model)
{
	this->model = model;
	QMatrix4x4 gizmoModel = model->transform.getModel(); //获取初始模型矩阵
	gizmoModel.translate(model->getCenter()); //转移至中心位置
	setModelMatrix(gizmoModel);
}

void Gizmo::applyToModel()
{
	QMatrix4x4 newmodel(gizmo->getEditMatrix());
	newmodel = newmodel.transposed();
	newmodel.translate(-model->getCenter()); //往回移动
	model->transform.setModel(newmodel);
}

void Gizmo::setSize(int w, int h)
{
	gizmoMove->SetScreenDimension(w, h);
	gizmoRotate->SetScreenDimension(w, h);
	gizmoScale->SetScreenDimension(w, h);
}

void Gizmo::setScale(float scale)
{
	gizmoMove->SetDisplayScale(scale);
	gizmoRotate->SetDisplayScale(scale);
	gizmoScale->SetDisplayScale(scale);
}

void Gizmo::setCamera(const QMatrix4x4& view, const QMatrix4x4& proj)
{
	view.transposed().copyDataTo(gizmoView);
	proj.transposed().copyDataTo(gizmoProj);
	gizmo->SetCameraMatrix(gizmoView, gizmoProj);
}

void Gizmo::setType(GIZMO_TYPE type)
{
	if (type == MOVE) {
		gizmo = gizmoMove;
	}
	else if (type == ROTATE) {
		gizmo = gizmoRotate;
	}
	else {
		gizmo = gizmoScale;
	}
}

void Gizmo::mouseMove(int x, int y)
{
	gizmo->OnMouseMove(x, y);
	applyToModel();
}

bool Gizmo::mouseDown(int x, int y)
{
	return gizmo->OnMouseDown(x, y);
}

void Gizmo::mouseUp(int x, int y)
{
	gizmo->OnMouseUp(x, y);
}

void Gizmo::Draw()
{
	gizmo->Draw();
}
