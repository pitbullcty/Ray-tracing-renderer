#ifndef __WINDOW_ACTION__
#define __WINDOW_ACTION__

#include "ui_Mainwindow.h"
#include<QtConcurrent>
#include <QAction>

class WindowActions:public QObject {
	Q_OBJECT

public:
	WindowActions(Ui::MainWindow* ui);
	~WindowActions();
	void bind();
	void loadModel();
	void crateScene();
	void loadScene();
	void saveScene();
	void saveSceneAS();
	void closeScene();

private:
	Ui::MainWindow* ui;
	
};

#endif
