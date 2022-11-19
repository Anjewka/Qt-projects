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
#include <QFileInfo>

#define PI 3.1415926535

glprogram::glprogram()
{
    texture = new QOpenGLTexture(QOpenGLTexture::Target2D);

    size = 200;
    angel_x = 0;
    angel_y = PI / 2;
    mouse_x = 0.0;
    mouse_y = 0.0;
    pause = false;
    pressed = false;
    z0 = -2.5;

    camera_point = QVector3D(0, 0, -2);
    linse_point = QVector3D(0, 0, -1.6);

    img.load("./assets/palette3.png");
}

void glprogram::use_triangle_program()
{
    palette = new QOpenGLTexture(img);

    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "./assets/vertex_shader.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "./assets/fragment_shader.glsl");
    m_program->link();

    frac = new QOpenGLShaderProgram();
    frac->addShaderFromSourceFile(QOpenGLShader::Compute, "./assets/compute_shader.glsl");
    frac->link();

    if(QFileInfo::exists("./assets/compute_shader")) {std::cout << "TRUEE";}
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
        GLfloat* tmp = &vars[0];
        glUniform1fv(address, 100 * sizeof(GLfloat), tmp);
        break;
    }
}

void glprogram::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    !pause ? size *= 1.005 : size *= 1;

    glClear(GL_COLOR_BUFFER_BIT);

    frac->bind();

    std::vector<float> a = {size};
    setVar("size", frac->programId(), a, false);
    a = {float(width())};
    setVar("width", frac->programId(), a, false);
    a = {float(height())};
    setVar("height", frac->programId(), a, false);
    a = {angel_x};
    setVar("angel_x", frac->programId(), a, false);
    a = {angel_y};
    setVar("angel_y", frac->programId(), a, false);
    a = {z0};
    setVar("z0", frac->programId(), a, false);

    std::vector<float> cam(3);
    cam[0] = linse_point.x(); cam[1] = linse_point.y(); cam[2] = linse_point.z();
    setVar("camera", frac->programId(), cam, false);

    glBindImageTexture(0, texture->textureId(), 0, false, 0, GL_WRITE_ONLY, texture->format());
    glBindImageTexture(1, palette->textureId(), 0, false, 0, GL_READ_ONLY, palette->format());

    glDispatchCompute(width() / 16 + 1, height() / 16 + 1, 1);

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

void glprogram::mousePressEvent(QMouseEvent *e)
{
    pressed = true;

    mouse_x = e->pos().x();
    mouse_y = e->pos().y();
}

void glprogram::mouseMoveEvent(QMouseEvent *e)
{
    if(pressed)
    {
        float dx = (e->pos().x() - this->width() / 2);
        float dy = (-e->pos().y() + this->height() / 2);

        angel_x -= 2 * (mouse_x - this->width() / 2 - dx) / this->width() * PI / 3;
        angel_y += 2 * (this->height() / 2 - mouse_y - dy) / this->height() * PI / 3;

        mouse_x = e->pos().x();
        mouse_y = e->pos().y();
    }
}

void glprogram::mouseReleaseEvent(QMouseEvent *e)
{
    pressed = false;
}

void glprogram::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_W:
        z0 += 0.005;
        break;
    case Qt::Key_S:
        z0 -= 0.005;
        break;
    }
}

