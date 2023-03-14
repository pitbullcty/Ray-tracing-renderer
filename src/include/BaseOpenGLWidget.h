# ifndef __BASE_OPENGL_WIDGET__
#define __BASE_OPENGL_WIDGET__

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QTime>
#include "SceneManager.h"
#include <QTimer>
#include <QPainter>

class BaseOpenGLWidget : public QOpenGLWidget, public QOpenGLExtraFunctions
{
    Q_OBJECT

public:

    BaseOpenGLWidget(QWidget* parent = 0);
    virtual ~BaseOpenGLWidget();
    QSharedPointer<SceneManager> getSceneManager() const;


protected:

    void processWheel(QWheelEvent* event);
    void processKeyRelease(QKeyEvent* event);
    void processMouseMove(QMouseEvent* event);
    void processCameraKey(QKeyEvent* event);
    void changeFullScreen(Qt::Key key);
    bool isIgnore();
    void initGL();

    virtual void initShaders() = 0;
    virtual void initRenderer()= 0;

    void compileShader(QOpenGLShaderProgram* shaderProgram, const QString& shaderName, const QString& vertName = "");
    void drawFPS();
    void drawTips(const QString& tips);

    bool isRightClicked; //右键是否按下
    bool isLeftClicked; //右键是否按下
    bool isFullScreen;  //是否全屏显示

    QPoint lastPos; //上次鼠标停留的位置

    Model modelToRevert; //需要撤回的模型

    float deltaTime; //渲染每帧间隔时间
    float lastFrameTime; //上一帧时间
 
    QSharedPointer<SceneManager> sceneManager; //场景管理器

};


#endif