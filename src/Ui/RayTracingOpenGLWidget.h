#ifndef __RAY_TRACING_OPENGLWIDGET__
#define __RAY_TRACING_OPENGLWIDGET__

#include "BaseOpenGLWidget.h"
#include "src/Renderer/RayTracingRenderer.h"
#include "src/Renderer/OfflineRenderController.h"

class RayTracingOpenGLWidget :public BaseOpenGLWidget {
    Q_OBJECT

public:
    RayTracingOpenGLWidget(QWidget* parent = 0);
    ~RayTracingOpenGLWidget() = default;
    QSharedPointer<RayTracingRenderer> getRayTracingRenderer();
    virtual void destoryData() override;

    //把离屏渲染(点击渲染出图)交给后台线程执行,界面线程继续正常显示
    void startOfflineRender(const RenderOption& option);
    void cancelOfflineRender();
    bool getIsOfflineRendering();

public slots:
    void onOfflineProgress(int frame, int maxFrame, float seconds);
    void onOfflinePreview(const QImage& image);
    void onOfflineFinished(const QString& path, float seconds, bool success);
    void onOfflineFailed(const QString& error);

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

    QSharedPointer<OfflineRenderController> offlineController; //后台离屏渲染控制器
    bool isOfflineRendering;      //是否正在后台离屏渲染
    QImage offlinePreview;        //后台回传的预览图
    QString offlineProgressText;  //叠加面板上显示的进度文字

    void drawOfflineProgress();   //在画面右下角叠加进度小面板(不覆盖原有画面)
};

#endif 
