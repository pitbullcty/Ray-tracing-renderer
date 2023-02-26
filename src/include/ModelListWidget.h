#ifndef __MODEL_LIST_WIDGET__
#define __MODEL_LIST_WIDGET__

#include <QListWidget>
#include "Model.h"
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QInputDialog>
#include <QMessageBox>

class ModelListWidget:public QListWidget {
	Q_OBJECT

public:
	ModelListWidget(QWidget* parent = 0);
	~ModelListWidget();

protected:
	virtual void contextMenuEvent(QContextMenuEvent* event);

signals:
	void sendNewname(const QString& oldname, const QString& newname);

public slots:
	void updateList(QMap<QString, Model>* models);
	void rename();

private:
	QListWidgetItem* current;
};

#endif