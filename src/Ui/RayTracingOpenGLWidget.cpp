#include "RayTracingOpenGLWidget.h"
#include <QThreadPool>

RayTracingOpenGLWidget::RayTracingOpenGLWidget(QWidget* parent) :
    BaseOpenGLWidget(parent),
    isLongPressing(false),
    isOfflineRendering(false)
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

    //后台离屏渲染期间只在右下角叠加一块小面板显示进度和缩略图,不覆盖原有画面
    if (isOfflineRendering) {
        drawOfflineProgress();
    }

    update();
}

void RayTracingOpenGLWidget::drawOfflineProgress()
{
    const int margin = 16;
    const int panelWidth = 300;
    const int padding = 10;
    const int titleHeight = 20;
    const int textHeight = 34;

    int previewWidth = panelWidth - padding * 2;
    int previewHeight = 0;
    if (!offlinePreview.isNull()) {
        previewHeight = qRound(previewWidth * (float)offlinePreview.height() / offlinePreview.width());
    }

    int panelHeight = padding + titleHeight + textHeight + previewHeight + padding;
    QRect panelRect(width() - panelWidth - margin, height() - panelHeight - margin,
        panelWidth, panelHeight);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setPen(QPen(QColor(255, 255, 255, 70), 1));
    painter.setBrush(QColor(18, 18, 18, 195));
    painter.drawRoundedRect(panelRect, 6, 6);

    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPixelSize(13);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(QRect(panelRect.left() + padding, panelRect.top() + padding, previewWidth, titleHeight),
        Qt::AlignLeft | Qt::AlignVCenter, "离屏渲染进行中");

    font.setBold(false);
    font.setPixelSize(12);
    painter.setFont(font);
    painter.setPen(QColor(220, 220, 220));
    painter.drawText(QRect(panelRect.left() + padding, panelRect.top() + padding + titleHeight, previewWidth, textHeight),
        Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, offlineProgressText);

    if (!offlinePreview.isNull()) {
        QRect previewRect(panelRect.left() + padding, panelRect.top() + padding + titleHeight + textHeight,
            previewWidth, previewHeight);
        painter.drawImage(previewRect, offlinePreview);
    }
    painter.end();
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

void RayTracingOpenGLWidget::startOfflineRender(const RenderOption& option)
{
    if (!offlineController) {
        offlineController = QSharedPointer<OfflineRenderController>(new OfflineRenderController(this));
        connect(offlineController.data(), &OfflineRenderController::progress, this, &RayTracingOpenGLWidget::onOfflineProgress);
        connect(offlineController.data(), &OfflineRenderController::previewReady, this, &RayTracingOpenGLWidget::onOfflinePreview);
        connect(offlineController.data(), &OfflineRenderController::finished, this, &RayTracingOpenGLWidget::onOfflineFinished);
        connect(offlineController.data(), &OfflineRenderController::failed, this, &RayTracingOpenGLWidget::onOfflineFailed);
    }

    if (offlineController->isRunning()) {
        emit Info("离屏渲染已在进行中,请稍候...", false);
        return;
    }

    if (QThreadPool::globalInstance()->activeThreadCount() != 0) {
        emit Info("场景数据仍在构建中,请稍后再试", false);
        return;
    } //数据还没构建完就渲染会得到空场景

    //在界面线程把所有需要的状态拍成快照,后台线程只读这份快照
    OfflineRenderJob job;
    job.data = DataBuilder::GetInstance()->getData();
    job.background = QImage(":/icons/background.png").convertToFormat(QImage::Format_RGB888).mirrored();

    auto skybox = Skybox::GetInstance();
    QString keys[6] = { "+x","-x","+y","-y","+z","-z" };
    for (auto& key : keys)
        job.skyboxFaces.append(QImage(skybox->pathes[key]));

    auto camera = sceneManager->getCamera();
    job.camera.pos = camera->getPos();
    job.camera.view = camera->getView();
    job.camera.zoom = camera->getZoom();

    job.option = option;

    isOfflineRendering = true;
    offlinePreview = QImage();
    offlineProgressText = "离屏渲染进行中...";

    offlineController->start(job);
    emit Info("离屏渲染已开始,界面可继续操作", false);
    update();
}

void RayTracingOpenGLWidget::cancelOfflineRender()
{
    if (offlineController && offlineController->isRunning()) {
        offlineController->cancel();
        emit Info("已请求取消离屏渲染", false);
    }
}

bool RayTracingOpenGLWidget::getIsOfflineRendering()
{
    return isOfflineRendering;
}

void RayTracingOpenGLWidget::onOfflineProgress(int frame, int maxFrame, float seconds)
{
    float percent = maxFrame > 0 ? (float)frame / maxFrame * 100.0f : 0.0f;
    float remain = frame > 0 ? seconds / frame * (maxFrame - frame) : 0.0f;

    offlineProgressText = QString("离屏渲染中 %1% 已用时%2s 预计剩余%3s")
        .arg(QString::number(percent, 'f', 1))
        .arg(QString::number(seconds, 'f', 1))
        .arg(QString::number(remain, 'f', 1));

    emit Info(offlineProgressText, false);
    update();
}

void RayTracingOpenGLWidget::onOfflinePreview(const QImage& image)
{
    offlinePreview = image;
    update();
}

void RayTracingOpenGLWidget::onOfflineFinished(const QString& path, float seconds, bool success)
{
    isOfflineRendering = false;
    offlinePreview = QImage();

    if (success) {
        QString info("渲染已完成,总耗时%1s。结果保存至%2, <a href=\"file:///%3\">单击链接查看</a>");
        info = info.arg(QString::number(seconds, 'f', 2)).arg(path).arg(path);
        emit Info(info, false);
    }
    else {
        emit Info("离屏渲染已取消", false);
    }

    update();
}

void RayTracingOpenGLWidget::onOfflineFailed(const QString& error)
{
    isOfflineRendering = false;
    offlinePreview = QImage();
    emit Info("离屏渲染失败:" + error, false);
    update();
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
