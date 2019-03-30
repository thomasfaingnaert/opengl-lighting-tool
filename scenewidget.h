#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <memory>

#include <QObject>
#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QWidget>

class SceneWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_2_Core
{
public:
    SceneWidget(QWidget *parent = nullptr);
    ~SceneWidget();

protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

private:
    void initProgram();
    void initData();
    QOpenGLShaderProgram *m_qprogram;

    GLuint m_program;
    GLuint m_vao;
    GLuint m_positionVbo;
    GLuint m_indicesVbo;
    GLint m_mvpMatrixUnif;

    QMatrix4x4 m_mvpMatrix;
};

#endif // SCENEWIDGET_H
