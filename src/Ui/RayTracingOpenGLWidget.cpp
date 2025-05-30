﻿#include "RayTracingOpenGLWidget.h"

RayTracingOpenGLWidget::RayTracingOpenGLWidget(QWidget* parent) :
    BaseOpenGLWidget(parent),
    isLongPressing(false)
{
    QDir dir;
    QString snapShotPath(QDir::currentPath() + "/snapshots");
    if (!dir.exists(snapShotPath)) {
        dir.mkdir(snapShotPath);
    }
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
        return;
    }
    else {
        if (!rayTracingRenderer->getIsOffScreenRendering()) {
            sceneManager->getCamera()->processKeyboard(deltaTime);
            if (isLongPressing)
                rayTracingRenderer->clearFrameCounter();
        }

        rayTracingRenderer->render();

        if (rayTracingRenderer->getIsOffScreenRendering()) {
            auto& option = rayTracingRenderer->getOption();
            if (option.frameCounter!=0 && option.frameCounter % 10 == 0) {
                QString info("渲染已完成%1\%,预计剩余时间%2s");
                float remainTime = 1.0f / fps * (option.maxFrameCounter - option.frameCounter);
                info = info.arg(QString::number((float)option.frameCounter / option.maxFrameCounter * 100, 'f', 2)).arg(QString::number(remainTime, 'f', 2));
                emit Info(info, false);
            }
        }

        clacFPS();
        emit sendFPS((int)fps);
    }
    update();
}

void RayTracingOpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    if (rayTracingRenderer->getIsOffScreenRendering() || !rayTracingRenderer->getIsRealTimeRendering()) return;
    Qt::Key key = (Qt::Key)(event->key());
    if (processCameraKey(event)) {
        isLongPressing = true;
    } //长按键盘
    else if (changeFullScreen(key)) {
        rayTracingRenderer->clearFrameCounter();
    }
    else if (key == Qt::Key_F10) {
        getSnapshot();
    }
}

void RayTracingOpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (rayTracingRenderer->getIsOffScreenRendering() || !rayTracingRenderer->getIsRealTimeRendering()) return;
    Qt::Key key = (Qt::Key)(event->key());
    processKeyRelease(event);
    if (key == Qt::Key_F10) return;
    isLongPressing = false;
}

void RayTracingOpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    if (rayTracingRenderer->getIsOffScreenRendering() || !rayTracingRenderer->getIsRealTimeRendering()) return;

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
    if (rayTracingRenderer->getIsOffScreenRendering() || !rayTracingRenderer->getIsRealTimeRendering()) return;
    if (isIgnore()) {
        event->ignore();
        return;
    }
    isRightClicked = false;
    isLeftClicked = false;
}

void RayTracingOpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (rayTracingRenderer->getIsOffScreenRendering() || !rayTracingRenderer->getIsRealTimeRendering()) return;
    processMouseMove(event);
    if (isRightClicked) {
        rayTracingRenderer->clearFrameCounter();
    }
}

void RayTracingOpenGLWidget::wheelEvent(QWheelEvent* event)
{
    if (rayTracingRenderer->getIsOffScreenRendering() || !rayTracingRenderer->getIsRealTimeRendering()) return;
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

    //rayTracingRenderer->initSkyboxTexture();
    rayTracingRenderer->initFBOs();

}

void RayTracingOpenGLWidget::destoryData()
{
    rayTracingRenderer->destoryData();
}

void RayTracingOpenGLWidget::getSnapshot()
{
    QString filename(QDir::currentPath() + "/snapshots/%1%2.png");
    filename = filename.arg(sceneManager->getSceneName()).arg(rayTracingRenderer->getFrameCounter());
    rayTracingRenderer->setSnapshotParam(filename);
    QString info("快照保存至%1, <a href=\"file:///%2\">单击链接查看</a>");
    info = info.arg(filename).arg(filename);
    emit Info(info, true);
}
