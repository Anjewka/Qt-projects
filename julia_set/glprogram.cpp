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

glprogram::glprogram()
{
    texture = new QOpenGLTexture(QOpenGLTexture::Target2D);

    size = 250;
    x0 = y0 = 0;
    mouse_x = 0.0;
    mouse_y = 0.0;
    pause = false;

    img.load("assets\\palette3.png");
}

void glprogram::use_triangle_program()
{
    palette = new QOpenGLTexture(img);

    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "assets\\vertex_shader.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "assets\\fragment_shader.glsl");
    m_program->link();

    frac = new QOpenGLShaderProgram();
    frac->addShaderFromSourceFile(QOpenGLShader::Compute, "assets\\compute_shader.glsl");
    frac->link();

    diff = new QOpenGLShaderProgram();
    diff->addShaderFromSourceFile(QOpenGLShader::Compute, "assets\\ray_tracing.glsl");
    diff->link();
}

void glprogram::setVar(const std::string&& name, const int& mCurrentProgramActive, const std::vector<int>& vars)
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

void glprogram::setVar(const std::string&& name, const int& mCurrentProgramActive, const std::vector<float>& vars, bool transpose) {
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
    }
}

void glprogram::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    !pause ? size *= 1.004 : size *= 1;

    glClear(GL_COLOR_BUFFER_BIT);

    frac->bind();

    std::vector<float> a = {size};
    setVar("size", frac->programId(), a, false);
    a = {float(width())};
    setVar("width", frac->programId(), a, false);
    a = {float(height())};
    setVar("height", frac->programId(), a, false);
    a = {x0};
    setVar("x0", frac->programId(), a, false);
    a = {y0};
    setVar("y0", frac->programId(), a, false);

    glBindImageTexture(0, texture->textureId(), 0, false, 0, GL_WRITE_ONLY, texture->format());
    glBindImageTexture(1, palette->textureId(), 0, false, 0, GL_READ_ONLY, palette->format());

    glDispatchCompute(width() / 16 + 1, height() / 16 + 1, 1);

    glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT);
    glMemoryBarrier(GL_ALL_SHADER_BITS);

//    diff->bind();

//    glBindImageTexture(0, texture->textureId(), 0, false, 0, GL_WRITE_ONLY, texture->format());
//    glBindImageTexture(1, texture->textureId(), 0, false, 0, GL_READ_ONLY, texture->format());
//    glBindImageTexture(2, palette->textureId(), 0, false, 0, GL_READ_ONLY, palette->format());

//    glDispatchCompute(width() / 16 + 1, height() / 16 + 1, 1);

    glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT);
    glMemoryBarrier(GL_ALL_SHADER_BITS);

    m_program->bind();

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
    m_program->release();
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

//void glprogram::mousePressEvent(QMouseEvent *e)
//{
//    x0 += (e->pos().x() - width() / 2) / size;
//    y0 -= (e->pos().y() - height() / 2) / size;
//}

//void glprogram::keyPressEvent(QKeyEvent *e)
//{
//    pause ^= 1;
//}

