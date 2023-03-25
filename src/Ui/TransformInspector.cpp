#include "TransformInspector.h"

TransformInspector::TransformInspector(QWidget* parent):
	QWidget(parent),
	ui(new Ui::TransformInspector)
{
	ui->setupUi(this);
}

TransformInspector::~TransformInspector()
{
	delete ui;
}
