#include "Gizmo.h"

Gizmo::Gizmo(QOpenGLExtraFunctions* _functions, QOpenGLShaderProgram* _program) :
	gizmo(nullptr),
	model(nullptr),
	gizmoMove(CreateMoveGizmo(_functions, _program)),
	gizmoRotate(CreateRotateGizmo(_functions, _program)),
	gizmoScale(CreateScaleGizmo(_functions, _program))
{
	gizmoMove->SetLocation(IGizmo::LOCATE_WORLD);
	gizmoRotate->SetLocation(IGizmo::LOCATE_WORLD);
	gizmoScale->SetLocation(IGizmo::LOCATE_WORLD);
	memset(gizmoModel, 0, sizeof(float) * 16);
	memset(gizmoView, 0, sizeof(float) * 16);
	memset(gizmoProj, 0, sizeof(float) * 16);
}

Gizmo::~Gizmo() {
	delete gizmoMove;
	delete gizmoRotate;
	delete gizmoScale;
	gizmo = gizmoRotate = gizmoMove = gizmoScale = nullptr;
	model = nullptr;
}

void Gizmo::setModelMatrix(const QMatrix4x4& model)
{
	model.transposed().copyDataTo(gizmoModel);
	gizmoMove->SetEditMatrix(gizmoModel);
	gizmoRotate->SetEditMatrix(gizmoModel);
	gizmoScale->SetEditMatrix(gizmoModel);
	gizmoMove->setSvgMatrix();
	gizmoRotate->setSvgMatrix();
	gizmoScale->setSvgMatrix();
}

void Gizmo::setEditModel(Model* model)
{
	this->model = model;
	if (!model) return;
	QMatrix4x4 gizmoModel ; //获取初始模型矩阵
	gizmoModel = model->transform.getModel();
	QMatrix4x4 trans;
	trans.translate(model->getCenter());
	gizmoModel = trans * gizmoModel; //转移至中心位置
	setModelMatrix(gizmoModel);
}

void Gizmo::applyToModel()
{
	QMatrix4x4 newmodel(gizmo->getEditMatrix());
	newmodel = newmodel.transposed();
	QMatrix4x4 trans;
	trans.translate(-model->getCenter());
	newmodel = trans * newmodel; //往回移动
	model->transform.setModel(newmodel);
	model->updateBound(); //更新包围盒
}

bool Gizmo::checkScale()
{
	if (abs((model->transform.scaleX - model->transform.initScaleX)) > 1e-6 || abs((model->transform.scaleZ - model->transform.initScaleZ)) > 1e-6
		|| abs((model->transform.scaleY - model->transform.initScaleY)) > 1e-6) {
		model->transform.scaleX = model->transform.initScaleX;
		model->transform.scaleY = model->transform.initScaleY;
		model->transform.scaleZ = model->transform.initScaleZ;
		return true;
	}
	return false;
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

void Gizmo::setType(GIZMOTYPE type)
{
	if (type == MOVE) {
		gizmo = gizmoMove;
		sendCheckedTransformButton(-2);
	}
	else if (type == ROTATE) {
		gizmo = gizmoRotate;
		sendCheckedTransformButton(-3);
	}
	else {
		gizmo = gizmoScale;
		sendCheckedTransformButton(-4);
	}
}

void Gizmo::mouseMove(int x, int y)
{
	if (!model) return;
	gizmo->OnMouseMove(x, y);  //采用消除旋转策略
	applyToModel();
	emit sendChanged();
}

bool Gizmo::mouseDown(int x, int y)
{
	if (!model) return false;

	if (gizmo->OnMouseDown(x, y)) {
		if (gizmo == gizmoRotate) {
			if (checkScale()) {
				model->transform.calcModel();
				auto modelMatrix = model->transform.getModel();
				auto trans = QMatrix4x4();
				trans.translate(model->getCenter());
				modelMatrix = trans * modelMatrix;
				setModelMatrix(modelMatrix);
			}
		} 
		return true;
	}
	return false;
}

void Gizmo::mouseUp(int x, int y)
{
	if (!model) return;
	gizmo->OnMouseUp(x, y);
}

void Gizmo::setLocation(IGizmo::LOCATION location)
{
	if (location == IGizmo::LOCATION::LOCATE_WORLD) emit sendCheckedLocationButton(-2);
	else emit sendCheckedLocationButton(-1);

	gizmo->SetLocation(location);
}

bool Gizmo::isChange(const QMatrix4x4& modelMatrix)
{
	return QMatrix4x4(gizmo->getEditMatrix()).transposed() != modelMatrix;
}

void Gizmo::Draw()
{
	if(!model) return;
	gizmo->Draw();
}
