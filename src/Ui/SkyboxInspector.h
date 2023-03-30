#ifndef __SKYBOX__INSPECTOR__
#define __SKYBOX__INSPECTOR__

#include <QWidget>
#include "UiSkyboxInspector.h"
#include "src/Data/Skybox.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QRegularExpression>
#include <QMessageBox>

class SkyboxInspector:public QWidget {
	Q_OBJECT

public:
	SkyboxInspector(QWidget* parent=nullptr);
	~SkyboxInspector();

signals:
	void sendUpdateSkybox(bool isEmpty);

private:
	Ui::SkyboxInspector* ui;
	QSharedPointer<Skybox> skybox;
	void reset();
	void bindData();
	void setLabelText(QLabel* label, const QString& path);
};


#endif // !__SKYBOX__INSPECTOR
