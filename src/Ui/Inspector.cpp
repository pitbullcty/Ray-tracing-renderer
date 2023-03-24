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
