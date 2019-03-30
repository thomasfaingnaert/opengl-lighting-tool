#include <iostream>

#include "scenewidget.h"

SceneWidget::SceneWidget(QWidget *parent)
    : QOpenGLWidget (parent)
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
}

void SceneWidget::initializeGL()
{
    // Init opengl
    initializeOpenGLFunctions();

    // Print version info
    std::clog << "OpenGL version: " << glGetString(GL_VERSION) <<
                 "\nGLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) <<
                 "\nRenderer: " << glGetString(GL_RENDERER) <<
                 "\nVendor: " << glGetString(GL_VENDOR) << '\n' << std::endl;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    initProgram();
    initData();
}

void SceneWidget::initProgram()
{
    m_qprogram = new QOpenGLShaderProgram(this);
    m_qprogram->addShaderFromSourceFile(QOpenGLShader::Vertex, "vertex.glsl");
    m_qprogram->addShaderFromSourceFile(QOpenGLShader::Fragment, "fragment.glsl");
    m_qprogram->link();
    m_program = m_qprogram->programId();

}

void SceneWidget::initData()
{
    // Data
    GLfloat data[] =
    {
        -0.5f, -0.5f, +0.0f,
        +0.5f, -0.5f, +0.0f,
        +0.0f, +0.5f, +0.0f,
    };

    // Create and bind vao
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Create and bind vbo
    glGenBuffers(1, &m_positionVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionVbo);

    // Fill buffer & associate with attrib
    glBufferData(GL_ARRAY_BUFFER, sizeof data, data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

    // Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SceneWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_program);
    glBindVertexArray(m_vao);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
    glUseProgram(0);
}

void SceneWidget::resizeGL(int w, int h)
{
    // Adjust viewport
    glViewport(0, 0, w, h);
}
