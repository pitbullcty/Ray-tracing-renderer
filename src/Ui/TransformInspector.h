#ifndef __TRANSFORMINSPECTOR__
#define __TRANSFORMINSPECTOR__

#include "UiTransformInspector.h"
#include <QWidget>

class TransformInspector:public QWidget
{
	Q_OBJECT
public:
	TransformInspector(QWidget* parent = nullptr);
	~TransformInspector();

private:
	Ui::TransformInspector* ui;
};



#endif // !__TRANSFORMINSPECTOR__
