#include "opengltest.h"

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , VBO(QOpenGLBuffer::VertexBuffer)
{
    vertices = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
}

OpenGLWidget::~OpenGLWidget()
{}

void OpenGLWidget::initializeGL()
{
    
    this->initializeOpenGLFunctions();        //初始化opengl函数
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/test.vert")) {     //添加并编译顶点着色器
        qDebug() << "ERROR:" << shaderProgram.log();    //如果编译出错,打印报错信息
    }
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/test.frag")) {   //添加并编译片段着色器
        qDebug() << "ERROR:" << shaderProgram.log();    //如果编译出错,打印报错信息
    }
    if (!shaderProgram.link()) {                      //链接着色器
        qDebug() << "ERROR:" << shaderProgram.log();    //如果链接出错,打印报错信息
    }

    //    VAO.create();       //生成VAO对象
    //    VAO.bind();         //绑定VAO，之后所以的顶点缓冲对象的操作都会存储到VAO中
    // QOpenGLVertexArrayObject::Binder{ &VAO };

    VBO.create();       //生成VBO对象
    VBO.bind();         //将VBO绑定到当前的顶点缓冲对象（QOpenGLBuffer::VertexBuffer）中
    //将顶点数据分配到VBO中，第一个参数为数据指针，第二个参数为数据的字节长度
    VBO.allocate(vertices.data(), sizeof(float) * vertices.size());

    shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
    shaderProgram.enableAttributeArray(0);

    //    VAO.release();      //解绑VAO，安全使用
}

void OpenGLWidget::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h);                //定义视口区域
}

void OpenGLWidget::paintGL()
{
    this->glClearColor(0.1f, 0.5f, 0.7f, 1.0f);  //设置清屏颜色
    this->glClear(GL_COLOR_BUFFER_BIT);       //清除颜色缓存
    shaderProgram.bind();                     //使用shaderProgram着色程序
    {
       QOpenGLVertexArrayObject::Binder{ &VAO };
        this->glDrawArrays(GL_TRIANGLES, 0, 3);     //使用以0开始，长度为3的顶点数据来绘制三角形
    }
}