#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QTime>
#include "Renderer.h"
#include "SceneManager.h"

class OpenGLWidget: public QOpenGLWidget, public QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget* parent = 0);
    ~OpenGLWidget();

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

    QSharedPointer<Renderer> renderer;
    QSharedPointer<SceneManager> sceneManager;

    bool isRightClicked; //右键是否按下
    bool isLeftClicked; //右键是否按下
    QPoint lastPos; //上次鼠标停留的位置

    float deltaTime; //渲染每帧间隔时间
    float lastFrame; //上一帧时间

    void initShaders();
    void initRenderer();
    void compileShader(QOpenGLShaderProgram* shaderProgram, const QString& shaderName);
   
};


#endif