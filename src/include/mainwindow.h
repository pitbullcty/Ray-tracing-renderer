#include "ui_Mainwindow.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();
    void setStyle(int style=0);

private:
    Ui::MainWindow* ui;

};