#include "RenderOptionInspector.h"

QString getOutputText(const QString& path) {
	QFileInfo info(path);
	return info.fileName();
}

RenderOptionInspector::RenderOptionInspector(QWidget* parent):
	QWidget(parent),
	ui(new Ui::RenderOptionInspector)
{
	ui->setupUi(this);
	setData();
	applyData();
}

RenderOptionInspector::~RenderOptionInspector()
{
	delete ui;
}

void RenderOptionInspector::reset()
{
	option.reset();
	ui->horizontalSliderMaxFrameCount->setValue(option.maxFrameCounter);
	ui->horizontalSliderDepth->setValue(option.depth);
	ui->horizontalSliderDenoiserStep->setValue(option.denoiserStep);
	ui->checkBox->setChecked(false);
	ui->comboBoxQuality->setCurrentIndex(1);
	ui->comboBoxResolution->setCurrentIndex(1);
	ui->labelOutputPath->setText(getOutputText(option.outputPath));
	ui->labelOutputPath->setToolTip(option.outputPath);
	ui->denoiserCount->hide();
}

void RenderOptionInspector::applyData()
{

	connect(ui->horizontalSliderMaxFrameCount, &QSlider::valueChanged, [=]() {
		auto value = ui->horizontalSliderMaxFrameCount->value();
		option.maxFrameCounter = value;
		ui->labelMaxFrameCount->setText(QString::number(value));
	});

	connect(ui->horizontalSliderDepth, &QSlider::valueChanged, [=]() {
		auto value = ui->horizontalSliderDepth->value();
		option.depth = value;
		ui->labelDepth->setText(QString::number(value));
	});

	connect(ui->horizontalSliderDenoiserStep, &QSlider::valueChanged, [=]() {
		auto value = ui->horizontalSliderDenoiserStep->value();
		option.denoiserStep = value;
		ui->labelDenoiserStep->setText(QString::number(value));
	});

	connect(ui->toolButton, &QToolButton::clicked, [=]() {
		QString savePath = QFileDialog::getSaveFileName(this, "选择保存路径", option.outputPath, "图片(*.jpg;*.png)");
		if (!savePath.isEmpty()) {
			ui->labelOutputPath->setText(getOutputText(savePath));
			ui->labelOutputPath->setToolTip(savePath);
			option.outputPath = savePath;
		}
	});

	connect(ui->checkBox, &QCheckBox::clicked, [=](){
		if (ui->checkBox->isChecked()) {
			ui->denoiserCount->show();
			option.isRealTimeDenoising = true;
		}
		else {
			option.isRealTimeDenoising = false;
			ui->denoiserCount->hide();
		}
	});

	connect(ui->comboBoxResolution, &QComboBox::activated, [=]() {
		QString text = ui->comboBoxResolution->currentText();
		auto list = text.split("×");
		option.resolutionX = list[0].toInt();
		option.resolutionY = list[1].toInt();
	});

	connect(ui->comboBoxQuality, &QComboBox::activated, [=]() {
		QString text = ui->comboBoxQuality->currentText();
		if (text == "低") {
			option.quality = 25;
		}
		else if (text == "中") {
			option.quality = 50;
		}
		else if (text == "高") {
			option.quality = 75;
		}
		else {
			option.quality = 100;
		}
	});

	connect(ui->pushButton, &QPushButton::clicked, [=]() {
		emit sendRenderOption(option);
	});

	connect(ui->pushButtonReset, &QPushButton::clicked, this, &RenderOptionInspector::reset);

}

void RenderOptionInspector::setData()
{
	ui->comboBoxResolution->addItem("640×480");
	ui->comboBoxResolution->addItem("1280×720");
	ui->comboBoxResolution->addItem("1920×1080");
	ui->comboBoxResolution->addItem("2560×1440");
	ui->comboBoxResolution->addItem("4096×2160");
	ui->comboBoxResolution->setCurrentIndex(1);

	ui->comboBoxQuality->addItem("低");
	ui->comboBoxQuality->addItem("中");
	ui->comboBoxQuality->addItem("高");
	ui->comboBoxQuality->addItem("最高");
	ui->comboBoxQuality->setCurrentIndex(1);

	ui->labelOutputPath->setText(getOutputText(option.outputPath));
	ui->labelOutputPath->setToolTip(option.outputPath);
	ui->denoiserCount->hide();

}
