#include <cmath>
#include <iostream>
#include <stdexcept>

#include "scenewidget.h"

SceneWidget::SceneWidget(QWidget *parent)
    : QOpenGLWidget (parent),
      m_lightPosition(0.0f, 1.0f, 0.0f),
      m_lightAmbient(0.2f, 0.2f, 0.2f),
      m_lightDiffuse(1.0f, 1.0f, 1.0f),
      m_lightSpecular(1.0f, 1.0f, 1.0f),
      m_materialShininess(30.0f),
      m_materialAmbient(1.0f, 1.0f, 1.0f),
      m_materialDiffuse(1.0f, 0.8f, 0.0f),
      m_materialSpecular(1.0f, 0.8f, 0.0f)
{
    // Set OpenGL version and profile
    QSurfaceFormat format;

    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setSamples(4);

    setFormat(format);
    setFocusPolicy(Qt::StrongFocus);
}

SceneWidget::~SceneWidget()
{
    delete m_qprogram;
    delete m_qprogramPhong;
}

void SceneWidget::initializeGL()
{
    // Init opengl
    if(!initializeOpenGLFunctions())
        throw std::runtime_error("Could not load OpenGL functions.\nDo you have OpenGL v3.2?");

    // Print version info
    std::clog << "OpenGL version: " << glGetString(GL_VERSION) <<
                 "\nGLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) <<
                 "\nRenderer: " << glGetString(GL_RENDERER) <<
                 "\nVendor: " << glGetString(GL_VENDOR) << '\n' << std::endl;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0);

    // Enable face culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glDepthRange(0.0, 1.0);

    initProgram(m_qprogram, m_program, "vertex.glsl", "fragment.glsl");
    initProgram(m_qprogramPhong, m_programPhong, "vertex_phong.glsl", "fragment_phong.glsl");
    initData();
    setLightingParams();
}

void SceneWidget::initProgram(QOpenGLShaderProgram *&qprogram, GLuint &programId, const std::string &vertexShader, const std::string &fragmentShader)
{
    qprogram = new QOpenGLShaderProgram(this);
    qprogram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShader.c_str());
    qprogram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShader.c_str());
    qprogram->link();
    programId = qprogram->programId();
}

void SceneWidget::initData()
{
    // Data
    GLfloat data[] =
    {
        /* POSITIONS */

        // Front face
        -1.0f, -1.0f, +1.0f,
        -1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        +1.0f, -1.0f, +1.0f,

        // Right face
        +1.0f, -1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, -1.0f,
        +1.0f, -1.0f, -1.0f,

        // Top face
        -1.0f, +1.0f, +1.0f,
        -1.0f, +1.0f, -1.0f,
        +1.0f, +1.0f, -1.0f,
        +1.0f, +1.0f, +1.0f,

        // Back face
        -1.0f, -1.0f, -1.0f,
        -1.0f, +1.0f, -1.0f,
        +1.0f, +1.0f, -1.0f,
        +1.0f, -1.0f, -1.0f,

        // Left face
        -1.0f, -1.0f, +1.0f,
        -1.0f, +1.0f, +1.0f,
        -1.0f, +1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,

        // Bottom face
        -1.0f, -1.0f, +1.0f,
        -1.0f, -1.0f, -1.0f,
        +1.0f, -1.0f, -1.0f,
        +1.0f, -1.0f, +1.0f,

        /* NORMALS */

        // Front face
        +0.0f, +0.0f, +1.0f,
        +0.0f, +0.0f, +1.0f,
        +0.0f, +0.0f, +1.0f,
        +0.0f, +0.0f, +1.0f,

        // Right face
        +1.0f, +0.0f, +0.0f,
        +1.0f, +0.0f, +0.0f,
        +1.0f, +0.0f, +0.0f,
        +1.0f, +0.0f, +0.0f,

        // Top face
        +0.0f, +1.0f, +0.0f,
        +0.0f, +1.0f, +0.0f,
        +0.0f, +1.0f, +0.0f,
        +0.0f, +1.0f, +0.0f,

        // Back face
        +0.0f, +0.0f, -1.0f,
        +0.0f, +0.0f, -1.0f,
        +0.0f, +0.0f, -1.0f,
        +0.0f, +0.0f, -1.0f,

        // Left face
        -1.0f, +0.0f, +0.0f,
        -1.0f, +0.0f, +0.0f,
        -1.0f, +0.0f, +0.0f,
        -1.0f, +0.0f, +0.0f,

        // Bottom face
        +0.0f, -1.0f, +0.0f,
        +0.0f, -1.0f, +0.0f,
        +0.0f, -1.0f, +0.0f,
        +0.0f, -1.0f, +0.0f,
    };

    // Indices
    GLushort indices[] =
    {
        // Front face
        0, 1, 2,
        0, 2, 3,

        // Right face
        4, 5, 6,
        4, 6, 7,

        // Top face
        8, 9, 10,
        8, 10, 11,

        // Back face
        12, 14, 13,
        12, 15, 14,

        // Left face
        16, 18, 17,
        16, 19, 18,

        // Bottom face
        20, 22, 21,
        20, 23, 22,
    };

    // Create and bind vao
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Create and bind position vbo
    glGenBuffers(1, &m_vertexDataVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataVbo);

    // Create and bind indices vbo
    glGenBuffers(1, &m_indicesVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesVbo);

    // Fill position buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof data, data, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(numOfVertices * 3 * sizeof (GLfloat)));

    // Fill in indices buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

    // Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SceneWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_usePhong ? m_programPhong : m_program);
    glBindVertexArray(m_vao);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(0));

    glBindVertexArray(0);
    glUseProgram(0);


    if (m_enableRotation)
    {
        m_angle = m_angle + m_rotationSpeed;
        while (m_angle > 360.0f) m_angle -= 360.0f;
    }

    recalcMvpMatrix();

    // Schedule another repaint event
    update();
}

