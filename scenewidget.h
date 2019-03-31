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
    Q_OBJECT

public:
    SceneWidget(QWidget *parent = nullptr);
    ~SceneWidget();

protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

public slots:
    void setLightPositionX(int x) { m_lightPosition.setX(x / 10.0f); setLightingParams(); }
    void setLightPositionY(int y) { m_lightPosition.setY(y / 10.0f); setLightingParams(); }
    void setLightPositionZ(int z) { m_lightPosition.setZ(z / 10.0f); setLightingParams(); }
    void setLightAmbientX(int x) { m_lightAmbient.setX(x / 256.0); setLightingParams(); }
    void setLightAmbientY(int y) { m_lightAmbient.setY(y / 256.0); setLightingParams(); }
    void setLightAmbientZ(int z) { m_lightAmbient.setZ(z / 256.0); setLightingParams(); }
    void setLightDiffuseX(int x) { m_lightDiffuse.setX(x / 256.0); setLightingParams(); }
    void setLightDiffuseY(int y) { m_lightDiffuse.setY(y / 256.0); setLightingParams(); }
    void setLightDiffuseZ(int z) { m_lightDiffuse.setZ(z / 256.0); setLightingParams(); }
    void setLightSpecularX(int x) { m_lightSpecular.setX(x / 256.0); setLightingParams(); }
    void setLightSpecularY(int y) { m_lightSpecular.setY(y / 256.0); setLightingParams(); }
    void setLightSpecularZ(int z) { m_lightSpecular.setZ(z / 256.0); setLightingParams(); }

    void setMaterialShininess(int shininess) { m_materialShininess = shininess; setLightingParams(); }
    void setMaterialAmbientX(int x) { m_lightAmbient.setX(x / 256.0); setLightingParams(); }
    void setMaterialAmbientY(int y) { m_lightAmbient.setY(y / 256.0); setLightingParams(); }
    void setMaterialAmbientZ(int z) { m_lightAmbient.setZ(z / 256.0); setLightingParams(); }
    void setMaterialDiffuseX(int x) { m_lightDiffuse.setX(x / 256.0); setLightingParams(); }
    void setMaterialDiffuseY(int y) { m_lightDiffuse.setY(y / 256.0); setLightingParams(); }
    void setMaterialDiffuseZ(int z) { m_lightDiffuse.setZ(z / 256.0); setLightingParams(); }
    void setMaterialSpecularX(int x) { m_lightSpecular.setX(x / 256.0); setLightingParams(); }
    void setMaterialSpecularY(int y) { m_lightSpecular.setY(y / 256.0); setLightingParams(); }
    void setMaterialSpecularZ(int z) { m_lightSpecular.setZ(z / 256.0); setLightingParams(); }

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
