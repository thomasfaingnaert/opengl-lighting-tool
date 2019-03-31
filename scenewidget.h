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
    void recalcMvpMatrix();
    void setLightingParams();

    QOpenGLShaderProgram *m_qprogram;

    GLuint m_program;
    GLuint m_vao;
    GLuint m_vertexDataVbo;
    GLuint m_indicesVbo;

    int m_width;
    int m_height;
    float m_angle = 0.0f;

    QMatrix4x4 m_mvMatrix;
    QMatrix4x4 m_pMatrix;

    QVector3D m_lightPosition;
    QVector3D m_lightAmbient;
    QVector3D m_lightDiffuse;
    QVector3D m_lightSpecular;

    float m_materialShininess;
    QVector3D m_materialAmbient;
    QVector3D m_materialDiffuse;
    QVector3D m_materialSpecular;

    constexpr static unsigned int numOfVertices = 24;
};

#endif // SCENEWIDGET_H
