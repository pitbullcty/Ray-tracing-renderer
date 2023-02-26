#include "ModelListWidget.h"

ModelListWidget::ModelListWidget(QWidget* parent):QListWidget(parent), current(nullptr)
{
	
}

ModelListWidget::~ModelListWidget()
{
    current = nullptr;
}

void ModelListWidget::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu* popMenu = new QMenu(this);
    QAction* renameAction = nullptr;
    QAction* copyAction = nullptr;
    QAction* deleteAction = nullptr;
    QAction* addAction = nullptr;
    QAction* pasteAction = nullptr;
    current = itemAt(event->pos());
    if (current != nullptr) //如果有item则添加
    {
        copyAction = new QAction("复制", this);
        if (!copyName.isEmpty()) {
            pasteAction = new QAction("粘贴", this);
            popMenu->addAction(pasteAction);
            connect(pasteAction, &QAction::triggered, this, &ModelListWidget::paste);
        }
        deleteAction = new QAction("删除", this);
        renameAction = new QAction("重命名", this);
        popMenu->addAction(copyAction);
        popMenu->addAction(deleteAction);
        popMenu->addAction(renameAction);
        connect(renameAction, &QAction::triggered, this, &ModelListWidget::rename);
        connect(deleteAction, &QAction::triggered, this, &ModelListWidget::remove);
        connect(copyAction, &QAction::triggered, this, &ModelListWidget::copy);
    }
    else {
        currentItem()->setSelected(false);//取消选中状态
        addAction = new QAction("新建", this);
        if (!copyName.isEmpty()) {
            pasteAction = new QAction("粘贴", this);
            popMenu->addAction(pasteAction);
            connect(pasteAction, &QAction::triggered, this, &ModelListWidget::paste);
        }
        popMenu->addAction(addAction);
        connect(addAction, &QAction::triggered, this, &ModelListWidget::add);
    }
    popMenu->exec((QCursor::pos())); // 菜单出现的位置为当前鼠标的位置
    if(!renameAction) delete renameAction;
    if (!copyAction) delete copyAction;
    if (!deleteAction) delete deleteAction;
    if (!addAction) delete addAction;
    if (!pasteAction) delete pasteAction;
    delete popMenu;
}

void ModelListWidget::keyPressEvent(QKeyEvent* event)
{
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
    else if (key == Qt::Key_F2) {
        rename();
    }
    else {
        ;
    }
}


void ModelListWidget::rename()
{
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
    QString name = current->text();
    emit sendRemoveName(name);
}

void ModelListWidget::copy()
{
    copyName = current->text();
}

void ModelListWidget::paste()
{
    if (!copyName.isEmpty()) emit sendCopyName(copyName);
    copyName = "";
}

void ModelListWidget::add()
{
   emit sendAddPath();
}

void ModelListWidget::lookAt()
{
    emit sendLookAtName(currentItem()->text());
}


void ModelListWidget::updateList(QMap<QString, Model>* models) {
	if (!models) return;
	clear();
	for (auto it = models->begin(); it != models->end(); it++) {
		addItem(it.key());
	}
}


