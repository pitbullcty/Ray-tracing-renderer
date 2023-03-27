#ifndef __MATERIAL__INSPECTOR__
#define __MATERIAL__INSPECTOR__

#include "UiMaterialInspector.h"
#include <QWidget>
#include "src/Data/Model.h"
#include <QColorDialog>
#include <QtConcurrent>
#include "src/Manager/DataBuilder.h"

class MaterialInspector :public QWidget
{
	Q_OBJECT

public:
	MaterialInspector(QWidget* parent = nullptr);
	~MaterialInspector();
	void setEdittable(bool edittable);

signals:
	void sendRevertModel(Model* model);

public slots:
	void bindModel(Model* model);
	void setData();

private:
	Ui::MaterialInspector* ui;
	Model* model; //挂载的模型

	void clearData();
	void applyData();

};


#endif // !__MATERIAL__INSPECTOR__
