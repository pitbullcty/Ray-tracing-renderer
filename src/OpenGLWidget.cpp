#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , modelLoader(ModelLoader::GetInstance())
    , isRightClicked(false)
    ,deltaTime(0.0f),lastFrame(0.0f)
{
    setFocusPolicy(Qt::ClickFocus); 
}

OpenGLWidget::~OpenGLWidget()
{
    sceneManager->destoryTexture();
}

void OpenGLWidget::initializeGL()
{
 
    this->initializeOpenGLFunctions();
    this->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  //设置清屏颜色
    this->glEnable(GL_DEPTH_TEST);
    this->glDepthFunc(GL_LEQUAL);
    this->glViewport(0, 0, width(), height());
   
    initShaders();
    initSceneManager();
    
    QString path = "C:/Users/admin/OneDrive/C and C++ Programs/Ray tracing renderer/resources/Model/2/nanosuit.obj";
    QString _path = path.right(path.size() - path.lastIndexOf('/') - 1);
    QString name = _path.left(_path.lastIndexOf('.'));
    Model m = modelLoader->loadModel(path);
    sceneManager->addModel(name,m);
    sceneManager->initSkybox();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    
    this->glViewport(0, 0, w, h);                //定义视口区域
    QMatrix4x4 projection;
    projection.perspective(sceneManager->getCamera()->getZoom(), width() / (float)height(), 0.1f, 500.0f);
    modelShaderProgram.setUniformValue("projection", projection);
    skyboxShaderProgram.setUniformValue("projection", projection);
}

void OpenGLWidget::paintGL()
{
    
    sceneManager->getCamera()->processKeyboard(deltaTime);
 
    sceneManager->renderModels();
    sceneManager->renderSkybox();  //渲染部分

    float time = QTime::currentTime().msecsSinceStartOfDay() / 1000.0; //返回当天的毫秒数
 
    deltaTime = time - lastFrame;
   
    lastFrame = time; //计算渲染时间
    update();
}

void OpenGLWidget::initShaders()
{ 
    if (!modelShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/model.vert")) {     //添加并编译顶点着色器
        qDebug() << "ERROR:" << modelShaderProgram.log();    //如果编译出错,打印报错信息
    }
    if (!modelShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/model.frag")) {   //添加并编译片段着色器
        qDebug() << "ERROR:" << modelShaderProgram.log();    //如果编译出错,打印报错信息
    }
    if (!modelShaderProgram.link()) {                      //链接着色器
        qDebug() << "ERROR:" << modelShaderProgram.log();    //如果链接出错,打印报错信息
    }
    if (!skyboxShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/skybox.vert")) {     //添加并编译顶点着色器
        qDebug() << "ERROR:" << skyboxShaderProgram.log();    //如果编译出错,打印报错信息
    }
    if (!skyboxShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/skybox.frag")) {   //添加并编译片段着色器
        qDebug() << "ERROR:" << skyboxShaderProgram.log();    //如果编译出错,打印报错信息
    }
    if (!skyboxShaderProgram.link()) {                      //链接着色器
        qDebug() << "ERROR:" << skyboxShaderProgram.log();    //如果链接出错,打印报错信息
    }
}

void OpenGLWidget::initSceneManager()
{
    QMap<QString, QOpenGLShaderProgram*> map;
    map.insert("model", &modelShaderProgram);
    map.insert("skybox", &skyboxShaderProgram);
    sceneManager = SceneManager::GetInstance(map, QOpenGLContext::currentContext()->functions(), width(), height());
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat())
        return;
    Qt::Key key = (Qt::Key)(event->key());
    sceneManager->getCamera()->addKey(key);

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
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    isRightClicked = false;
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (isRightClicked) {
        int xpos = event->pos().x();
        int ypos = event->pos().y();
        int xoffset = xpos - lastPos.x();
        int yoffset = lastPos.y() - ypos;
        lastPos = event->pos();
        sceneManager->getCamera()->processMouseMovement(xoffset, yoffset);
    }
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
    QPoint offset = event->angleDelta();
    sceneManager->getCamera()->processMouseScroll(offset.y() / 20.0f);
}

