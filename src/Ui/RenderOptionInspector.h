#ifndef __RENDEROPTION_INSPECTOR__
#define __RENDEROPTION_INSPECTOR__

#include <QWidget>
#include "UiRenderOptionInspector.h"
#include "src/Renderer/RenderOption.h"
#include <QFileDialog>
#include <QDesktopServices>

class RenderOptionInspector :public QWidget {
	Q_OBJECT

public:
	RenderOptionInspector(QWidget* parent = nullptr);
	~RenderOptionInspector();

signals:
	void sendRenderOption(const RenderOption& option);
	void sendRenderState(bool state);
	void sendRealTimeDenoise(bool state); //实时预览降噪开关

public slots:
	void reset();

private:
	Ui::RenderOptionInspector* ui;
	RenderOption option;

	void applyData();
	void setData();

};

#endif // ! __RENDEROPTION_INSPECTOR__
