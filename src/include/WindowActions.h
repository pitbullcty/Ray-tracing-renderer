#ifndef __WINDOW_ACTION__
#define __WINDOW_ACTION__

#include "ui_Mainwindow.h"
#include "SceneManager.h"
#include <QAction>

class WindowActions:public QObject {
	Q_OBJECT

public:
	WindowActions(Ui::MainWindow* ui);
	~WindowActions();
	void bind();

private:
	Ui::MainWindow* ui;
	QSharedPointer<SceneManager> sceneManager;
	void loadModel();
	void crateScene();
	void loadScene();
	void saveScene();
	void saveSceneAS();
	void closeScene();
};

#endif
