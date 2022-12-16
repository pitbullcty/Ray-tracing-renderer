#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , modelLoader(ModelLoader::GetInstance())
{
    ;
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
    sceneManager->addModel(name, modelLoader->loadModel(path));
}

void OpenGLWidget::resizeGL(int w, int h)
{
    
    this->glViewport(0, 0, w, h);                //定义视口区域
}

void OpenGLWidget::paintGL()
{
    renderModels();
}

void OpenGLWidget::renderModels() {
    shaderProgram.setUniformValue("model", QMatrix4x4());
    QMatrix4x4 projection;
    projection.perspective(45.0f, width() / (float)height(), 0.1f, 500.0f);
    shaderProgram.setUniformValue("projection", projection);
    shaderProgram.setUniformValue("view", QMatrix4x4());
    sceneManager->renderModels();
}