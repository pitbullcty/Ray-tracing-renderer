#ifndef __MODEL_LIST_WIDGET__
#define __MODEL_LIST_WIDGET__

#include <QListWidget>
#include "Model.h"
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>

class ModelListWidget:public QListWidget {
	Q_OBJECT

public:
	ModelListWidget(QWidget* parent = 0);
	~ModelListWidget();

protected:
	virtual void contextMenuEvent(QContextMenuEvent* event) override;
	virtual void keyPressEvent(QKeyEvent* event) override;

signals:
	void sendNewname(const QString& oldname, const QString& newname);
	void sendRemoveName(const QString& name);
	void sendCopyName(const QString& name);
	void sendAddPath();
	void sendLookAtName(const QString& name);

public slots:
	void updateList(QMap<QString, Model>* models);
	void rename();
	void remove();
	void copy();
	void paste();
	void add();
	void lookAt();

private:
	QListWidgetItem* current;
	QString copyName;
};

#endif