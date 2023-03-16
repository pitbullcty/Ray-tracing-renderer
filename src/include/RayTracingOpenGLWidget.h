#ifndef __RAY_TRACING_OPENGLWIDGET__
#define __RAY_TRACING_OPENGLWIDGET__

#include "BaseOpenGLWidget.h"
#include "RayTracingRenderer.h"

class RayTracingOpenGLWidget :public BaseOpenGLWidget {

public:
    RayTracingOpenGLWidget(QWidget* parent = 0);
    ~RayTracingOpenGLWidget();
    QSharedPointer<RayTracingRenderer> getRayTracingRenderer();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

    virtual void initShaders() override;
    virtual void initRenderer() override;

    
private:
    QOpenGLShaderProgram pathTracingProgram;
    QOpenGLShaderProgram accumProgram;
    QOpenGLShaderProgram outputProgram;

    QSharedPointer<RayTracingRenderer> rayTracingRenderer; //编辑器所用渲染器
    void getSnapshot();
};

#endif 
