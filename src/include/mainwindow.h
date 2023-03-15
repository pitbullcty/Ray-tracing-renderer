#include "ui_Mainwindow.h"
#include "MainWindowManager.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();
  
    void closeEvent(QCloseEvent* event);

protected:
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dropEvent(QDropEvent* event);
    virtual void keyPressEvent(QKeyEvent* event);

signals:
    void sendChangeWindow();

private:
    Ui::MainWindow* ui;
    MainWindowManager* mainWindowManager;

    void bindSignals();
};


