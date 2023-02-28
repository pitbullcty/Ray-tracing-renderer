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
#include <QTemporaryDir>

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
	void sendAddPath(const QString& path);
	void sendLookAtName(const QString& name);

public slots:
	void updateList(QMap<QString, Model>* models);
	void rename();
	void remove();
	void copy();
	void paste();
	void add();
	void lookAt();
	void addRectLight();
	void addSphereLight();

private:
	QListWidgetItem* current;
	QString lightPath;
	QString copyName; //复制文件名
	QString tempRectFile; //临时文件名
	QString tempSphereFile; //临时文件名
};

#endif