#include "MaterialInspector.h"

QColor QVector3D2QColor(const QVector3D& v) {
	return QColor(v.x(), v.y(), v.z());
}

QVector3D QColor2QVector3D(const QColor& v) {
	return QVector3D(v.red(), v.green(), v.blue());
}

QColor Emissive2Color(const QVector3D& emissive) {
	float factor = std::max(std::max(emissive.x(), emissive.y()), emissive.z());
	if (factor != 0) {
		QVector3D newEmissive = emissive / factor * 255;
		return QVector3D2QColor(newEmissive);
	}
}

QVector3D Color2Emissive(const QColor& color, int factor) {
	auto colorV = QColor2QVector3D(color);
	return colorV / 255 * factor;
}

MaterialInspector::MaterialInspector(QWidget* parent):
	QWidget(parent),
	ui(new Ui::MaterialInspector),
	model(nullptr)
{
	ui->setupUi(this);
	applyData();
}

MaterialInspector::~MaterialInspector()
{
	model = nullptr;
	delete ui;
}

void MaterialInspector::applyData()
{
	connect(ui->horizontalSliderAnisotropic, &QSlider::sliderReleased, [=]() {
		auto value = ui->horizontalSliderAnisotropic->value() / 100.0f;
		if (model != nullptr && abs(model->modelMaterial.anisotropic - value) > 1e-6) {
			emit sendRevertModel(model);
			model->modelMaterial.anisotropic = value;
			ui->anisotropic->setText(QString::number(model->modelMaterial.anisotropic, 'f', 2));
			QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, false, false);
		}
	});
	connect(ui->horizontalSliderAnisotropic, &QSlider::valueChanged, [=]() {
		auto value = ui->horizontalSliderAnisotropic->value() / 100.0f;
		if (model != nullptr) {
			ui->anisotropic->setText(QString::number(value, 'f', 2));
		}
	});
	connect(ui->horizontalSliderClearcoat, &QSlider::sliderReleased, [=]() {
		auto value = ui->horizontalSliderClearcoat->value() / 100.0f;
		if (model != nullptr && abs(model->modelMaterial.clearcoat-value) > 1e-6) {
			emit sendRevertModel(model);
			model->modelMaterial.clearcoat = value;
			ui->clearcoat->setText(QString::number(model->modelMaterial.clearcoat, 'f', 2));
			QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, false, false);
		}
	});
	connect(ui->horizontalSliderClearcoat, &QSlider::valueChanged, [=]() {
		auto value = ui->horizontalSliderClearcoat->value() / 100.0f;
		if (model != nullptr) {
			ui->clearcoat->setText(QString::number(value, 'f', 2));
		}
	});
	connect(ui->horizontalSliderClearcoatGloss, &QSlider::sliderReleased, [=]() {
		auto value = ui->horizontalSliderClearcoatGloss->value() / 100.0f;
		if (model != nullptr && abs(model->modelMaterial.clearcoatGloss-value) > 1e-6) {
			emit sendRevertModel(model);
			model->modelMaterial.clearcoatGloss = value;
			ui->clearcoatGloss->setText(QString::number(model->modelMaterial.clearcoatGloss, 'f', 2));
			QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, false, false);
		}
	});
	connect(ui->horizontalSliderClearcoatGloss, &QSlider::valueChanged, [=]() {
		auto value = ui->horizontalSliderClearcoatGloss->value() / 100.0f;
		if (model != nullptr && abs(model->modelMaterial.clearcoatGloss - value) > 1e-6) {
			ui->clearcoatGloss->setText(QString::number(value, 'f', 2));
		}
	});
	connect(ui->horizontalSliderMetallic, &QSlider::sliderReleased, [=]() {
		auto value = ui->horizontalSliderMetallic->value() / 100.0f;
		if (model != nullptr && abs(model->modelMaterial.metallic-value) > 1e-6) {
			emit sendRevertModel(model);
			model->modelMaterial.metallic = value;
			ui->metallic->setText(QString::number(model->modelMaterial.metallic, 'f', 2));
			QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, false, false);
		}
	});
	connect(ui->horizontalSliderMetallic, &QSlider::valueChanged, [=]() {
		auto value = ui->horizontalSliderMetallic->value() / 100.0f;
		if (model != nullptr) {
			ui->metallic->setText(QString::number(value, 'f', 2));
		}
	});
	connect(ui->horizontalSliderRoughness, &QSlider::sliderReleased, [=]() {
		auto value = ui->horizontalSliderRoughness->value() / 100.0f;
		if (model != nullptr && abs(model->modelMaterial.roughness-value) > 1e-6) {
			emit sendRevertModel(model);
			model->modelMaterial.roughness = value;
			ui->roughness->setText(QString::number(model->modelMaterial.roughness, 'f', 2));
			QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, false, false);
		}
	});
	connect(ui->horizontalSliderRoughness, &QSlider::valueChanged, [=]() {
		auto value = ui->horizontalSliderRoughness->value() / 100.0f;
		if (model != nullptr) {
			ui->roughness->setText(QString::number(value, 'f', 2));
		}
	});
	connect(ui->horizontalSliderSheen, &QSlider::sliderReleased, [=]() {
		auto value = ui->horizontalSliderSheen->value() / 100.0f;
		if (model != nullptr && abs(model->modelMaterial.sheen-value) > 1e-6) {
			emit sendRevertModel(model);
			model->modelMaterial.sheen = value;
			ui->sheen->setText(QString::number(model->modelMaterial.sheen, 'f', 2));
			QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, false, false);
		}
	});
	connect(ui->horizontalSliderSheen, &QSlider::valueChanged, [=]() {
		auto value = ui->horizontalSliderSheen->value() / 100.0f;
		if (model != nullptr) {
			ui->sheen->setText(QString::number(value, 'f', 2));
		}
	});
	connect(ui->horizontalSliderSheenTint, &QSlider::sliderReleased, [=]() {
		auto value = ui->horizontalSliderSheenTint->value() / 100.0f;
		if (model != nullptr && abs(model->modelMaterial.sheenTint-value) > 1e-6) {
			emit sendRevertModel(model);
			model->modelMaterial.sheenTint = value;
			ui->sheenTint->setText(QString::number(model->modelMaterial.sheenTint, 'f', 2));
			QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, false, false);
		}
	});
	connect(ui->horizontalSliderSheenTint, &QSlider::valueChanged, [=]() {
		auto value = ui->horizontalSliderSheenTint->value() / 100.0f;
		if (model != nullptr) {
			ui->sheenTint->setText(QString::number(value, 'f', 2));
		}
	});
	connect(ui->horizontalSliderSpecular, &QSlider::sliderReleased, [=]() {
		auto value = ui->horizontalSliderSpecular->value() / 100.0f;
		if (model != nullptr && abs(model->modelMaterial.specular-value) > 1e-6) {
			emit sendRevertModel(model);
			model->modelMaterial.specular = value;
			ui->specular->setText(QString::number(model->modelMaterial.specular, 'f', 2));
			QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, false, false);
		}
	});
	connect(ui->horizontalSliderSpecular, &QSlider::valueChanged, [=]() {
		auto value = ui->horizontalSliderSpecular->value() / 100.0f;
		if (model != nullptr) {
			ui->specular->setText(QString::number(value, 'f', 2));
		}
	});
	connect(ui->horizontalSliderSpecularTint, &QSlider::sliderReleased, [=]() {
		auto value = ui->horizontalSliderSpecularTint->value() / 100.0f;
		if (model != nullptr && abs(model->modelMaterial.specularTint-value) > 1e-6) {
			emit sendRevertModel(model);
			model->modelMaterial.specularTint = value;
			ui->specularTint->setText(QString::number(model->modelMaterial.specularTint, 'f', 2));
			QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, false, false);
		}
	});
	connect(ui->horizontalSliderSpecularTint, &QSlider::valueChanged, [=]() {
		auto value = ui->horizontalSliderSpecularTint->value() / 100.0f;
		if (model != nullptr) {
			ui->specularTint->setText(QString::number(value, 'f', 2));
		}
	});
	connect(ui->horizontalSliderSubsurface, &QSlider::sliderReleased, [=]() {
		auto value = ui->horizontalSliderSubsurface->value() / 100.0f;
		if (model != nullptr && abs(model->modelMaterial.subsurface-value) > 1e-6) {
			emit sendRevertModel(model);
			model->modelMaterial.subsurface = value;
			ui->subsurface->setText(QString::number(model->modelMaterial.subsurface, 'f', 2));
			QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, false, false);
		}
	});
	connect(ui->horizontalSliderSubsurface, &QSlider::valueChanged, [=]() {
		auto value = ui->horizontalSliderSubsurface->value() / 100.0f;
		if (model != nullptr) {
			ui->subsurface->setText(QString::number(value, 'f', 2));
		}
	});
	connect(ui->horizontalSliderFactor, &QSlider::valueChanged, [=]() {
		auto value = ui->horizontalSliderFactor->value();
		if (model != nullptr) {
			ui->labelFactor ->setText(QString::number(value));
			QVector3D emissive;
			if (model->getType() == NORMAL)
				emissive = model->modelMaterial.emissive;
			else
				emissive = model->lightMaterial.emissive;
			auto color = Emissive2Color(emissive);
			if (model->getType() == NORMAL)
				model->modelMaterial.emissive = Color2Emissive(color, value);
			else {
				model->lightMaterial.baseColor = QColor2QVector3D(color) / 255.0f;
				model->lightMaterial.emissive = Color2Emissive(color, value);
			}
			QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, false, false);
		}
	});
	connect(ui->toolButtonEmissive, &QToolButton::clicked, [=]() {
		if(model != nullptr){
			
			QVector3D emissive;
			if (model->getType() == NORMAL) 
				emissive = model->modelMaterial.emissive;
			else 
				emissive = model->lightMaterial.emissive;

			QColor color = QColorDialog::getColor(Emissive2Color(emissive), this, "请选择自发光颜色", QColorDialog::DontUseNativeDialog);

			if (color.isValid()) {
				emit sendRevertModel(model);

				ui->labelEmissive->setStyleSheet(QString("background-color: %1;").arg(color.name()));
				ui->labelEmissive->setToolTip(QString("R:%1 G:%2 B:%3").arg(color.red()).arg(color.green()).arg(color.blue()));

				if (model->getType() == NORMAL)
					model->modelMaterial.emissive = Color2Emissive(color, ui->horizontalSliderFactor->value());
				else {
					model->lightMaterial.baseColor = QColor2QVector3D(color) / 255.0f;
					model->lightMaterial.emissive = Color2Emissive(color, ui->horizontalSliderFactor->value());
				}

				QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, false, false);
			}
		
		}
	});
	connect(ui->toolButtonBaseColor, &QToolButton::clicked, [=]() {
		if (model != nullptr) {
			
			QVector3D baseColor;
			if (model->getType() == NORMAL)
				baseColor = model->modelMaterial.baseColor * 255;
			else
				return;

			QColor color = QColorDialog::getColor(QVector3D2QColor(baseColor), this, "请选择基本颜色", QColorDialog::DontUseNativeDialog);

			if (color.isValid()) {
				emit sendRevertModel(model);

				ui->labelBaseColor->setStyleSheet(QString("background-color: %1;").arg(color.name()));
				ui->labelBaseColor->setToolTip(QString("R:%1 G:%2 B:%3").arg(color.red()).arg(color.green()).arg(color.blue()));

				model->modelMaterial.baseColor = QColor2QVector3D(color) / 255.0f;
			
				QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, false, false);
			}

		}
	});
}

