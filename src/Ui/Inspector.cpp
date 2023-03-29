#include "Inspector.h"

Inspector::Inspector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Inspector),
    contentVBoxLayout(nullptr)
{
    ui->setupUi(this);

    QWidget *widget = new QWidget(this);
    contentVBoxLayout = new QVBoxLayout;
    contentVBoxLayout->setContentsMargins(0, 0, 0, 0);
    contentVBoxLayout->setSpacing(2);

    QVBoxLayout *vBoxLayout = new QVBoxLayout(widget);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->addLayout(contentVBoxLayout);
    vBoxLayout->addStretch(1);

    ui->scrollArea->setWidget(widget);
    ui->modelName->hide();
}

Inspector::~Inspector()
{
    delete ui;
}

void Inspector::addWidget(const QString &title, QWidget *widget)
{
    InspectorPage* page = new InspectorPage(this);
    page->addWidget(title, widget);
    contentVBoxLayout->addWidget(page);

}

void Inspector::setEnabled(bool isEnabled)
{
    for (int i = 0; i < contentVBoxLayout->count(); i++) {
        auto page = static_cast<InspectorPage*>(contentVBoxLayout->itemAt(i)->widget());
        page->setEnabled(isEnabled);
    }
}

void Inspector::expandAll()
{
    for (int i = 0; i < contentVBoxLayout->count(); i++) {
        auto page = static_cast<InspectorPage*>(contentVBoxLayout->itemAt(i)->widget());
        page->expand();
        page->setEnabled(true);
    }
}

void Inspector::changeCurrentIndex(int index)
{
    auto transform = static_cast<InspectorPage*>(contentVBoxLayout->itemAt(0)->widget());
    auto material = static_cast<InspectorPage*>(contentVBoxLayout->itemAt(1)->widget());
    auto renderOption = static_cast<InspectorPage*>(contentVBoxLayout->itemAt(2)->widget());

    if (index) {
        ui->modelName->show();
        material->show();
        transform->show();
        renderOption->hide();
    }
    else {
        ui->modelName->hide();
        material->hide();
        transform->hide();
        renderOption->expand();
        renderOption->setEnabled(true);
        renderOption->show();
    }
}

void Inspector::hideWidget(int index)
{
    auto page = static_cast<InspectorPage*>(contentVBoxLayout->itemAt(index)->widget());
    page->hide();
}

void Inspector::showWidget(int index)
{
    auto page = static_cast<InspectorPage*>(contentVBoxLayout->itemAt(index)->widget());
    page->show();
}


void Inspector::collapseAll(bool isClose)
{
    if (isClose) {
        ui->modelName->hide();
        ui->modelName->setText("");
    }
    for (int i = 0; i < contentVBoxLayout->count(); i++) {
        auto page = static_cast<InspectorPage*>(contentVBoxLayout->itemAt(i)->widget());
        page->collapse();
        page->setEnabled(false);
    }
}

void Inspector::setModelName(const QString& name) {

    QString text("<font size = \"5\"><b>%1</b></font>");
    if (name.isEmpty()) {
        collapseAll();
        ui->modelName->setText(text.arg("未选中物体"));
    }
    else {
        expandAll();
        ui->modelName->setText(text.arg(name));
    }

    auto renderOption = static_cast<InspectorPage*>(contentVBoxLayout->itemAt(2)->widget());
    if (!renderOption->isHidden()) return;
    ui->modelName->show();
   
}