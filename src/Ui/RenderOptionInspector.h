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

private:
	Ui::RenderOptionInspector* ui;
	RenderOption option;

	void applyData();
	void setData();

};

#endif // ! __RENDEROPTION_INSPECTOR__