void MaterialInspector::setEdittable(bool edittable)
{
	edittable = !edittable;
	ui->horizontalSliderAnisotropic->setDisabled(edittable);
	ui->horizontalSliderClearcoat->setDisabled(edittable);
	ui->horizontalSliderClearcoatGloss->setDisabled(edittable);
	ui->horizontalSliderMetallic->setDisabled(edittable);
	ui->horizontalSliderRoughness->setDisabled(edittable);
	ui->horizontalSliderSheen->setDisabled(edittable);
	ui->horizontalSliderSheenTint->setDisabled(edittable);
	ui->horizontalSliderSpecular->setDisabled(edittable);
	ui->horizontalSliderSpecularTint->setDisabled(edittable);
	ui->horizontalSliderSubsurface->setDisabled(edittable);
}


void MaterialInspector::clearData()
{
	ui->brdf->show();
	ui->baseColorWidget->show();
	ui->horizontalSliderAnisotropic->setValue(0);
	ui->horizontalSliderClearcoat->setValue(0);
	ui->horizontalSliderClearcoatGloss->setValue(0);
	ui->horizontalSliderMetallic->setValue(0);
	ui->horizontalSliderRoughness->setValue(0);
	ui->horizontalSliderSheen->setValue(0);
	ui->horizontalSliderSheenTint->setValue(0);
	ui->horizontalSliderSpecular->setValue(0);
	ui->horizontalSliderSpecularTint->setValue(0);
	ui->horizontalSliderSubsurface->setValue(0);
	ui->labelBaseColor->setStyleSheet("background-color: white;");
	ui->labelEmissive->setStyleSheet("background-color: white;"); //设为白色
}


