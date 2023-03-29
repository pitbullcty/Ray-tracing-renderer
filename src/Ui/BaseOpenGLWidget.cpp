#include "BaseOpenGLWidget.h"

extern bool isBusy;

BaseOpenGLWidget::BaseOpenGLWidget(QWidget* parent): 
	QOpenGLWidget(parent),
    sceneManager(SceneManager::GetInstance()),
    isRightClicked(false),
    isLeftClicked(false),
    isFullScreen(false),
    deltaTime(0.0f),
    lastFrameTime(0.0f),
    fps(0)
{
    QSurfaceFormat surfaceFormat;
    surfaceFormat.setSamples(4);
    surfaceFormat.setDepthBufferSize(24); //深度测试缓冲区位数
    setFormat(surfaceFormat); //开启多重采样抗锯齿
    setFocusPolicy(Qt::ClickFocus);
    setMouseTracking(true);
}


void BaseOpenGLWidget::compileShader(QOpenGLShaderProgram* shaderProgram, const QString& shaderName, const QString& vertName)
{
    QString newVertName;
    if (vertName.isEmpty()) {
        newVertName = ":/" + shaderName + ".vert";
    }
    else
        newVertName = ":/" + vertName + ".vert";;
    QString fragName = ":/" + shaderName + ".frag";
    if (!shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, newVertName)) {     //添加并编译顶点着色器
        qDebug() << "ERROR:" << shaderProgram->log();    //如果编译出错,打印报错信息
    }
    if (!shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragName)) {   //添加并编译片段着色器
        qDebug() << "ERROR:" << shaderProgram->log();    //如果编译出错,打印报错信息
    }
    if (!shaderProgram->link()) {                      //链接着色器
        qDebug() << "ERROR:" << shaderProgram->log();    //如果链接出错,打印报错信息
    }
}

void BaseOpenGLWidget::clacFPS()
{
    float time = QTime::currentTime().msecsSinceStartOfDay() / 1000.0; //返回当天的秒数
    deltaTime = time - lastFrameTime;
    lastFrameTime = time; //更新渲染时间

    if (abs(deltaTime - 0.0f) < 1e-6) return;
    fps = 1.0f / deltaTime;
    if (fps > 144) {
        return;
    }

}

void BaseOpenGLWidget::drawTips(const QString& tips)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    QPainter painter(this);
    painter.setPen(QColor(0, 0, 0));//设置画笔颜色
    painter.setRenderHint(QPainter::Antialiasing, true); // 反走样
    QFont font;//设置字体，下面设置字体属性
    font.setFamily("宋体");//字体样式
    font.setPointSizeF(30);
    painter.setFont(font);
    painter.drawText(rect(), Qt::AlignCenter, tips);
    painter.end();
    glEnable(GL_DEPTH_TEST);
}

void BaseOpenGLWidget::enterEvent(QEnterEvent* event)
{
    setFocus();
}

void BaseOpenGLWidget::leaveEvent(QEvent* event)
{
    clearFocus();
    sceneManager->getCamera()->clearAllKey();
}

bool BaseOpenGLWidget::isIgnore()
{
    return isBusy || sceneManager->getState() == NONE;
}

void BaseOpenGLWidget::initGL()
{
    this->initializeOpenGLFunctions();
    this->glClearColor(1.0f, 1.0f, 1.0f, 0.0f);  //设置清屏颜色
    this->glEnable(GL_DEPTH_TEST);
    this->glDepthFunc(GL_LEQUAL);
    this->glViewport(0, 0, width(), height());
    initShaders();
    initRenderer();
}


QSharedPointer<SceneManager> BaseOpenGLWidget::getSceneManager() const
{
    return sceneManager;
}

int BaseOpenGLWidget::getFPS()
{
    return fps;
}

void BaseOpenGLWidget::processWheel(QWheelEvent* event)
{
    if (isIgnore()) {
        event->ignore();
        return;
    }

    QPoint offset = event->angleDelta();
    sceneManager->getCamera()->processMouseScroll(offset.y() / 20.0f);
}

void BaseOpenGLWidget::processKeyRelease(QKeyEvent* event)
{
    if (isIgnore()) {
        event->ignore();
        return;
    }

    if (event->isAutoRepeat())
        return;
    Qt::Key key = (Qt::Key)(event->key());
    sceneManager->getCamera()->removeKey(key);
}

void BaseOpenGLWidget::processMouseMove(QMouseEvent* event)
{
    if (isIgnore()) {
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
    return; 
}

bool BaseOpenGLWidget::processCameraKey(QKeyEvent* event)
{
    Qt::Key key = (Qt::Key)(event->key());
    auto modifiers = event->modifiers();
    if(key == Qt::Key_W || key == Qt::Key_S || key == Qt::Key_A || (key == Qt::Key_D && modifiers!= Qt::ControlModifier)
        || key == Qt::Key_Space || key == Qt::Key_Shift)
    {
        if (event->isAutoRepeat())
            return true;
        sceneManager->getCamera()->addKey(key);
        return true;
    }
    return false;
}

bool BaseOpenGLWidget::changeFullScreen(Qt::Key key)
{
   
    if (key == Qt::Key_F11) {
        if (!isFullScreen) {
            setWindowFlags(Qt::Window);
            isFullScreen = true;
            emit sendHideRenderWidget(isFullScreen);
            setFocus();
            showFullScreen();
        }
        return true;
    }
    else if (key == Qt::Key_Escape) {
        if (isFullScreen) {
            setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint); //去除边框和按钮
            setWindowFlag(Qt::SubWindow);
            isFullScreen = false;
            emit sendHideRenderWidget(isFullScreen);
            setFocus();
            showNormal();
        }
        return true;
    }
    else
        return false;
    
}

