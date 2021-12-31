#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "openglwindow.h"

#include <QGuiApplication>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QtMath>
#include <fstream>
#include <iostream>
#include <QDebug>
#include <QOpenGLTexture>
#include <QWidget>
#include <QMouseEvent>
#include <QApplication>
#include <QKeyEvent>
#include <QTimer>

class glprogram : public OpenGLWindow
{
private:
    QOpenGLTexture *texture;
    GLint m_posAttr;
    GLint m_colAttr;
    GLint m_matrixUniform;
    float size;
    int m_frame;
    float x0, y0;
    float mouse_x, mouse_y;
    QOpenGLTexture *palette;

    QOpenGLShaderProgram *m_program = nullptr;
    QOpenGLShaderProgram *frac = nullptr;
    QOpenGLShaderProgram *diff = nullptr;
    QImage img;
    bool pause;

public:
    glprogram();

    void use_triangle_program();

    void make_texture();

    void setVar(const std::string&& name, const int& mCurrentProgramActive, const std::vector<int>& vars);

    void setVar(const std::string&& name, const int& mCurrentProgramActive, const std::vector<float>& vars, bool transpose = false);

    void render() override;

    void initialize() override;
protected:

//    void mousePressEvent(QMouseEvent *e) override;
//    void keyPressEvent(QKeyEvent *e) override;
};

#endif // GLPROGRAM_H
