#include "InspectorPage.h"


InspectorPage::InspectorPage(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::InspectorPage),
    isExpanded(true),
    isEnabled(true),
    label(nullptr)
{
    ui->setupUi(this);

    ui->widgetContent->setAttribute(Qt::WA_StyledBackground);

    label = new QLabel(this);
    label->setFixedSize(20, 20);
    label->setPixmap(QPixmap(":/icons/downArrow.png").scaled(label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    QHBoxLayout *layout = new QHBoxLayout(ui->pushButtonFold);
    layout->setContentsMargins(0, 0, 5, 0);
    layout->addStretch(1);
    layout->addWidget(label);

    connect(ui->pushButtonFold, &QPushButton::clicked, this, &InspectorPage::onPushButtonFoldClicked);
}

InspectorPage::~InspectorPage()
{
    delete ui;
}

void InspectorPage::setEnabled(bool isEnabled)
{
    this->isEnabled = isEnabled;
}

void InspectorPage::addWidget(const QString &title, QWidget *widget)
{
    ui->pushButtonFold->setText(title);
    ui->verticalLayoutContent->addWidget(widget);
    collapse();
}

void InspectorPage::expand()
{
    ui->widgetContent->show();
    isExpanded = true;
    label->setPixmap(QPixmap(":/icons/downArrow.png").scaled(label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void InspectorPage::collapse()
{
    ui->widgetContent->hide();
    isExpanded = false;

    label->setPixmap(QPixmap(":/icons/leftArrow.png").scaled(label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void InspectorPage::onPushButtonFoldClicked()
{
    if (!isEnabled) return;

    if (isExpanded) {
        collapse();
    } else {
        expand();
    }
}
