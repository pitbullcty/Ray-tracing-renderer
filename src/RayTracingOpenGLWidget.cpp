#include "RayTracingOpenGLWidget.h"


RayTracingOpenGLWidget::RayTracingOpenGLWidget(QWidget* parent):
    BaseOpenGLWidget(parent)
{
    QDir dir;
    QString snapShotPath(QDir::currentPath() + "/snapshots");
    if (!dir.exists(snapShotPath)) {
        dir.mkdir(snapShotPath);
    }
}

RayTracingOpenGLWidget::~RayTracingOpenGLWidget()
{
    rayTracingRenderer->destoryData();
}

QSharedPointer<RayTracingRenderer> RayTracingOpenGLWidget::getRayTracingRenderer()
{
    return rayTracingRenderer;
}

void RayTracingOpenGLWidget::initializeGL()
{
    initGL();
}

void RayTracingOpenGLWidget::resizeGL(int w, int h)
{
    this->glEnable(GL_DEPTH_TEST);
    this->glDepthFunc(GL_LEQUAL);
    this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->glViewport(0, 0, w, h); 
    rayTracingRenderer->resize(w, h);
    sceneManager->setSize(w, h);
}

void RayTracingOpenGLWidget::paintGL()
{
    if (sceneManager->getState() == NONE) {
        drawTips("请新建或打开文件！");
    }
    else {
        sceneManager->getCamera()->processKeyboard(deltaTime);
        rayTracingRenderer->render();
        clacFPS();
    }
    update();
}

void RayTracingOpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    Qt::Key key = (Qt::Key)(event->key());
    processCameraKey(event);
    changeFullScreen(key);
    if (key == Qt::Key_F10) {
        getSnapshot();
    }
    rayTracingRenderer->clearFrameCounter();
}

void RayTracingOpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    processKeyRelease(event);
    rayTracingRenderer->clearFrameCounter();
}

void RayTracingOpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    if (isIgnore()) {
        event->ignore();
        return;
    }
    Model* selected = nullptr;

    if (event->button() == Qt::RightButton) {
        isRightClicked = true; //右键激活
        lastPos = event->pos();
    }
    else if (event->button() == Qt::LeftButton) {
        isLeftClicked = true;
    }
}

void RayTracingOpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (isIgnore()) {
        event->ignore();
        return;
    }
    isRightClicked = false;
    isLeftClicked = false;
}

void RayTracingOpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    processMouseMove(event);
    if (isRightClicked) {
        rayTracingRenderer->clearFrameCounter();
    }
}

void RayTracingOpenGLWidget::wheelEvent(QWheelEvent* event)
{
    processWheel(event);
    rayTracingRenderer->clearFrameCounter();
}

void RayTracingOpenGLWidget::initShaders()
{
    compileShader(&pathTracingProgram, "pathTracing");
    compileShader(&accumProgram, "accum", "pathTracing");
    compileShader(&outputProgram, "output", "pathTracing");
}

void RayTracingOpenGLWidget::initRenderer()
{
    QMap<QString, QOpenGLShaderProgram*> rayTracingMap;
    rayTracingMap.insert("pathTracing", &pathTracingProgram);
    rayTracingMap.insert("accum", &accumProgram);
    rayTracingMap.insert("output", &outputProgram);

    rayTracingRenderer = RayTracingRenderer::GetInstance(rayTracingMap,
        QOpenGLContext::currentContext()->extraFunctions(), width(), height());
    auto& dataBuilder = DataBuilder::GetInstance();
    connect(dataBuilder.data(), &DataBuilder::sendDataDone, rayTracingRenderer.data(), &RayTracingRenderer::sendDataToGPU);

    rayTracingRenderer->initSkyboxTexture();
    rayTracingRenderer->initFBOs();

}

void RayTracingOpenGLWidget::getSnapshot()
{
    QString filename(QDir::currentPath() + "/snapshots/%1%2.jpg");
    filename = filename.arg(sceneManager->getSceneName()).arg(rayTracingRenderer->getFrameCounter());
    rayTracingRenderer->setSavingParam(filename);
    emit Info("快照保存至" + filename);
}
