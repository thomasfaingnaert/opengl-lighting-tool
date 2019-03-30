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

    // Enable face culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    initProgram();
    initData();
}

void SceneWidget::initProgram()
{
    std::cout << "hello" << std::endl;

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
    glGenBuffers(1, &m_positionVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionVbo);

    // Create and bind indices vbo
    glGenBuffers(1, &m_indicesVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesVbo);

    // Fill position buffer & associate with attrib
    glBufferData(GL_ARRAY_BUFFER, sizeof data, data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

    // Fill in indices buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

    // Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SceneWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_program);
    glBindVertexArray(m_vao);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(0));

    glBindVertexArray(0);
    glUseProgram(0);
}

void SceneWidget::resizeGL(int w, int h)
{
    // Adjust viewport
    glViewport(0, 0, w, h);

    // Set mvp matrix
    const float aspect = static_cast<float>(w) / h;
    m_mvpMatrix.setToIdentity();

    m_mvpMatrix.perspective(90.0f, aspect, 1.0f, 30.0f); // projection
    m_mvpMatrix.lookAt(QVector3D(0, 0, 10), QVector3D(0, 0, 0), QVector3D(0, 1, 0)); // view
    m_mvpMatrix.translate(QVector3D(-10, 0, 0));

    // Load uniforms
    glUseProgram(m_program);
    m_mvpMatrixUnif = glGetUniformLocation(m_program, "mvpMatrix");
    m_qprogram->setUniformValue("mvpMatrix", m_mvpMatrix);
    glUseProgram(0);
}
