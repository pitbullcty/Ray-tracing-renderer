#include "ModelListWidget.h"

ModelListWidget::ModelListWidget(QWidget* parent):QListWidget(parent), current(nullptr)
{
	setStyleSheet("QListView::item {height: 25px;font-size:5px;}");
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
    current = itemAt(event->pos());
    if (current != nullptr) //如果有item则添加
    {
        copyAction = new QAction("复制", this);
        deleteAction = new QAction("删除", this);
        renameAction = new QAction("重命名", this);
        popMenu->addAction(copyAction);
        popMenu->addAction(deleteAction);
        popMenu->addAction(renameAction);
        connect(renameAction, &QAction::triggered, this, &ModelListWidget::rename);
    }
    popMenu->exec((QCursor::pos())); // 菜单出现的位置为当前鼠标的位置
    if(!renameAction) delete renameAction;
    if (!copyAction) delete copyAction;
    if (!deleteAction) delete deleteAction;
    delete popMenu;
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


void ModelListWidget::updateList(QMap<QString, Model>* models) {
	if (!models) return;
	clear();
	for (auto it = models->begin(); it != models->end(); it++) {
		addItem(it.key());
	}
}


