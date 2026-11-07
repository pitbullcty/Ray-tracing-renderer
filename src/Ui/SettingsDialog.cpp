#include "SettingsDialog.h"
#include "src/Renderer/RayTracingRenderer.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QElapsedTimer>
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QTimer>
#include <QVBoxLayout>

SettingsDialog::SettingsDialog(QSharedPointer<RayTracingRenderer> renderer, QWidget* parent) :
	QDialog(parent), renderer(renderer)
{
	setWindowTitle(QString::fromUtf8("系统设置"));
	setMinimumWidth(430);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	QLabel* titleLabel = new QLabel(QString::fromUtf8("BVH 构建方式"), this);
	QFont titleFont = titleLabel->font();
	titleFont.setBold(true);
	titleLabel->setFont(titleFont);
	mainLayout->addWidget(titleLabel);

	supportLabel = new QLabel(this);
	supportLabel->setWordWrap(true);
	supportLabel->setText(QString::fromUtf8("GPU 状态: 尚未检测(加载场景后自动检测)"));
	mainLayout->addWidget(supportLabel);

	useGPUBVHCheck = new QCheckBox(QString::fromUtf8("启用 GPU 动态构建 BVH 树"), this);
	useGPUBVHCheck->setToolTip(QString::fromUtf8("使用计算着色器在显卡上构建 BVH,构建完成后由光追着色器直接使用"));
	mainLayout->addWidget(useGPUBVHCheck);

	autoRebuildCheck = new QCheckBox(QString::fromUtf8("场景变化时自动在 GPU 上重建 BVH"), this);
	autoRebuildCheck->setToolTip(QString::fromUtf8("关闭后,场景改变时保留旧的 BVH,需要手动点击下面的按钮重建"));
	mainLayout->addWidget(autoRebuildCheck);

	QHBoxLayout* timingLayout = new QHBoxLayout();
	timingLabel = new QLabel(this);
	timingLabel->setWordWrap(true);
	timingLabel->setText(QString::fromUtf8("尚未构建"));
	timingLayout->addWidget(timingLabel);
	timingLayout->addStretch();
	QPushButton* rebuildButton = new QPushButton(QString::fromUtf8("重建并对比耗时"), this);
	timingLayout->addWidget(rebuildButton);
	mainLayout->addLayout(timingLayout);

	QLabel* tipLabel = new QLabel(QString::fromUtf8("说明: 对比数据会在光追页面渲染一帧后更新,同时输出到控制台。"), this);
	tipLabel->setWordWrap(true);
	QFont tipFont = tipLabel->font();
	tipFont.setPointSize(9);
	tipLabel->setFont(tipFont);
	mainLayout->addWidget(tipLabel);

	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
	buttons->button(QDialogButtonBox::Ok)->setText(QString::fromUtf8("确定"));
	buttons->button(QDialogButtonBox::Cancel)->setText(QString::fromUtf8("取消"));
	mainLayout->addWidget(buttons);

	QLabel* noteLabel = new QLabel(QString::fromUtf8("GPU 构建需要 OpenGL 4.3 及以上;若不可用会自动回退到 CPU。"), this);
	noteLabel->setWordWrap(true);
	QFont noteFont = noteLabel->font();
	noteFont.setPointSize(9);
	noteLabel->setFont(noteFont);
	mainLayout->addWidget(noteLabel);

	//读取已保存的设置
	QSettings settings("settings.ini", QSettings::IniFormat);
	useGPUBVHCheck->setChecked(settings.value("bvh/useGPU", false).toBool());
	autoRebuildCheck->setChecked(settings.value("bvh/autoRebuild", true).toBool());
	autoRebuildCheck->setEnabled(useGPUBVHCheck->isChecked());
	if (renderer != nullptr && renderer->getUseGPUBVH() != useGPUBVHCheck->isChecked())
		useGPUBVHCheck->setChecked(renderer->getUseGPUBVH());

	connect(useGPUBVHCheck, &QCheckBox::toggled, this, [this](bool checked) {
		autoRebuildCheck->setEnabled(checked);
	});
	connect(rebuildButton, &QPushButton::clicked, this, &SettingsDialog::rebuildBVH);
	connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
		//保存设置并立即应用
		QSettings settings("settings.ini", QSettings::IniFormat);
		settings.setValue("bvh/useGPU", useGPUBVHCheck->isChecked());
		settings.setValue("bvh/autoRebuild", autoRebuildCheck->isChecked());
		if (this->renderer != nullptr) {
			this->renderer->setUseGPUBVH(useGPUBVHCheck->isChecked());
			this->renderer->setAutoRebuildBVH(autoRebuildCheck->isChecked());
			if (useGPUBVHCheck->isChecked())
				this->renderer->setPendingBVHRebuild(true); //在GL线程里重建
		}
		accept();
	});
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

	refreshTimer = new QTimer(this);
	refreshTimer->setInterval(400);
	connect(refreshTimer, &QTimer::timeout, this, &SettingsDialog::refreshTiming);
	refreshTimer->start();
	refreshTiming();
}

