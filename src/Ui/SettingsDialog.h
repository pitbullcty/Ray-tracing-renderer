#ifndef __SETTINGS_DIALOG__
#define __SETTINGS_DIALOG__

#include <QDialog>
#include <QSharedPointer>

class RayTracingRenderer;
class QCheckBox;
class QLabel;
class QTimer;

//系统设置对话框: 目前提供"在GPU上动态构建/更新BVH树"的开关与构建耗时对比
class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	SettingsDialog(QSharedPointer<RayTracingRenderer> renderer, QWidget* parent = nullptr);

	bool getUseGPUBVH() const;
	bool getAutoRebuildBVH() const;

private slots:
	void refreshTiming();
	void rebuildBVH();

private:
	QSharedPointer<RayTracingRenderer> renderer;

	QCheckBox* useGPUBVHCheck = nullptr;
	QCheckBox* autoRebuildCheck = nullptr;
	QLabel* supportLabel = nullptr;
	QLabel* timingLabel = nullptr;
	QTimer* refreshTimer = nullptr;
};

#endif // !__SETTINGS_DIALOG__
