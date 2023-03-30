#include "SkyboxInspector.h"

QString getPlainText(const QString& text) {
	QRegularExpression reg("<b>(.*)</b>(.*)");
	return reg.match(text).captured(1);
}

SkyboxInspector::SkyboxInspector(QWidget* parent):
	QWidget(parent),
	ui(new Ui::SkyboxInspector),
	skybox(Skybox::GetInstance())
{
	ui->setupUi(this);
	reset();
	bindData();
}

void SkyboxInspector::bindData()
{
	connect(ui->pushButtonReset, &QPushButton::clicked, [=]() {
		reset();
		emit sendUpdateSkybox(false);
	});
	connect(ui->checkBox, &QCheckBox::clicked, [=]() {
		if (!ui->checkBox->isChecked()) {
			ui->skyboxwidget->hide();
			emit sendUpdateSkybox(true);
		}
		else {
			ui->skyboxwidget->show();
			emit sendUpdateSkybox(false);
		}
	});	
	connect(ui->pushButtonEditAll, &QPushButton::clicked, [=]() {
		QString path = getPlainText(ui->labelPX->toolTip());
		if (path == "默认图片") path = QDir::currentPath();
		auto imagePathes = QFileDialog::getOpenFileNames(this, "打开图片", path, tr("图片(*.png *.jpg)"));
		if (imagePathes.size() < 6) {
			QMessageBox::warning(this, "警告", "请选择6张图片！", QMessageBox::Yes);
			return;
		}
		else {
			setLabelText(ui->labelPX, imagePathes[0]);
			setLabelText(ui->labelNX, imagePathes[1]);
			setLabelText(ui->labelPY, imagePathes[2]);
			setLabelText(ui->labelNY, imagePathes[3]);
			setLabelText(ui->labelPZ, imagePathes[4]);
			setLabelText(ui->labelNZ, imagePathes[5]);
			skybox->pathes["+x"] = imagePathes[0];
			skybox->pathes["-x"] = imagePathes[1];
			skybox->pathes["+y"] = imagePathes[2];
			skybox->pathes["-y"] = imagePathes[3];
			skybox->pathes["+z"] = imagePathes[4];
			skybox->pathes["-z"] = imagePathes[5];
			emit sendUpdateSkybox(false);
		}
	});
	connect(ui->toolButtonPX, &QToolButton::clicked, [=]() {
		QString path = getPlainText(ui->labelPX->toolTip());
		if (path == "默认图片") path = QDir::currentPath();
		QString imagePath = QFileDialog::getOpenFileName(this, "打开图片", path, tr("图片(*.png *.jpg)"));
		if (!imagePath.isEmpty()) {
			setLabelText(ui->labelPX, imagePath);
			skybox->pathes["+x"] = imagePath;
			emit sendUpdateSkybox(false);
		}
	});
	connect(ui->toolButtonNX, &QToolButton::clicked, [=]() {
		QString path = getPlainText(ui->labelNX->toolTip());
		if (path == "默认图片") path = QDir::currentPath();
		QString imagePath = QFileDialog::getOpenFileName(this, "打开图片", path, tr("图片(*.png *.jpg)"));
		if (!imagePath.isEmpty()) {
			setLabelText(ui->labelNX, imagePath);
			skybox->pathes["-x"] = imagePath;
			emit sendUpdateSkybox(false);
		}
	});
	connect(ui->toolButtonPY, &QToolButton::clicked, [=]() {
		QString path = getPlainText(ui->labelPY->toolTip());
		if (path == "默认图片") path = QDir::currentPath();
		QString imagePath = QFileDialog::getOpenFileName(this, "打开图片", path, tr("图片(*.png *.jpg)"));
		if (!imagePath.isEmpty()) {
			setLabelText(ui->labelPY, imagePath);
			skybox->pathes["+y"] = imagePath;
			emit sendUpdateSkybox(false);
		}
	});
	connect(ui->toolButtonNY, &QToolButton::clicked, [=]() {
		QString path = getPlainText(ui->labelNY->toolTip());
		if (path == "默认图片") path = QDir::currentPath();
		QString imagePath = QFileDialog::getOpenFileName(this, "打开图片", path, tr("图片(*.png *.jpg)"));
		if (!imagePath.isEmpty()) {
			setLabelText(ui->labelNY, imagePath);
			skybox->pathes["-y"] = imagePath;
			emit sendUpdateSkybox(false);
		}
	});
	connect(ui->toolButtonPZ, &QToolButton::clicked, [=]() {
		QString path = getPlainText(ui->labelPZ->toolTip());
		if (path == "默认图片") path = QDir::currentPath();
		QString imagePath = QFileDialog::getOpenFileName(this, "打开图片", path, tr("图片(*.png *.jpg)"));
		if (!imagePath.isEmpty()) {
			setLabelText(ui->labelPZ, imagePath);
			skybox->pathes["+z"] = imagePath;
			emit sendUpdateSkybox(false);
		}
	});
	connect(ui->toolButtonNZ, &QToolButton::clicked, [=]() {
		QString path = getPlainText(ui->labelNZ->toolTip());
		if (path == "默认图片") path = QDir::currentPath();
		QString imagePath = QFileDialog::getOpenFileName(this, "打开图片", path, tr("图片(*.png *.jpg)"));
		if (!imagePath.isEmpty()) {
			setLabelText(ui->labelNZ, imagePath);
			skybox->pathes["-z"] = imagePath;
			emit sendUpdateSkybox(false);
		}
	});
}

void SkyboxInspector::setLabelText(QLabel* label, const QString& path)
{
	QString toolTip("<b>%1</b><br><img src=\"%2\" height=\"100\" width=\"100\"/>");
	QString text("<b>%1</b>");

	QString newpath = path;
	QFileInfo info(newpath); 

	if (path.startsWith(":/")) newpath = "默认图片";

	label->setText(text.arg(info.fileName()));
	label->setToolTip(toolTip.arg(newpath).arg(path));
}


SkyboxInspector::~SkyboxInspector()
{
	delete ui;
}

void SkyboxInspector::reset()
{
	skybox->reset();
	ui->checkBox->setChecked(true);

	QString toolTip("<b>%1</b><br><img src=\"%2\" height=\"100\" width=\"100\"/>");
	QString text("<b>%1</b>");

	setLabelText(ui->labelNX, skybox->pathes["-x"]);
	setLabelText(ui->labelPX, skybox->pathes["+x"]);

	setLabelText(ui->labelNY, skybox->pathes["-y"]);
	setLabelText(ui->labelPY, skybox->pathes["+y"]);

	setLabelText(ui->labelNZ, skybox->pathes["-z"]);
	setLabelText(ui->labelPZ, skybox->pathes["+z"]);

}
