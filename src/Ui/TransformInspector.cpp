#include "TransformInspector.h"

TransformInspector::TransformInspector(QWidget* parent):
	QWidget(parent),
	ui(new Ui::TransformInspector),
    model(nullptr)
{
	ui->setupUi(this);
    applyData();
}

TransformInspector::~TransformInspector()
{
	delete ui;
}

void TransformInspector::bindModel(Model* model)
{
    this->model = model;
    if (!model) {
        clearData();
        return;
    }
    setData();
}

void TransformInspector::clearData()
{
    ui->doubleSpinBoxPosX->clear();
    ui->doubleSpinBoxPosY->clear();
    ui->doubleSpinBoxPosZ->clear();
    ui->doubleSpinBoxRotationX->clear();
    ui->doubleSpinBoxRotationY->clear();
    ui->doubleSpinBoxRotationZ->clear();
    ui->doubleSpinBoxScaleX->clear();
    ui->doubleSpinBoxScaleY->clear();
    ui->doubleSpinBoxScaleZ->clear();
}

void TransformInspector::applyData()
{
    connect(ui->doubleSpinBoxPosX, &QDoubleSpinBox::editingFinished, [=]() {
        if (model != nullptr) {
            emit sendRevertModel(model);
            model->transform.translationX = ui->doubleSpinBoxPosX->value();
            model->transform.calcModel();
            model->updateBound();
            emit sendEditModel(model);
        }
    });
    connect(ui->doubleSpinBoxPosY, &QDoubleSpinBox::editingFinished, [=]() {
        if (model != nullptr) {
            emit sendRevertModel(model);
            model->transform.translationY = ui->doubleSpinBoxPosY->value();
            model->transform.calcModel();
            model->updateBound();
            emit sendEditModel(model);
        }
    });
    connect(ui->doubleSpinBoxPosZ, &QDoubleSpinBox::editingFinished, [=]() {
        if (model != nullptr) {
            emit sendRevertModel(model);
            model->transform.translationZ = ui->doubleSpinBoxPosZ->value();
            model->transform.calcModel();
            model->updateBound();
            emit sendEditModel(model);
        }
    });
    connect(ui->doubleSpinBoxRotationX, &QDoubleSpinBox::editingFinished, [=]() {
        if (model != nullptr) {
            emit sendRevertModel(model);
            model->transform.rotationX = ui->doubleSpinBoxRotationX->value();
            model->transform.calcModel();
            model->updateBound();
            emit sendEditModel(model);
        }
    });
    connect(ui->doubleSpinBoxRotationY, &QDoubleSpinBox::editingFinished, [=]() {
        if (model != nullptr) {
            emit sendRevertModel(model);
            model->transform.rotationY = ui->doubleSpinBoxRotationY->value();
            model->transform.calcModel();
            model->updateBound();
            emit sendEditModel(model);
        }
    });
    connect(ui->doubleSpinBoxRotationZ, &QDoubleSpinBox::editingFinished, [=]() {
        if (model != nullptr) {
            emit sendRevertModel(model);
            model->transform.rotationZ = ui->doubleSpinBoxRotationZ->value();
            model->transform.calcModel();
            model->updateBound();
            emit sendEditModel(model);
        }
    });
    connect(ui->doubleSpinBoxScaleX, &QDoubleSpinBox::editingFinished, [=]() {
        if (model != nullptr) {
            emit sendRevertModel(model);
            model->transform.scaleX = ui->doubleSpinBoxScaleX->value();
            model->transform.calcModel();
            model->updateBound();
            emit sendEditModel(model);
        }
    });
    connect(ui->doubleSpinBoxScaleY, &QDoubleSpinBox::editingFinished, [=]() {
        if (model != nullptr) {
            emit sendRevertModel(model);
            model->transform.scaleY = ui->doubleSpinBoxScaleY->value();
            model->transform.calcModel();
            model->updateBound();
            emit sendEditModel(model);
        }
    });
    connect(ui->doubleSpinBoxScaleZ, &QDoubleSpinBox::editingFinished, [=]() {
        if (model != nullptr) {
            emit sendRevertModel(model);
            model->transform.scaleZ = ui->doubleSpinBoxScaleZ->value();
            model->transform.calcModel();
            model->updateBound();
            emit sendEditModel(model);
        }
     });
}

void TransformInspector::setData()
{
    auto& transform = model->transform;
    ui->doubleSpinBoxPosX->setValue(transform.translationX);
    ui->doubleSpinBoxPosY->setValue(transform.translationY);
    ui->doubleSpinBoxPosZ->setValue(transform.translationZ);
    ui->doubleSpinBoxRotationX->setValue(transform.rotationX);
    ui->doubleSpinBoxRotationY->setValue(transform.rotationY);
    ui->doubleSpinBoxRotationZ->setValue(transform.rotationZ);
    ui->doubleSpinBoxScaleX->setValue(transform.scaleX);
    ui->doubleSpinBoxScaleY->setValue(transform.scaleY);
    ui->doubleSpinBoxScaleZ->setValue(transform.scaleZ);
}