void MaterialInspector::bindModel(Model* model)
{
	this->model = model;
	if (!model) {
		clearData();
		setEdittable(false);
		return;
	}
	setEdittable(true);
	setData();
}


void MaterialInspector::setData()
{
	auto modelMaterial = model->modelMaterial;
	auto lightMaterial = model->lightMaterial;
	QVector3D emissive;

	if (model->getType() == NORMAL) {

		ui->brdf->show();
		ui->baseColorWidget->show();

		ui->horizontalSliderAnisotropic->setValue(modelMaterial.anisotropic * 100);
		ui->horizontalSliderClearcoat->setValue(modelMaterial.clearcoat * 100);
		ui->horizontalSliderClearcoatGloss->setValue(modelMaterial.clearcoatGloss * 100);
		ui->horizontalSliderMetallic->setValue(modelMaterial.metallic * 100);
		ui->horizontalSliderRoughness->setValue(modelMaterial.roughness * 100);
		ui->horizontalSliderSheen->setValue(modelMaterial.sheen * 100);
		ui->horizontalSliderSheenTint->setValue(modelMaterial.sheenTint * 100);
		ui->horizontalSliderSpecular->setValue(modelMaterial.specular * 100);
		ui->horizontalSliderSpecularTint->setValue(modelMaterial.specularTint * 100);
		ui->horizontalSliderSubsurface->setValue(modelMaterial.subsurface * 100);

		ui->anisotropic->setText(QString::number(modelMaterial.anisotropic, 'f', 2));
		ui->clearcoat->setText(QString::number(modelMaterial.clearcoat, 'f', 2));
		ui->clearcoatGloss->setText(QString::number(modelMaterial.clearcoatGloss, 'f', 2));
		ui->metallic->setText(QString::number(modelMaterial.metallic, 'f', 2));
		ui->roughness->setText(QString::number(modelMaterial.roughness, 'f', 2));
		ui->sheen->setText(QString::number(modelMaterial.sheen, 'f', 2));
		ui->sheenTint->setText(QString::number(modelMaterial.sheenTint, 'f', 2));
		ui->specular->setText(QString::number(modelMaterial.specular, 'f', 2));
		ui->specularTint->setText(QString::number(modelMaterial.specularTint, 'f', 2));
		ui->subsurface->setText(QString::number(modelMaterial.subsurface, 'f', 2));

		QString baseColorText("background-color:%1");
		auto base = modelMaterial.baseColor * 255;
		auto baseColor = QVector3D2QColor(base);
		baseColorText = baseColorText.arg(baseColor.name());
		ui->labelBaseColor->setToolTip(QString("R:%1 G:%2 B:%3").arg(baseColor.red()).arg(baseColor.green()).arg(baseColor.blue()));
		ui->labelBaseColor->setStyleSheet(baseColorText);

		emissive = modelMaterial.emissive;
	}

	else {
		ui->brdf->hide();
		ui->baseColorWidget->hide();
		emissive = lightMaterial.emissive;
	}

	QString emissiveColorText("background-color:%1");
	auto emissiveColor = Emissive2Color(emissive);
	emissiveColorText = emissiveColorText.arg(emissiveColor.name());
	ui->labelEmissive->setToolTip(QString("R:%1 G:%2 B:%3").arg(emissiveColor.red()).arg(emissiveColor.green()).arg(emissiveColor.blue()));
	ui->labelEmissive->setStyleSheet(emissiveColorText);

}
