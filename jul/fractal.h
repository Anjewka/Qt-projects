#ifndef FRACTAL_H
#define FRACTAL_H

#include <QWidget>
#include <QTimer>
#include <QPaintEvent>
#include <complex.h>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QTimerEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QWheelEvent>

#include "CL/cl.h"

class fractal : public QWidget
{
private:
    QTimer *t;
    QPoint mouse;
    double size;
    double Re, Im;
    double x0, y0;
    double mouse_x, mouse_y;
    double max_iterations;
    Complex c;

    QPushButton *btn;

    std::list<QLineEdit*> *lineEdit_list;
    std::list<QLabel*> *label_list;
    std::list<QRadioButton*> *radio_list;

    bool menu;
    bool pause;
    bool opencl = false;


    QColor colors[32];
    QImage *background;
    QImage *picture;
    QImage *img;

    QPainter p;

    cl_kernel kernel;
    cl_mem buffer_gpu_array;
    cl_mem buffer_input;
    cl_mem buffer_palette;
    cl_command_queue queue;
    uchar *GPU_ARRAY;
    float *INPUT;
    uchar *PALETTE;
    cl_context context;
    cl_program program;

    QString string;
    int scope_time;
    double angel;
    size_t localWorkers_size;

public:
    fractal();

    void Timer();
    void DrawFractal();
    void DrawMenu();
    void SetMenu();
    void setUp();
    void Start();
    bool TextChanged();
    void SwapRadioButton();
    void makePhoto();
    void DrawBackGround();
    void upd();

    void init(double w, double h);
    cl_device_id create_device();
    cl_program build_program(cl_context ctx);
    std::string get_program_text();
    void draw_call();

    enum Color
    {
        colorfull,
        black_white,
        smooth
    };

    Color color;

protected:
    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void resizeEvent(QResizeEvent* e) override;
    void wheelEvent(QWheelEvent *e) override;
};

#endif // FRACTAL_H
