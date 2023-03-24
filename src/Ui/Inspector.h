#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QWidget>
#include <QVBoxLayout>
#include "UiInspector.h"
#include "InspectorPage.h"
#include <QVBoxLayout>

class Inspector : public QWidget
{
    Q_OBJECT

public:
    explicit Inspector(QWidget *parent = nullptr);
    ~Inspector();

    void addWidget(const QString &title, QWidget *widget);

private:
    Ui::Inspector*ui;

    QVBoxLayout *contentVBoxLayout;
};

#endif // TOOLBOX_H
