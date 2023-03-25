#ifndef __MAIN_WINDOW_MANAGER__
#define __MAIN_WINDOW_MANAGER__

#include "src/Ui/UIMainWindow.h"
#include "src/Ui/TransformInspector.h"
#include <QtConcurrent>
#include <QAction>
#include <QFile>
#include <QSettings>

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
	QString lastScenePath; //上次打开位置

	int currentIndex;  //当前index

	void copyLightsModel();
	void setButtonStyle(int index);
	void setStyle(int style = 0);

	void readSettings();
	void saveSettings();

	void showFPS(int fps);
	void showSceneName(const QString& sceneName);
};

#endif
