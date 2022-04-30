#include "glprogram.h"

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QtMath>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLExtraFunctions>
#include <QGuiApplication>
#include <QWidget>
#include <QMouseEvent>
#include <QRandomGenerator>

glprogram::glprogram()
{
    texture = new QOpenGLTexture(QOpenGLTexture::Target2D);

    time = 0;
    pause = false;
}

void glprogram::use_triangle_program()
{
    m_program1 = new QOpenGLShaderProgram();
    m_program1->addShaderFromSourceFile(QOpenGLShader::Vertex, "assets\\vertex_shader.glsl");
    m_program1->addShaderFromSourceFile(QOpenGLShader::Fragment, "assets\\fragment_shader.glsl");
    m_program1->link();

    m_program2 = new QOpenGLShaderProgram();
    m_program2->addShaderFromSourceFile(QOpenGLShader::Compute, "assets\\ray_tracing.glsl");
    m_program2->link();
}

void glprogram::setVar(const std::string&& name, const int& mCurrentProgramActive, std::vector<int>& vars)
{
    int addr = glGetUniformLocation(mCurrentProgramActive, name.c_str());
    switch (vars.size())
    {
    case 1:
        glUniform1i(addr, vars[0]);
        break;
    case 2:
        glUniform2i(addr, vars[0], vars[1]);
        break;
    case 3:
        glUniform3i(addr, vars[0], vars[1], vars[2]);
        break;
    case 4:
        glUniform4i(addr, vars[0], vars[1], vars[2], vars[3]);
        break;
    }
}

void glprogram::setVar(const std::string&& name, const int& mCurrentProgramActive, std::vector<float>& vars, bool transpose) {
    int address = glGetUniformLocation(mCurrentProgramActive, name.c_str());
    switch (vars.size()) {
    case 1:
        glUniform1f(address, vars[0]);
        break;
    case 2:
        glUniform2f(address, vars[0], vars[1]);
        break;
    case 3:
        glUniform3f(address, vars[0], vars[1], vars[2]);
        break;
    case 4:
        glUniform4f(address, vars[0], vars[1], vars[2], vars[3]);
        break;
    case 100:
        GLfloat* tmp = &vars[0];
        glUniform1fv(address, 100 * sizeof(GLfloat), tmp);
    }
}

void glprogram::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    time += 0.05;

    glClear(GL_COLOR_BUFFER_BIT);

    m_program2->bind();

    std::vector<float> a = {time};
    setVar("time", m_program2->programId(), a, false);

    a.clear();
    for(size_t i = 0; i<100 ; ++i)
    {
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dis(-0.07, 0.07);
        a.push_back(dis(gen));
    }

    setVar("random_array", m_program2->programId(), a, false);

    glBindImageTexture(0, texture->textureId(), 0, false, 0, GL_WRITE_ONLY, texture->format());

    glDispatchCompute(width() / 16 + 1, height() / 16 + 1, 1);

    glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT);
    glMemoryBarrier(GL_ALL_SHADER_BITS);

    m_program1->bind();

    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, texture->textureId());

    static const GLfloat vertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f,
        -1.0f, 1.0f,
    };

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_QUADS, 0, 4);
    ++m_frame;
}

void glprogram::initialize()
{
    use_triangle_program();
    texture->create();
    texture->setFormat(QOpenGLTexture::RGBA16F);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();

    texture->setSize(width, height);
    texture->bind();
    texture->allocateStorage();
}

void glprogram::mousePressEvent(QMouseEvent *e)
{

}

void glprogram::keyPressEvent(QKeyEvent *e)
{

}

