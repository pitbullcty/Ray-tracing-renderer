#include "OpenGLWidget.h"

GIZMO_TYPE type[3] = { MOVE,ROTATE,SCALE };
IGizmo::LOCATION locations[2] = { IGizmo::LOCATE_WORLD, IGizmo::LOCATE_LOCAL };
int changeCount = 0;
int locationCount = 0;

bool test = false;
extern bool isBusy;

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    ,sceneManager(SceneManager::GetInstance())
    ,isRightClicked(false)
    ,isLeftClicked(false)
    ,isFullScreen(false)
    ,deltaTime(0.0f)
    ,lastFrameTime(0.0f)
{
    QSurfaceFormat surfaceFormat;
    surfaceFormat.setSamples(4);
    surfaceFormat.setDepthBufferSize(24); //深度测试缓冲区位数
    setFormat(surfaceFormat); //开启多重采样抗锯齿
    setFocusPolicy(Qt::ClickFocus); 
}

OpenGLWidget::~OpenGLWidget()
{
    sceneManager->clearModels();
}

bool OpenGLWidget::closeApp()
{
    return sceneManager->closeApp();
}


QSharedPointer<EditorRenderer> OpenGLWidget::getEditorRenderer() const
{
    return editorRenderer;
}

QSharedPointer<SceneManager> OpenGLWidget::getSceneManager() const
{
    return sceneManager;
}

void OpenGLWidget::initializeGL()
{
 
    this->initializeOpenGLFunctions();
    this->glClearColor(1.0f, 1.0f, 1.0f, 0.0f);  //设置清屏颜色
    this->glEnable(GL_DEPTH_TEST);
    this->glDepthFunc(GL_LEQUAL);
    this->glViewport(0, 0, width(), height());
    initShaders();
    initRenderer();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    this->glEnable(GL_DEPTH_TEST);
    this->glDepthFunc(GL_LEQUAL);
    this->glViewport(0, 0, w, h);    //定义视口区域
    editorRenderer->resize(w, h);
    sceneManager->setSize(w, h);
}

void OpenGLWidget::paintGL()
{

    if (sceneManager->getState() == NONE) {
        drawTips("请新建或打开文件！");
        return;
    }

    else {

        sceneManager->getCamera()->processKeyboard(deltaTime);

        //Opengl渲染部分
        editorRenderer->renderModels();
        editorRenderer->renderSkybox();
        editorRenderer->renderAABB();
        editorRenderer->renderGizmo(); //最后渲染gizmo避免被遮挡
        drawFPS();

        float time = QTime::currentTime().msecsSinceStartOfDay() / 1000.0; //返回当天的秒数
        deltaTime = time - lastFrameTime;
        lastFrameTime = time; //计算渲染时间

    }
    update();
}


void OpenGLWidget::initShaders()
{ 
    compileShader(&modelShaderProgram, "model");
    compileShader(&skyboxShaderProgram, "skybox");
    compileShader(&gizmoShaderProgram, "gizmo");
}

void OpenGLWidget::initRenderer()
{
    QMap<QString, QOpenGLShaderProgram*> map;
    map.insert("model", &modelShaderProgram);
    map.insert("skybox", &skyboxShaderProgram);
    map.insert("gizmo", &gizmoShaderProgram);

    editorRenderer = EditorRenderer::GetInstance(map, QOpenGLContext::currentContext()->extraFunctions(), width(), height());
    editorRenderer->setModels(sceneManager->getModels());
    connect(sceneManager.data(), &SceneManager::sendEditModel, editorRenderer.data(), &EditorRenderer::setSelected);

    rayTracingRenderer = RayTracingRenderer::GetInstance();
    rayTracingRenderer->setModels(sceneManager->getModels());

    auto& modelLoader = ModelLoader::GetInstance();
    modelLoader->setContext(QOpenGLContext::currentContext()->extraFunctions(), &modelShaderProgram);
    editorRenderer->initSkybox();
}

void OpenGLWidget::compileShader(QOpenGLShaderProgram* shaderProgram, const QString& shaderName)
{
    QString vertName = ":/" + shaderName + ".vert";
    QString fragName = ":/" + shaderName + ".frag";
    if (!shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertName)) {     //添加并编译顶点着色器
        qDebug() << "ERROR:" << shaderProgram->log();    //如果编译出错,打印报错信息
    }
    if (!shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragName)) {   //添加并编译片段着色器
        qDebug() << "ERROR:" << shaderProgram->log();    //如果编译出错,打印报错信息
    }
    if (!shaderProgram->link()) {                      //链接着色器
        qDebug() << "ERROR:" << shaderProgram->log();    //如果链接出错,打印报错信息
    }
}

