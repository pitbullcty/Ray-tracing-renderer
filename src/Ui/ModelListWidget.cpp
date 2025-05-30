﻿#include "ModelListWidget.h"

ModelListWidget::ModelListWidget(QWidget* parent):
    QListWidget(parent), 
    current(nullptr),
    copyName(""),
    currentIndex(0)
{
   
}

ModelListWidget::~ModelListWidget()
{
    current = nullptr;
}

void ModelListWidget::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu* popMenu = new QMenu(this);
    QMenu* subMenu = new QMenu(popMenu);
    subMenu->setTitle("新建");
    QMenu* lightMenu = new QMenu(subMenu);
    lightMenu->setTitle("新建光源");
    QAction* renameAction = nullptr;
    QAction* copyAction = nullptr;
    QAction* deleteAction = nullptr;
    QAction* addModelAction = nullptr;
    QAction* addRectLightAction = nullptr;
    QAction* addSphereLightAction = nullptr;
    QAction* addAction = nullptr;
    QAction* pasteAction = nullptr;

    current = itemAt(event->pos());
    popMenu->addMenu(subMenu);
    subMenu->addMenu(lightMenu);

    addModelAction = new QAction("新建模型", subMenu);
    subMenu->addAction(addModelAction);
   
    addRectLightAction = new QAction("新建平面光", subMenu);
    addSphereLightAction = new QAction("新建球形光", subMenu);
    lightMenu->addAction(addRectLightAction);
    lightMenu->addAction(addSphereLightAction);

    connect(addModelAction, &QAction::triggered, this, &ModelListWidget::add);
    connect(addRectLightAction, &QAction::triggered, this, &ModelListWidget::addRectLight);
    connect(addSphereLightAction, &QAction::triggered, this, &ModelListWidget::addSphereLight);

    if (!currentIndex) { //当前未使用光追
        if (current != nullptr) //如果有item
        {
            copyAction = new QAction("复制", this);
            if (!copyName.isEmpty()) {
                pasteAction = new QAction("粘贴", this);
                popMenu->addAction(pasteAction);
                connect(pasteAction, &QAction::triggered, this, &ModelListWidget::paste);
            }
            deleteAction = new QAction("删除", popMenu);
            renameAction = new QAction("重命名", popMenu);
            popMenu->addAction(copyAction);
            popMenu->addAction(deleteAction);
            popMenu->addAction(renameAction);
            connect(renameAction, &QAction::triggered, this, &ModelListWidget::rename);
            connect(deleteAction, &QAction::triggered, this, &ModelListWidget::remove);
            connect(copyAction, &QAction::triggered, this, &ModelListWidget::copy);
        }
        else {
            if (currentItem()) currentItem()->setSelected(false);//取消选中状态
            if (!copyName.isEmpty()) {
                pasteAction = new QAction("粘贴", popMenu);
                popMenu->addAction(pasteAction);
                connect(pasteAction, &QAction::triggered, this, &ModelListWidget::paste);
            }
        }
    }

    popMenu->exec((QCursor::pos())); // 菜单出现的位置为当前鼠标的位置
    delete popMenu; //设置好父对象可以析构所有子对象
}

void ModelListWidget::keyPressEvent(QKeyEvent* event)
{
    if (currentIndex) return;
    auto key = event->key();
    auto modifiers = event->modifiers();
    current = currentItem();
    if (key == Qt::Key_Delete) {
        remove();
    }
    else if (modifiers == Qt::ControlModifier && key == Qt::Key_C) {
        copy();
    }
    else if (modifiers == Qt::ControlModifier && key == Qt::Key_V) {
        paste();
    }
    else if (modifiers == Qt::ControlModifier && key == Qt::Key_Z) {
        emit sendRevert();
    }
    else if (key == Qt::Key_F2) {
        rename();
    }
    else {
        ;
    }
}


void ModelListWidget::rename()
{
    if (!current) return;
    QString oldname = current->text();
    bool res;
    QString newname = QInputDialog::getText(this, "重命名","请输入模型新名",QLineEdit::Normal,oldname,&res);
    if (!res) return;
    if (newname == oldname || newname.isEmpty()) return;
    else {
        for (int i = 0; i < count(); i++) {
            QString name = item(i)->text();
            if (name == newname) {
                QMessageBox::warning(this,"警告","已包含同名模型！", QMessageBox::Yes);
                return;
            }
        }
        emit sendNewname(oldname, newname);
    }
}

void ModelListWidget::remove()
{
    if (!current) return;
    QString name = current->text();
    emit sendRemoveName(name);
}

void ModelListWidget::copy()
{
    if (!current) return;
    copyName = current->text();
}

void ModelListWidget::paste()
{
    if (!copyName.isEmpty()) emit sendCopyName(copyName);
}

void ModelListWidget::add()
{
   emit sendAddPath("");
}

void ModelListWidget::lookAt()
{
    emit sendLookAtName(currentItem()->text());
}

void ModelListWidget::addRectLight()
{
    
    emit sendAddPath(QDir::currentPath() + "/lights/rectlight.obj");
}

void ModelListWidget::addSphereLight()
{
    
    emit sendAddPath(QDir::currentPath() + "/lights/spherelight.obj");
    
}

void ModelListWidget::getSelectedName()
{
    emit sendSelectedName(currentItem()->text());
}

void ModelListWidget::receiveIndex(int index)
{
    currentIndex = index;
}

void ModelListWidget::updateList(QMap<QString, Model>* models, Model* model) {
	if (!models) return;
	clear();
    int index = 0, resIndex = 0;
	for (auto it = models->begin(); it != models->end(); it++) {
        if (&it.value() == model) {
            resIndex = index;
        }
        index++;
		addItem(it.key());
	}
    if (model) item(resIndex)->setSelected(true); //选中物体
}


