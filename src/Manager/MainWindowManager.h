#ifndef __MAIN_WINDOW_MANAGER__
#define __MAIN_WINDOW_MANAGER__

#include "src/Ui/UIMainWindow.h"
#include "src/Ui/TransformInspector.h"
#include "src/Ui/MaterialInspector.h"
#include "src/ui/RenderOptionInspector.h"
#include "src/ui/SkyboxInspector.h"
#include "src/ui/HelpDialog.h"
#include "src/Data/Skybox.h"
#include <QtConcurrent>
#include <QAction>
#include <QFile>
#include <QSettings>

const int MAXSCENEFILESIZE = 5;

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

	QStack<QString> sceneList;
	QVector<QAction*> actions;

	int currentIndex;  //当前index

	TransformInspector* transformInspector;
	MaterialInspector* materialInspector;
	RenderOptionInspector* renderOptionInspector;
	SkyboxInspector* skyboxInspector;

	void copyLightsModel();
	void setButtonStyle(int index);
	void setStyle(int style = 0);

	void readSettings();
	void saveSettings();
	void setUi();

	void showFPS(int fps);
	void showSceneName(const QString& sceneName);
};

#endif