void OpenGLWidget::drawFPS()
{
    if (abs(deltaTime-0.0f)<1e-6) return; 
    int fps = 1.0f / deltaTime;
    if (fps > 144) {
        return;
    }
    if (!painter.isActive())
        painter.begin(this);
    painter.setPen(QColor(0, 0, 0));//设置画笔颜色
    painter.setRenderHint(QPainter::Antialiasing, true);
    QFont font;//设置字体，下面设置字体属性
    font.setFamily("宋体");//字体样式
    font.setPointSizeF(20);
    painter.setFont(font);
    QString text = "FPS:" + QString::number(fps);
    painter.drawText(rect(), Qt::AlignLeft, text);
    painter.end();
}

void OpenGLWidget::drawTips(const QString& tips)
{
    if(!painter.isActive())
        painter.begin(this);
    painter.setPen(QColor(0, 0, 0));//设置画笔颜色
    painter.setRenderHint(QPainter::Antialiasing, true); // 反走样
    QFont font;//设置字体，下面设置字体属性
    font.setFamily("宋体");//字体样式
    font.setPointSizeF(30);
    painter.setFont(font);
    painter.drawText(rect(), Qt::AlignCenter, tips);
    painter.end();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    if (isBusy || sceneManager->getState() == NONE) {
        event->ignore();
        return;
    }

    Qt::Key key = (Qt::Key)(event->key());
    auto modifiers = event->modifiers();
    if(key == Qt::Key_W || key == Qt::Key_S || key == Qt::Key_A || (key == Qt::Key_D && modifiers!= Qt::ControlModifier)
        || key == Qt::Key_Space || key == Qt::Key_Shift)
    {
        if (event->isAutoRepeat())
            return;
        sceneManager->getCamera()->addKey(key);
    }
    else if (key == Qt::Key_Q) {
        changeCount++;
        if (changeCount == 3) changeCount = 0;
        editorRenderer->getGizmo()->setType(type[changeCount]);
    }
    else if (key == Qt::Key_E) {
        locationCount++;
        if (locationCount == 2) locationCount = 0;
        editorRenderer->getGizmo()->setLocate(locations[locationCount]);
    }
    else if (key == Qt::Key_F5) {
        if (!isFullScreen) {
            setWindowFlags(Qt::Window);
            showFullScreen();
            isFullScreen = true;
        }
    }
    else if (key == Qt::Key_Escape) {
        if (isFullScreen) {
            setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint); //去除边框和按钮
            setWindowFlag(Qt::SubWindow);
            showNormal();
            isFullScreen = false;
        }
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
    else {
        ;
    }

}

void OpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (isBusy || sceneManager->getState() == NONE) {
        event->ignore();
        return;
    }

    if (event->isAutoRepeat())
        return;
    Qt::Key key = (Qt::Key)(event->key());
    sceneManager->getCamera()->removeKey(key);
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    if (isBusy || sceneManager->getState() == NONE) {
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
        } //调整Gizmo
        else {
            selected = sceneManager->getSelected(x, y); //计算选中的物体
            editorRenderer->setSelected(selected);
        }
    }
    else {
        ;
    }
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (isBusy || sceneManager->getState() == NONE) {
        event->ignore();
        return;
    }

    int x = event->pos().x();
    int y = event->pos().y();
    if (isLeftClicked) {
        editorRenderer->getGizmo()->mouseUp(x, y);
        isLeftClicked = false;
    }
    else if (isRightClicked)
        isRightClicked = false;
    else
        ;
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (isBusy || sceneManager->getState() == NONE) {
        event->ignore();
        return;
    }

    int x = event->pos().x();
    int y = event->pos().y();
    if (isRightClicked) {
        int xoffset = x - lastPos.x();
        int yoffset = lastPos.y() - y;
        lastPos = event->pos();
        sceneManager->getCamera()->processMouseMovement(xoffset, yoffset);
    }
    if(sceneManager->getState() != NONE)
        editorRenderer->getGizmo()->mouseMove(x, y);
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
    if (isBusy || sceneManager->getState() == NONE) {
        event->ignore();
        return;
    }

    QPoint offset = event->angleDelta();
    sceneManager->getCamera()->processMouseScroll(offset.y() / 20.0f);
}


