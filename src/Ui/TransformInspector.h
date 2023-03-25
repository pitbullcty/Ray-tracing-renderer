#ifndef __TRANSFORMINSPECTOR__
#define __TRANSFORMINSPECTOR__

#include "UiTransformInspector.h"
#include <QWidget>
#include "src/Data/Model.h"

class TransformInspector:public QWidget
{
	Q_OBJECT

public:
	TransformInspector(QWidget* parent = nullptr);
	~TransformInspector();

signals:
	void sendEditModel(Model* model);
	void sendRevertModel(Model* model);

public slots:
	void bindModel(Model* model);
	void setData();

private:
	Ui::TransformInspector* ui;
	Model* model; //挂载的模型

	void clearData();
	void applyData();

};



#endif // !__TRANSFORMINSPECTOR__
