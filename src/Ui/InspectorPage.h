#ifndef TOOLPAGE_H
#define TOOLPAGE_H

#include <QWidget>
#include <QLabel>
#include "UiInspectorPage.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFile>


class InspectorPage : public QWidget
{
    Q_OBJECT

public:

    InspectorPage(QWidget *parent = nullptr);
    ~InspectorPage();
    void setEnabled(bool isEnabled);

public slots:
    void addWidget(const QString &title, QWidget *widget);
    void expand();
    void collapse();

private slots:
    void onPushButtonFoldClicked();

private:
    Ui::InspectorPage* ui;

    bool isEnabled; //是否开启按钮
    bool isExpanded;

    QLabel *label;

};

#endif // TOOLPAGE_H
