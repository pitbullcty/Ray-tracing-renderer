﻿#ifndef __MODEL_LIST_WIDGET__
#define __MODEL_LIST_WIDGET__

#include <QListWidget>
#include "src/Data/Model.h"
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
	void sendSelectedName(const QString& name);
	void sendRevert();

public slots:
	void updateList(QMap<QString, Model>* models, Model* model);
	void rename();
	void remove();
	void copy();
	void paste();
	void add();
	void lookAt();
	void addRectLight();
	void addSphereLight();
	void getSelectedName();
	void receiveIndex(int index);

private:
	QListWidgetItem* current;
	int currentIndex; //当前使用的窗口
	QString copyName; //复制文件名
};

#endif