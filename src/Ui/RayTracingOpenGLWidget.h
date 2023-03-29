#ifndef __RAY_TRACING_OPENGLWIDGET__
#define __RAY_TRACING_OPENGLWIDGET__

#include "BaseOpenGLWidget.h"
#include "src/Renderer/RayTracingRenderer.h"

class RayTracingOpenGLWidget :public BaseOpenGLWidget {
    Q_OBJECT

public:
    RayTracingOpenGLWidget(QWidget* parent = 0);
    ~RayTracingOpenGLWidget() = default;
    QSharedPointer<RayTracingRenderer> getRayTracingRenderer();
    virtual void destoryData() override;

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

    bool isLongPressing;

    QSharedPointer<RayTracingRenderer> rayTracingRenderer; //编辑器所用渲染器
    void getSnapshot();
};

#endif 
