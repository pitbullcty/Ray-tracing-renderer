#include "EditorOpenGLWidget.h"
#include <QPainter>
#include <QFontMetrics>

GIZMOTYPE type[3] = { MOVE,ROTATE,SCALE };
IGizmo::LOCATION locations[2] = { IGizmo::LOCATE_WORLD, IGizmo::LOCATE_LOCAL };

int changeCount = 0;
int locationCount = 0;

extern bool isBusy;

EditorOpenGLWidget::EditorOpenGLWidget(QWidget* parent)
    : BaseOpenGLWidget(parent)
{
}


bool EditorOpenGLWidget::closeApp()
{
    return sceneManager->closeApp();
}

void EditorOpenGLWidget::destoryData()
{
    editorRenderer->destoryData();
    sceneManager->clearModels();
}

QSharedPointer<EditorRenderer> EditorOpenGLWidget::getEditorRenderer() const
{
    return editorRenderer;
}

void EditorOpenGLWidget::initializeGL()
{
    initGL();
}

void EditorOpenGLWidget::resizeGL(int w, int h)
{
    this->glEnable(GL_DEPTH_TEST);
    this->glDepthFunc(GL_LEQUAL);
    this->glViewport(0, 0, w, h);    //定义视口区域
    editorRenderer->resize(w, h);
    sceneManager->setSize(w, h);
}

void EditorOpenGLWidget::paintGL()
{
    if (isBusy) return;

    if (sceneManager->getState() == NONE) {
        drawTips("请新建或打开文件！");
    }

    else {
        sceneManager->getCamera()->processKeyboard(deltaTime);
        editorRenderer->renderModels();
        editorRenderer->renderSkybox();
        editorRenderer->renderAABB();
        editorRenderer->renderGizmo(); //最后渲染gizmo避免被遮挡 
        clacFPS();
        emit sendFPS(fps);
    }
    drawGizmoHints(); //左下角操作提示(叠加在主画面上,不遮挡模型)
    update();
}

void EditorOpenGLWidget::drawGizmoHints()
{
    if (sceneManager->getState() == NONE) return; //空场景只有居中提示

    auto gizmo = editorRenderer->getGizmo();
    const bool building = DataBuilder::GetInstance().data()->isBuilding();
    const bool isLocal = gizmo->getLocation() == IGizmo::LOCATE_LOCAL;
    const GIZMOTYPE tool = gizmo->getType();

    QString toolName = QString::fromUtf8("移动");
    if (tool == ROTATE) toolName = QString::fromUtf8("旋转");
    else if (tool == SCALE) toolName = QString::fromUtf8("缩放");

    QStringList lines;
    lines << QString::fromUtf8("当前工具: %1  (Q 切换)").arg(toolName);
    lines << QString::fromUtf8("坐标系: %1  (E 切换)").arg(isLocal ? QString::fromUtf8("局部") : QString::fromUtf8("世界"));
    lines << QString::fromUtf8("右键拖动视角 · WASD 移动 · 滚轮缩放 · Delete 删除");
    if (building) lines << QString::fromUtf8("正在重建 BVH, 交互暂时锁定…");

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    QFont font;
    font.setFamily(QString::fromUtf8("微软雅黑"));
    font.setPointSizeF(9.5);
    painter.setFont(font);

    QFontMetrics metrics(font);
    int textWidth = 0;
    const int lineHeight = metrics.height();
    for (const auto& line : lines) textWidth = qMax(textWidth, metrics.horizontalAdvance(line));

    const int padding = 10;
    const int spacing = 3;
    const int panelWidth = textWidth + padding * 2;
    const int panelHeight = lines.size() * lineHeight + (lines.size() - 1) * spacing + padding * 2;
    const int margin = 12;
    //窗口缩放后 QOpenGLWidget 的尺寸可能大于被主窗口裁剪后真正可见的区域,
    //这里把提示面板夹到「实际可见区域」的左下角,避免画到窗口外面而看不见。
    int visibleWidth = width();
    int visibleHeight = height();
    if (QWidget* topWindow = window()) {
        const QPoint origin = mapTo(topWindow, QPoint(0, 0)); //本控件原点在窗口坐标系中的位置
        visibleWidth = qMin(visibleWidth, topWindow->width() - origin.x());
        visibleHeight = qMin(visibleHeight, topWindow->height() - origin.y());
    }
    if (visibleWidth <= 0 || visibleHeight <= 0) {
        visibleWidth = width();
        visibleHeight = height();
    }
    QRectF panel(margin, visibleHeight - panelHeight - margin, panelWidth, panelHeight);

    painter.setPen(QPen(QColor(255, 255, 255, 45), 1));
    painter.setBrush(QColor(18, 18, 18, 150));
    painter.drawRoundedRect(panel, 6, 6);

    int textY = panel.top() + padding;
    for (int i = 0; i < lines.size(); i++) {
        const bool isBusyLine = building && (i == lines.size() - 1);
        painter.setPen(isBusyLine ? QColor(255, 200, 80) : QColor(232, 232, 232));
        painter.drawText(QRect(panel.left() + padding, textY, textWidth, lineHeight), Qt::AlignLeft | Qt::AlignVCenter, lines.at(i));
        textY += lineHeight + spacing;
    }
    painter.end();
}