bool SettingsDialog::getUseGPUBVH() const
{
	return useGPUBVHCheck->isChecked();
}

bool SettingsDialog::getAutoRebuildBVH() const
{
	return autoRebuildCheck->isChecked();
}

void SettingsDialog::refreshTiming()
{
	if (renderer == nullptr) {
		supportLabel->setText(QString::fromUtf8("GPU 状态: 渲染器尚未初始化"));
		return;
	}

	if (renderer->isGPUBVHReady())
		supportLabel->setText(QString::fromUtf8("GPU 状态: ") + renderer->getGPUBVHStatus());
	else if (renderer->getUseGPUBVH())
		supportLabel->setText(QString::fromUtf8("GPU 状态: 等待首次构建后检测"));
	else
		supportLabel->setText(QString::fromUtf8("GPU 状态: 已关闭(当前使用 CPU 构建)"));

	const double cpuMs = renderer->getLastCPUBVHMs();
	const double gpuMs = renderer->getLastGPUBVHMs();
	const int gpuTris = renderer->getGPUBVHTriangleCount();
	if (gpuTris <= 0 && cpuMs <= 0.0) {
		timingLabel->setText(QString::fromUtf8("尚未构建"));
		return;
	}

	QString text = QString::fromUtf8("三角形数量: %1\nCPU 构建耗时: %2 ms")
		.arg(gpuTris > 0 ? gpuTris : DataBuilder::GetInstance()->getTriangleCount())
		.arg(cpuMs, 0, 'f', 2);
	if (gpuMs > 0.0) {
		const double speedup = gpuMs > 0.0 ? cpuMs / gpuMs : 0.0;
		text += QString::fromUtf8("\nGPU 构建耗时: %1 ms (加速比 %2x)")
			.arg(gpuMs, 0, 'f', 2).arg(speedup, 0, 'f', 2);
	}
	timingLabel->setText(text);
}

void SettingsDialog::rebuildBVH()
{
	if (renderer == nullptr) {
		timingLabel->setText(QString::fromUtf8("渲染器尚未初始化"));
		return;
	}
	if (!useGPUBVHCheck->isChecked()) {
		timingLabel->setText(QString::fromUtf8("请先勾选“启用 GPU 动态构建 BVH 树”"));
		return;
	}

	QSettings settings("settings.ini", QSettings::IniFormat);
	settings.setValue("bvh/useGPU", true);
	settings.setValue("bvh/autoRebuild", autoRebuildCheck->isChecked());
	renderer->setUseGPUBVH(true);
	renderer->setAutoRebuildBVH(autoRebuildCheck->isChecked());
	renderer->setPendingBVHRebuild(true);

	timingLabel->setText(QString::fromUtf8("已请求重建,等待下一帧完成(也可查看控制台输出)"));
}

