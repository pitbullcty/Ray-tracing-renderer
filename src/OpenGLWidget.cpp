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
    
    //初始化opengl函数
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/model.vert")) {     //添加并编译顶点着色器
        qDebug() << "ERROR:" << shaderProgram.log();    //如果编译出错,打印报错信息
    }
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/model.frag")) {   //添加并编译片段着色器
        qDebug() << "ERROR:" << shaderProgram.log();    //如果编译出错,打印报错信息
    }
    if (!shaderProgram.link()) {                      //链接着色器
        qDebug() << "ERROR:" << shaderProgram.log();    //如果链接出错,打印报错信息
    }
    
    sceneManager = SceneManager::GetInstance(&shaderProgram, QOpenGLContext::currentContext()->functions());
    QString path = "C:/Users/admin/OneDrive/C and C++ Programs/Ray tracing renderer/resources/Model/2/nanosuit.obj";
    QString _path = path.right(path.size() - path.lastIndexOf('/') - 1);
    QString name = _path.left(_path.lastIndexOf('.'));
    Model m = modelLoader->loadModel(path);
    sceneManager->addModel(name,m);
    
}

void OpenGLWidget::resizeGL(int w, int h)
{
    
    this->glViewport(0, 0, w, h);                //定义视口区域
    QMatrix4x4 projection;
    projection.perspective(sceneManager->getCamera()->getZoom(), width() / (float)height(), 0.1f, 500.0f);
    shaderProgram.setUniformValue("projection", projection);
}

void OpenGLWidget::paintGL()
{
    
    float time = QTime::currentTime().msecsSinceStartOfDay() / 1000.0; //返回当天的毫秒数

    sceneManager->getCamera()->processKeyboard(deltaTime);
    shaderProgram.bind();
    this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderModels(); //渲染部分

    if (abs(lastFrame - 0.0f) < 1e6) {
        deltaTime = QTime::currentTime().msecsSinceStartOfDay() / 1000.0 - time;
    }
    else {
        deltaTime = time - lastFrame;
    }
    
    lastFrame = time; //计算渲染时间
    shaderProgram.release();
   
    update();
}

void OpenGLWidget::renderModels() {
    QMatrix4x4 model;
    model.translate(0.0f, 0.0f, 0.0f);
    model.scale(1.0f, 1.0f, 1.0f);
    shaderProgram.setUniformValue("model", model);
    QMatrix4x4 projection;
    projection.perspective(sceneManager->getCamera()->getZoom(), width() / (float)height(), 0.1f, 500.0f);
    shaderProgram.setUniformValue("projection", projection);
    shaderProgram.setUniformValue("view", sceneManager->getCamera()->getView());
    sceneManager->renderModels();
    
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

