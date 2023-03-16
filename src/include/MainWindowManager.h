#ifndef __MAIN_WINDOW_MANAGER__
#define __MAIN_WINDOW_MANAGER__

#include "ui_Mainwindow.h"
#include <QtConcurrent>
#include <QAction>
#include <QFile>

class MainWindowManager:public QObject {
	Q_OBJECT

public:
	MainWindowManager(Ui::MainWindow* ui);
	~MainWindowManager();
	void bindSignals();
	void loadModel(const QString& path);
	void crateScene();
	void loadScene(const QString& path);
	void saveScene();
	void saveSceneAS();
	void closeScene();

public slots:
	void loadModelFromAction();
	void loadSceneFromAction();
	void changeRenderWindow();
	void hideRenderWidget(bool isHide);

signals:
	void sendCurrentIndex(int index);

private:
	Ui::MainWindow* ui;
	QString lastModelPath;
	QString lastScenePath;
	QTimer fpsTimer;

	int currentIndex;  //当前index

	void copyLightsModel();
	void showSceneInfo();
	void setButtonStyle(int index);
	void setStyle(int style = 0);

};

#endif