void EditorOpenGLWidget::initShaders()
{ 
    compileShader(&modelShaderProgram, "model");
    compileShader(&skyboxShaderProgram, "skybox");
    compileShader(&gizmoShaderProgram, "gizmo");
}

void EditorOpenGLWidget::initRenderer()
{
    QMap<QString, QOpenGLShaderProgram*> editorMap;
    editorMap.insert("model", &modelShaderProgram);
    editorMap.insert("skybox", &skyboxShaderProgram);
    editorMap.insert("gizmo", &gizmoShaderProgram);

    editorRenderer = EditorRenderer::GetInstance(editorMap, 
        QOpenGLContext::currentContext()->extraFunctions(), width(), height());
    editorRenderer->setModels(sceneManager->getModels());
    connect(sceneManager.data(), &SceneManager::sendEditModel, editorRenderer.data(), &EditorRenderer::setSelected);

    auto& modelLoader = ModelLoader::GetInstance();
    modelLoader->setContext(QOpenGLContext::currentContext()->extraFunctions(), &modelShaderProgram);

    editorRenderer->initSkybox();

}

void EditorOpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    if (isIgnore()) {
        event->ignore();
        return;
    }

    Qt::Key key = (Qt::Key)(event->key());
    auto modifiers = event->modifiers();
    
    processCameraKey(event);
    changeFullScreen(key);
    auto selected = editorRenderer->getSelected();

    if (key == Qt::Key_Q) {
        if (selected) {
            changeCount++;
            if (changeCount == 3) changeCount = 0;
            editorRenderer->getGizmo()->setType(type[changeCount]);
        }
    }
    else if (key == Qt::Key_E) {
        if (selected) {
            locationCount++;
            if (locationCount == 2) locationCount = 0;
            editorRenderer->getGizmo()->setLocation(locations[locationCount]);
        }
    }
    else if (key == Qt::Key_Delete) {
        if(selected){
            sceneManager->removeModel(selected);
        }//如果选中了物体
    }
    else if (modifiers == Qt::ControlModifier && key == Qt::Key_C) {
        if (selected) {
            sceneManager->copyModel(selected);
        }
    } //复制物体
    else if (modifiers == Qt::ControlModifier && key == Qt::Key_D) {
        if (selected) {
            sceneManager->copyModel(selected);
            sceneManager->pasteModel(selected->transform.getTranslation());
        }
        sceneManager->copyModel(nullptr);
    } //原地复制物体
    else if (modifiers == Qt::ControlModifier && key == Qt::Key_V) {
        if (selected) {
            sceneManager->pasteModel(mapFromGlobal(QCursor::pos()));
        }
    }
    else if (modifiers == Qt::ControlModifier && key == Qt::Key_Z) {
        sceneManager->revertAction();
    }
    else {
        ;
    }

}

void EditorOpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    processKeyRelease(event);
}

void EditorOpenGLWidget::mousePressEvent(QMouseEvent* event)
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
        int x = event->pos().x();
        int y = event->pos().y();
        if (editorRenderer->getGizmo()->mouseDown(x, y)) {
            isLeftClicked = true;
            selected = editorRenderer->getSelected();
        } //调整Gizmo
        else {
            selected = sceneManager->getSelected(x, y); //计算选中的物体
            editorRenderer->setSelected(selected);
            if (selected) {
                editorRenderer->getGizmo()->setType(MOVE);
                editorRenderer->getGizmo()->setLocation(IGizmo::LOCATION::LOCATE_WORLD);
                changeCount = locationCount = 0; //清空选择
            }
        }
        if (selected) {
            modelToRevert = *selected;
            lastEdit = selected->transform.getModel();
        } //选中则保存编辑的模型矩阵
    }
    else {
        ;
    }
}

void EditorOpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (isIgnore()) {
        event->ignore();
        return;
    }
    int x = event->pos().x();
    int y = event->pos().y();
    if (isLeftClicked) {
        editorRenderer->getGizmo()->mouseUp(x, y);
        if (editorRenderer->getGizmo()->isChange(lastEdit)) {
            QtConcurrent::run(&DataBuilder::buildData, DataBuilder::GetInstance().data(), false, true, true); //位置改变则建立
            auto selected = editorRenderer->getSelected();
            addRevertModel(selected);
        }
        isLeftClicked = false;
    }
    else if (isRightClicked) {
        isRightClicked = false;
    }
    else {
        ;
    }
}

void EditorOpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    int x = event->pos().x();
    int y = event->pos().y();
    processMouseMove(event);
    if (sceneManager->getState() != NONE) {
        editorRenderer->getGizmo()->mouseMove(x, y);
    }
}

void EditorOpenGLWidget::wheelEvent(QWheelEvent* event)
{
    processWheel(event);
}

void EditorOpenGLWidget::addRevertModel(Model* model){
    auto models = sceneManager->getModels();
    QString name;
    for (auto it = models->begin(); it != models->end(); it++) {
        if (&it.value() == model) {
            name = it.key();
            break;
        }
    }
    sceneManager->addRevertModel(MOVEPOS, modelToRevert, name);
    if(model) modelToRevert = *model;
}
