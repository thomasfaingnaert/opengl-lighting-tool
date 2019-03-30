#include <iostream>
#include <stdexcept>

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


        /* COLORS */

        // Front face
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        // Right face
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // Top face
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        // Back face
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,

        // Left face
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,

        // Bottom face
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
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

    // Color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(numOfVertices * 3 * sizeof(GLfloat)));

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

    m_angle = m_angle + 0.75f;
    while (m_angle > 360.0f) m_angle -= 360.0f;

    recalcMvpMatrix();

    // Schedule another repaint event
    update();
}

void SceneWidget::recalcMvpMatrix()
{
    // Set mvp matrix
    const float aspect = static_cast<float>(m_width) / m_height;
    m_mvpMatrix.setToIdentity();

    m_mvpMatrix.perspective(90.0f, aspect, 1.0f, 30.0f); // projection
    m_mvpMatrix.lookAt(QVector3D(3, 3, 3), QVector3D(0, 0, 0), QVector3D(0, 1, 0)); // view
    //m_mvpMatrix.translate(QVector3D(-10, 0, 0));
    m_mvpMatrix.rotate(m_angle, QVector3D(1, 0, 0));

    // Load uniforms
    glUseProgram(m_program);
    m_mvpMatrixUnif = glGetUniformLocation(m_program, "mvpMatrix");
    m_qprogram->setUniformValue("mvpMatrix", m_mvpMatrix);
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
