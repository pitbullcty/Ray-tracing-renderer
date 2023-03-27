#include "EditorOpenGLWidget.h"

GIZMO_TYPE type[3] = { MOVE,ROTATE,SCALE };
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
    update();
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

    if (key == Qt::Key_Q) {
        changeCount++;
        if (changeCount == 3) changeCount = 0;
        editorRenderer->getGizmo()->setType(type[changeCount]);
    }
    else if (key == Qt::Key_E) {
        locationCount++;
        if (locationCount == 2) locationCount = 0;
        editorRenderer->getGizmo()->setLocate(locations[locationCount]);
    }
    else if (key == Qt::Key_Delete) {
        auto selected = editorRenderer->getSelected();
        if(selected){
            sceneManager->removeModel(selected);
        }//如果选中了物体
    }
    else if (modifiers == Qt::ControlModifier && key == Qt::Key_C) {
        auto selected = editorRenderer->getSelected();
        if (selected) {
            sceneManager->copyModel(selected);
        }
    } //复制物体
    else if (modifiers == Qt::ControlModifier && key == Qt::Key_D) {
        auto selected = editorRenderer->getSelected();
        if (selected) {
            sceneManager->copyModel(selected);
            sceneManager->pasteModel(selected->transform.getTranslation());
        }
        sceneManager->copyModel(nullptr);
    } //原地复制物体
    else if (modifiers == Qt::ControlModifier && key == Qt::Key_V) {
        auto selected = editorRenderer->getSelected();
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
    if(sceneManager->getState() != NONE)
        editorRenderer->getGizmo()->mouseMove(x, y);
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
    modelToRevert = *model;
}
