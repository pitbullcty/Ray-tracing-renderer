#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "SceneManager.h"
#include "ModelLoader.h"

class OpenGLWidget: public QOpenGLWidget, public QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget* parent = 0);
    ~OpenGLWidget();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    QOpenGLShaderProgram shaderProgram;
    QSharedPointer<SceneManager> sceneManager;
    QSharedPointer<ModelLoader> modelLoader;
    void renderModels();
};


#endif