#ifndef __EDITOR_OPENGL_WIDGET__
#define __EDITOR_OPENGL_WIDGET__

#include "BaseOpenGLWidget.h"
#include "EditorRenderer.h"

class EditorOpenGLWidget: public BaseOpenGLWidget
{
    Q_OBJECT

public:

    EditorOpenGLWidget(QWidget* parent = 0);
    ~EditorOpenGLWidget() = default;
    bool closeApp();
    virtual void destoryData() override;
    QSharedPointer<EditorRenderer> getEditorRenderer() const;

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual void initShaders() override;
    virtual void initRenderer() override;


    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    
private:

    QOpenGLShaderProgram modelShaderProgram;
    QOpenGLShaderProgram skyboxShaderProgram;
    QOpenGLShaderProgram gizmoShaderProgram;

    QSharedPointer<EditorRenderer> editorRenderer; //编辑器所用渲染器

    QMatrix4x4 lastEdit; //上次编辑的模型矩阵

    Model modelToRevert; //需要撤回的模型

};


#endif