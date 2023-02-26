#ifndef __MODEL_LIST_WIDGET__
#define __MODEL_LIST_WIDGET__

#include <QListWidget>
#include "Model.h"

class ModelListWidget:public QListWidget {
	Q_OBJECT

public:
	ModelListWidget(QWidget* parent = 0);
	~ModelListWidget();
	void setModels(QMap<QString, Model>* models);

public slots:
	void updateList();

private:
	QMap<QString, Model>* models; //模型
};

#endif