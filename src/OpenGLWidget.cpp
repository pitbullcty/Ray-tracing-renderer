#include "OpenGLWidget.h"

GIZMO_TYPE type[3] = { MOVE,ROTATE,SCALE };
IGizmo::LOCATION locations[2] = { IGizmo::LOCATE_WORLD, IGizmo::LOCATE_LOCAL };
int changeCount = 0;
int locationCount = 0;

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    ,sceneManager(SceneManager::GetInstance())
    ,isRightClicked(false), isLeftClicked(false)
    ,deltaTime(0.0f),lastFrame(0.0f)
{
    QSurfaceFormat surfaceFormat;
    surfaceFormat.setSamples(4);
    setFocusPolicy(Qt::ClickFocus); 
    setFormat(surfaceFormat); //开启多重采样抗锯齿
}

OpenGLWidget::~OpenGLWidget()
{
    sceneManager->clearModels();
}

void OpenGLWidget::initializeGL()
{
 
    this->initializeOpenGLFunctions();
    this->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  //设置清屏颜色
    this->glEnable(GL_DEPTH_TEST);
    this->glDepthFunc(GL_LEQUAL);
    this->glViewport(0, 0, width(), height());

    QString path = "C:/Users/admin/OneDrive/C and C++ Programs/Ray tracing renderer/resources/Model/2/nanosuit.obj";
    sceneManager->addModel(path);

    initShaders();
    initRenderer();

}

void OpenGLWidget::resizeGL(int w, int h)
{
    
    this->glViewport(0, 0, w, h);    //定义视口区域
    renderer->resize(w, h);
}

void OpenGLWidget::paintGL()
{
    
    sceneManager->getCamera()->processKeyboard(deltaTime);
 
    renderer->renderModels();
    renderer->renderSkybox();
    renderer->renderGizmo(); //最后渲染gizmo避免被遮挡
    //渲染部分

    float time = QTime::currentTime().msecsSinceStartOfDay() / 1000.0; //返回当天的毫秒数
 
    deltaTime = time - lastFrame;
   
    lastFrame = time; //计算渲染时间
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
    renderer = Renderer::GetInstance(map, QOpenGLContext::currentContext()->extraFunctions(), width(), height());
    renderer->setModels(sceneManager->getModels());
    renderer->initSkybox();
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

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    Qt::Key key = (Qt::Key)(event->key());
    if(key == Qt::Key_W || key == Qt::Key_S || key == Qt::Key_A || key == Qt::Key_D
        || key == Qt::Key_Space || key == Qt::Key_Shift)
    {
        if (event->isAutoRepeat())
            return;
        sceneManager->getCamera()->addKey(key);
    }
    else if (key == Qt::Key_Q) {
        changeCount++;
        if (changeCount == 3) changeCount = 0;
        renderer->getGizmo()->setType(type[changeCount]);
    }
    else if (key == Qt::Key_E) {
        locationCount++;
        if (locationCount == 2) locationCount = 0;
        renderer->getGizmo()->setLocate(locations[locationCount]);
    }
    else {
        ;
    }

}

void OpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat())
        return;
    Qt::Key key = (Qt::Key)(event->key());
    sceneManager->getCamera()->removeKey(key);
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
  
    if (event->button() == Qt::RightButton) {
        isRightClicked = true; //右键激活
        lastPos = event->pos();
    }
    else if (event->button() == Qt::LeftButton) {
        int x = event->pos().x();
        int y = event->pos().y();
        if (renderer->getGizmo()->mouseDown(x, y)) {
            isLeftClicked = true;
        } //调整Gizmo
    }
    else {
        ;
    }
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    int x = event->pos().x();
    int y = event->pos().y();
    if (isLeftClicked) {
        renderer->getGizmo()->mouseUp(x, y);
        isLeftClicked = false;
    }
    else if (isRightClicked)
        isRightClicked = false;
    else
        ;
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    int x = event->pos().x();
    int y = event->pos().y();
    if (isRightClicked) {
        int xoffset = x - lastPos.x();
        int yoffset = lastPos.y() - y;
        lastPos = event->pos();
        sceneManager->getCamera()->processMouseMovement(xoffset, yoffset);
    }
    renderer->getGizmo()->mouseMove(x, y);
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
    QPoint offset = event->angleDelta();
    sceneManager->getCamera()->processMouseScroll(offset.y() / 20.0f);
}