void SceneWidget::recalcMvpMatrix()
{
    // Set mvp matrix
    const float aspect = static_cast<float>(m_width) / m_height;

    m_mvMatrix.setToIdentity();

    const QVector3D eye
    {
        m_cameraRadius * std::cos(m_cameraPhi) * std::cos(m_cameraTheta),
        m_cameraRadius * std::sin(m_cameraPhi),
        m_cameraRadius * std::cos(m_cameraPhi) * std::sin(m_cameraTheta)
    };

    m_mvMatrix.lookAt(eye, QVector3D(0, 0, 0), QVector3D(0, 1, 0)); // view
    m_mvMatrix.rotate(m_angle, QVector3D(1, 0, 0)); // model

    m_pMatrix.setToIdentity();
    m_pMatrix.perspective(90.0f, aspect, 1.0f, 30.0f); // projection

    // Load uniforms (gourard)
    glUseProgram(m_program);
    m_qprogram->setUniformValue("mvMatrix", m_mvMatrix);
    m_qprogram->setUniformValue("pMatrix", m_pMatrix);
    glUseProgram(0);

    // Load uniforms (phong)
    glUseProgram(m_programPhong);
    m_qprogram->setUniformValue("mvMatrix", m_mvMatrix);
    m_qprogram->setUniformValue("pMatrix", m_pMatrix);
    glUseProgram(0);
}

void SceneWidget::setLightingParams()
{
    setLightingParamsForProgram(m_qprogram, m_program);
    setLightingParamsForProgram(m_qprogramPhong, m_programPhong);
}

void SceneWidget::setLightingParamsForProgram(QOpenGLShaderProgram *qprogram, GLuint program)
{
    glUseProgram(program);

    qprogram->setUniformValue("lightPosition", m_lightPosition);
    qprogram->setUniformValue("lightAmbient", m_enableAmbient ? m_lightAmbient : QVector3D(0.0, 0.0, 0.0));
    qprogram->setUniformValue("lightDiffuse", m_enableDiffuse ? m_lightDiffuse : QVector3D(0.0, 0.0, 0.0));
    qprogram->setUniformValue("lightSpecular", m_enableSpecular ? m_lightSpecular : QVector3D(0.0, 0.0, 0.0));

    qprogram->setUniformValue("materialShininess", m_materialShininess);
    qprogram->setUniformValue("materialAmbient", m_enableAmbient ? m_materialAmbient : QVector3D(0.0, 0.0, 0.0));
    qprogram->setUniformValue("materialDiffuse", m_enableDiffuse ? m_materialDiffuse : QVector3D(0.0, 0.0, 0.0));
    qprogram->setUniformValue("materialSpecular", m_enableSpecular ? m_materialSpecular : QVector3D(0.0, 0.0, 0.0));

    glUseProgram(0);
}

void SceneWidget::resizeGL(int w, int h)
{
    // Adjust viewport
    glViewport(0, 0, w, h);

    // Change width and height
    m_width = w;
    m_height = h;
    recalcMvpMatrix();
}
