#include "ModelListWidget.h"

ModelListWidget::ModelListWidget(QWidget* parent):QListWidget(parent), models(nullptr)
{
	setStyleSheet("QListView::item {height: 25px;font-size:5px;}");
}

ModelListWidget::~ModelListWidget()
{
	models = nullptr;
}

void ModelListWidget::setModels(QMap<QString, Model>* models)
{
	this->models = models;
}

void ModelListWidget::updateList() {
	if (!models) return;
	clear();
	for (auto it = models->begin(); it != models->end(); it++) {
		addItem(it.key());
	}
}


