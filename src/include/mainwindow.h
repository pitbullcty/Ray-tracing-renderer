#include "ui_Mainwindow.h"
#include "WindowActions.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();
    void setStyle(int style=0);
    void closeEvent(QCloseEvent* event);

private:
    Ui::MainWindow* ui;
    WindowActions actions;
    void bindSignals();
};


