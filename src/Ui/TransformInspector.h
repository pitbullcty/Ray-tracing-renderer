#ifndef __TRANSFORM__INSPECTOR__
#define __TRANSFORM__INSPECTOR__

#include "UiTransformInspector.h"
#include <QWidget>
#include "src/Data/Model.h"
#include <QtConcurrent>
#include "src/Manager/DataBuilder.h"
#include "src/Renderer/Gizmo.h"

class TransformInspector:public QWidget
{
	Q_OBJECT

public:
	TransformInspector(QWidget* parent = nullptr);
	~TransformInspector();
	void setEdittable(bool edittable);

signals:
	void sendEditModel(Model* model);
	void sendRevertModel(Model* model);
	void sendGizmoType(GIZMOTYPE type);
	void sendLocationType(IGizmo::LOCATION location);

public slots:
	void bindModel(Model* model);
	void setData();
	void setCheckedTransformButton(int id);
	void setCheckedLocationButton(int id);

private:
	Ui::TransformInspector* ui;
	Model* model; //挂载的模型

	void clearData();
	void applyData();
	void upDateModel();

	void getCheckedTransformType();
	void getCheckedLocationType();
};



#endif // !__TRANSFORMINSPECTOR__
