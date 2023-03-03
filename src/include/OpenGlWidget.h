#ifndef __OPENGL_WIDGET__
#define __OPENGL_WIDGET__

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QTime>
#include "EditorRenderer.h"
#include "SceneManager.h"
#include "RayTracingRenderer.h"
#include <QTimer>
#include <QPainter>

class OpenGLWidget: public QOpenGLWidget, public QOpenGLExtraFunctions
{
    Q_OBJECT

public:

    OpenGLWidget(QWidget* parent = 0);
    ~OpenGLWidget();
    bool closeApp();
    QSharedPointer<EditorRenderer> getEditorRenderer() const;
    QSharedPointer<SceneManager> getSceneManager() const;

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    
private:

    QOpenGLShaderProgram modelShaderProgram;
    QOpenGLShaderProgram skyboxShaderProgram;
    QOpenGLShaderProgram gizmoShaderProgram;

    QSharedPointer<EditorRenderer> editorRenderer; //编辑器所用渲染器
    QSharedPointer<RayTracingRenderer> rayTracingRenderer; //光线追踪渲染器
    QSharedPointer<SceneManager> sceneManager; //场景管理器

    bool isRightClicked; //右键是否按下
    bool isLeftClicked; //右键是否按下
    bool isFullScreen;  //是否全屏显示

    QPoint lastPos; //上次鼠标停留的位置
    QMatrix4x4 lastEdit; //上次编辑的模型矩阵

    QPainter painter; //绘制提示/FPS
    Model modelToRevert; //需要撤回的模型

    float deltaTime; //渲染每帧间隔时间
    float lastFrameTime; //上一帧时间

    void initShaders();
    void initRenderer();
    void compileShader(QOpenGLShaderProgram* shaderProgram, const QString& shaderName);
    void drawFPS();
    void drawTips(const QString& tips);

};


#endif