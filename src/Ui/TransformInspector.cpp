#include "TransformInspector.h"

extern int changeCount;
extern int locationCount;

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

void TransformInspector::setEdittable(bool edittable)
{
    edittable = !edittable;
    ui->doubleSpinBoxPosX->setReadOnly(edittable);
    ui->doubleSpinBoxPosY->setReadOnly(edittable);
    ui->doubleSpinBoxPosZ->setReadOnly(edittable);
    ui->doubleSpinBoxRotationX->setReadOnly(edittable);
    ui->doubleSpinBoxRotationY->setReadOnly(edittable);
    ui->doubleSpinBoxRotationZ->setReadOnly(edittable);
    ui->doubleSpinBoxScaleX->setReadOnly(edittable);
    ui->doubleSpinBoxScaleY->setReadOnly(edittable);
    ui->doubleSpinBoxScaleZ->setReadOnly(edittable);
}

void TransformInspector::bindModel(Model* model)
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
        auto value = ui->doubleSpinBoxPosX->value();
        if (model != nullptr && abs(model->transform.translationX-value) > 1e-6) {
            emit sendRevertModel(model);
            model->transform.translationX = value;
            upDateModel();
        }
    });
    connect(ui->doubleSpinBoxPosY, &QDoubleSpinBox::editingFinished, [=]() {
        auto value = ui->doubleSpinBoxPosY->value();;
        if (model != nullptr && abs(model->transform.translationY -value) > 1e-6) {
            emit sendRevertModel(model);
            model->transform.translationY = value;
            upDateModel();
        }
    });
    connect(ui->doubleSpinBoxPosZ, &QDoubleSpinBox::editingFinished, [=]() {
        auto value = ui->doubleSpinBoxPosZ->value();
        if (model != nullptr && abs(model->transform.translationZ - value) > 1e-6) {
            emit sendRevertModel(model);
            model->transform.translationZ = value;
            upDateModel();
        }
    });
    connect(ui->doubleSpinBoxRotationX, &QDoubleSpinBox::editingFinished, [=]() {
        auto value = ui->doubleSpinBoxRotationX->value();
        if (model != nullptr && abs(model->transform.rotationX - value) > 1e-6) {
            emit sendRevertModel(model);
            model->transform.rotationX = value;
            upDateModel();
        }
    });
    connect(ui->doubleSpinBoxRotationY, &QDoubleSpinBox::editingFinished, [=]() {
        auto value = ui->doubleSpinBoxRotationY->value();
        if (model != nullptr && abs(model->transform.rotationY - value) > 1e-6) {
            emit sendRevertModel(model);
            model->transform.rotationY = value;
            upDateModel();
        }
    });
    connect(ui->doubleSpinBoxRotationZ, &QDoubleSpinBox::editingFinished, [=]() {
        auto value = ui->doubleSpinBoxRotationZ->value();
        if (model != nullptr && abs(model->transform.rotationZ - value) > 1e-6) {
            emit sendRevertModel(model);
            model->transform.rotationZ = value;
            upDateModel();
        }
    });
    connect(ui->doubleSpinBoxScaleX, &QDoubleSpinBox::editingFinished, [=]() {
        auto value = ui->doubleSpinBoxScaleX->value();
        if (model != nullptr && abs(model->transform.scaleX - value) > 1e-6) {
            emit sendRevertModel(model);
            model->transform.scaleX = value;
            upDateModel();
        }
    });
    connect(ui->doubleSpinBoxScaleY, &QDoubleSpinBox::editingFinished, [=]() {
        auto value = ui->doubleSpinBoxScaleY->value();
        if (model != nullptr && abs(model->transform.scaleY - value) > 1e-6) {
            emit sendRevertModel(model);
            model->transform.scaleY = value;
            upDateModel();
        }
    });
    connect(ui->doubleSpinBoxScaleZ, &QDoubleSpinBox::editingFinished, [=]() {
        auto value = ui->doubleSpinBoxScaleZ->value();
        if (model != nullptr && abs(model->transform.scaleZ - value) > 1e-6) {
            emit sendRevertModel(model);
            model->transform.scaleZ = value;
            upDateModel();
        }
    });

    connect(ui->radioButtonPos, &QRadioButton::clicked, this, &TransformInspector::getCheckedTransformType);
    connect(ui->radioButtonRotation, &QRadioButton::clicked, this, &TransformInspector::getCheckedTransformType);
    connect(ui->radioButtonScale, &QRadioButton::clicked, this, &TransformInspector::getCheckedTransformType);

    connect(ui->radioButtonWorld, &QRadioButton::clicked, this, &TransformInspector::getCheckedLocationType);
    connect(ui->radioButtonLocal, &QRadioButton::clicked, this, &TransformInspector::getCheckedLocationType);

}

void TransformInspector::upDateModel()
{
    model->transform.calcModel();
    model->updateBound();
    emit sendEditModel(model);
    QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, true, false);
}

void TransformInspector::getCheckedTransformType()
{
    if (model != nullptr) {
        int id = ui->buttonGroup->checkedId();
        if (id == -2) {
            changeCount = 0;
            emit sendGizmoType(MOVE);
        }
        else if (id == -3) {
            changeCount = 1;
            emit sendGizmoType(ROTATE);
        }
        else {
            changeCount = 2;
            emit sendGizmoType(SCALE);
        }
    }
    locationCount = 0;
    emit sendLocationType(IGizmo::LOCATION::LOCATE_WORLD);
}

void TransformInspector::getCheckedLocationType()
{
    if (model != nullptr) {
        int id = ui->buttonGroup2->checkedId();
        if (id == -2) {
            locationCount = 0;
            emit sendLocationType(IGizmo::LOCATION::LOCATE_WORLD);
        }
        else {
            locationCount = 1;
            emit sendLocationType(IGizmo::LOCATION::LOCATE_LOCAL);
        }
    }
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

void TransformInspector::setCheckedTransformButton(int id)
{
    if (id == ui->buttonGroup->checkedId()) return;
    
    if (id == -2) {
        ui->radioButtonPos->setChecked(true);
    }
    else if (id == -3) {
        ui->radioButtonRotation->setChecked(true);
    }
    else {
        ui->radioButtonScale->setChecked(true);
    }
}

void TransformInspector::setCheckedLocationButton(int id)
{
    if (id == ui->buttonGroup2->checkedId()) return;

    if (id == -2) {
        ui->radioButtonWorld->setChecked(true);
    }
    else {
        ui->radioButtonLocal->setChecked(true);
    }
}
