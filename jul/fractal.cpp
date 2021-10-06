#include "fractal.h"
#include <QPalette>
#include <QPainter>
#include <complex.h>
#include <QColor>
#include <QPen>
#include <iostream>
#include <QPalette>
#include <QImage>
#include <QFont>
#include <list>
#include <fstream>
#include <QColormap>
#include <QResizeEvent>
#include <QElapsedTimer>
#include "windows.h"
#include <QLibrary>

#include "CL/cl.h"

#define vector_size 4

fractal::fractal()
{
    this->setWindowIcon(QIcon("images/colorfull_img.png"));
    this->resize(1000, 1000);
    this->setMinimumSize(500, 500);
    this->setWindowTitle("Julia");

    btn = new QPushButton(this);

    lineEdit_list = new std::list<QLineEdit*>;
    lineEdit_list->resize(6);
    for(auto &i : *lineEdit_list) {i = new QLineEdit(this);}

    label_list = new std::list<QLabel*>;
    label_list->resize(10);
    for(auto &i : *label_list) {i = new QLabel(this);}

    radio_list = new std::list<QRadioButton*>;
    radio_list->resize(3);



    for(auto &i : *radio_list) {i = new QRadioButton(this);}

    scope_time = 0;

    Re = -0.8;
    Im = 0.156;
    size = 250;
    x0 = 0;
    y0 = 0;
    max_iterations = 250;
    picture = new QImage(this->width(), this->height(), QImage::Format_ARGB32);

    setUp();
}

void fractal::setUp()
{
    color = colorfull;

    SetMenu();

    pause = true;

    background = new QImage();

    this->repaint();

    t = new QTimer(this);
    t->connect(t, &QTimer::timeout, this, &fractal::Timer);
    t->start(1);

    init(this->width(), this->height());

    Timer();
}

void fractal::Timer()
{
    if(scope_time > 1)
    {
        this->setWindowTitle("Julia    " + QString("Scope: ") + QString::number(size));
        --scope_time;
    }
    else
    {
        this->setWindowTitle("Julia");
        scope_time = 0;
    }

    if(!pause)
    {
        size += 5;
    }

    upd();

    this->repaint();
}

void fractal::SetMenu()
{
    menu = true;
    btn->connect(btn, &QPushButton::clicked, this, &fractal::Start);

    std::list<QRadioButton*>::iterator i = radio_list->begin();
    (*i)->setChecked(true);
    (*i)->connect(*i, &QRadioButton::clicked, this, &fractal::SwapRadioButton); ++i;
    (*i)->connect(*i, &QRadioButton::clicked, this, &fractal::SwapRadioButton); ++i;
    (*i)->connect(*i, &QRadioButton::clicked, this, &fractal::SwapRadioButton); ++i;

    std::list<QLineEdit*>::iterator j = lineEdit_list->begin();
    (*j)->setText(QString::number(Re)); ++j;
    (*j)->setText(QString::number(Im)); ++j;
    (*j)->setText(QString::number(max_iterations)); ++j;
    (*j)->setText(QString::number(size)); ++j;
    (*j)->setText(QString::number(x0)); ++j;
    (*j)->setText(QString::number(y0)); ++j;

    j = lineEdit_list->begin();
    (*j)->connect(*j, &QLineEdit::textChanged, this, &fractal::TextChanged); ++j;
    (*j)->connect(*j, &QLineEdit::textChanged, this, &fractal::TextChanged); ++j;
    (*j)->connect(*j, &QLineEdit::textChanged, this, &fractal::TextChanged); ++j;
    (*j)->connect(*j, &QLineEdit::textChanged, this, &fractal::TextChanged); ++j;
    (*j)->connect(*j, &QLineEdit::textChanged, this, &fractal::TextChanged); ++j;
    (*j)->connect(*j, &QLineEdit::textChanged, this, &fractal::TextChanged); ++j;

    btn->setVisible(true);
    btn->setEnabled(true);

    for(auto &i : *lineEdit_list) {i->setVisible(true); i->setEnabled(true);}
    for(auto &i : *label_list) {i->setVisible(true); i->setEnabled(true);}
    for(auto &i : *radio_list) {i->setVisible(true); i->setEnabled(true);}
}

void fractal::SwapRadioButton()
{
    if((*radio_list->begin())->isChecked())
    {
        color = colorfull;
        background->load("images/colorfull_img.png");
    }
    else if((*(++radio_list->begin()))->isChecked())
    {
        color = black_white;
        background->load("images/black_white_img.png");
    }
    else
    {
        color = smooth;
        background->load("images/smooth_img.png");
    }
}

void fractal::makePhoto()
{
    bool tmp = pause;
    pause = true;

    init(4000, 4000);

    clEnqueueWriteBuffer(queue, buffer_input, CL_TRUE, 0, sizeof(float) * 10, INPUT, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, buffer_palette, CL_TRUE, 0, sizeof(uchar) * vector_size * 16, PALETTE, NULL, NULL, NULL);

    size_t global_work_size[2] = {size_t(4000), size_t(4000)};
    size_t local_work_size[2] = {1, 1};
    local_work_size[0] = local_work_size[1] = 1;

    clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_work_size, local_work_size, 0, NULL, NULL);
    clEnqueueReadBuffer(queue, buffer_gpu_array, CL_TRUE, 0, sizeof(uchar) * 4000 * 4000 * vector_size, GPU_ARRAY, 0, NULL, NULL);
    clFinish(queue);

    img = new QImage(GPU_ARRAY, 4000, 4000, QImage::Format_RGBA8888);
    img->save("photo.png");
    init(this->height(), this->width());

    pause = tmp;
}

void fractal::DrawBackGround()
{
    QPainter p(this);

    background = new QImage(background->copy(0, 0, this->width(), this->height()));
    p.drawImage(QRect(0, 0, this->width(), this->height()), *background);
}

void fractal::upd()
{
    INPUT[0] = Re; INPUT[1] = Im;
    INPUT[2] = x0; INPUT[3] = y0;
    INPUT[4] = size; INPUT[5] = max_iterations;
    INPUT[6] = this->width();
}

cl_device_id fractal::create_device()
{
    cl_platform_id platform;
    cl_device_id dev;
    cl_int err = 0;
    err |= clGetPlatformIDs(1, &platform, NULL);
    err |= clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
    if(err == CL_DEVICE_NOT_FOUND)
    {
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
    }
    if(err) {throw;}
    return dev;
}

cl_program fractal::build_program(cl_context ctx)
{
    int err = 0;
    std::string src = get_program_text();
    const char* src_text = src.data();
    size_t src_length = src.size();

    cl_program program = clCreateProgramWithSource(ctx, 1, &src_text, &src_length, &err);
    err |= clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

    if(err) {std::cout << err << " ";}
    return program;
}

void fractal::init(double w, double h)
{
    colors[0].setRgb(66, 30, 15);
    colors[1].setRgb(25, 7, 26);
    colors[2].setRgb(9, 1, 47);
    colors[3].setRgb(4, 4, 73);
    colors[4].setRgb(0, 7, 100);
    colors[5].setRgb(12, 44, 138);
    colors[6].setRgb(24, 82, 177);
    colors[7].setRgb(57, 125, 209);
    colors[8].setRgb(134, 181, 229);
    colors[9].setRgb(211, 236, 248);
    colors[10].setRgb(241, 233, 191);
    colors[11].setRgb(248, 201, 95);
    colors[12].setRgb(255, 170, 0);
    colors[13].setRgb(204, 128, 0);
    colors[14].setRgb(153, 87, 0);
    colors[15].setRgb(106, 52, 3);

    GPU_ARRAY = new uchar[int(w) * int(h) * vector_size];
    INPUT = new float[10];
    INPUT[0] = Re; INPUT[1] = Im;
    INPUT[2] = x0; INPUT[3] = y0;
    INPUT[4] = size; INPUT[5] = max_iterations;
    INPUT[6] = float(w);
    INPUT[7] = float(h);
    PALETTE = new uchar[16 * vector_size];

    for(int i = 0; i<16 ; ++i)
    {
        PALETTE[vector_size * i] = colors[i].red();
        PALETTE[vector_size * i + 1] = colors[i].green();
        PALETTE[vector_size * i + 2] = colors[i].blue();
    }

    cl_int err;
    cl_uint nr_platforms;

    clGetPlatformIDs(0, NULL, &nr_platforms);
    cl_platform_id *platform_ids = new cl_platform_id[nr_platforms];
    clGetPlatformIDs(nr_platforms, platform_ids, &nr_platforms);
    std::vector<cl_device_id> devices;

    cl_device_id device_id = create_device();

    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);

    queue = clCreateCommandQueue(context, device_id, 0, &err);

    std::string str = get_program_text();

    if(err) {std::cout << "Pr: " << err << " ";}

    program = build_program(context);

    std::vector<char> buildLog;
    size_t logSize;
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, nullptr, &logSize);

    buildLog.resize(logSize);
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, logSize, &buildLog[0], nullptr);


    std::cout << &buildLog[0] << "\n";
    if(err) {std::cout << "Ker: " << err << " ";}

    buffer_gpu_array = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(uchar) * w * h * vector_size, NULL, &err);
    buffer_input = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * 10, INPUT, NULL);
    buffer_palette = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uchar) * vector_size * 16, PALETTE, NULL);

    kernel = clCreateKernel(program, "main", &err);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer_gpu_array);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer_input);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer_palette);

    if(err) {std::cout << err;}
}

std::string fractal::get_program_text()
{
    std::ifstream t("GPU.cl");
    return std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
}

QColor fractal::getColor(double mu)
{
    int color1 = int(mu);
    int color2 = (color1 + 1) % 16;
    double t2 = mu - color1;
    double t1 = 1 - t2;
    color1 %= 16;

    int r = int(colors[color1].red() * t1 + colors[color2].red() * t2);
    int g = int(colors[color1].green() * t1 + colors[color2].green() * t2);
    int b = int(colors[color1].blue() * t1 + colors[color2].blue() * t2);

    return QColor(r, g, b);
}

void fractal::draw_call()
{
//    QElapsedTimer tmr;
//    tmr.start();

    clEnqueueWriteBuffer(queue, buffer_input, CL_TRUE, 0, sizeof(float) * 10, INPUT, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, buffer_palette, CL_TRUE, 0, sizeof(uchar) * vector_size * 16, PALETTE, NULL, NULL, NULL);

    picture = new QImage(GPU_ARRAY, this->width(), this->height(), QImage::Format_RGBA8888);

    size_t global_work_size[2] = {size_t(this->width()), size_t(this->height())};
    size_t local_work_size[2] = {1, 1};
    local_work_size[0] = local_work_size[1] = 1;

    clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_work_size, local_work_size, 0, NULL, NULL);
    clEnqueueReadBuffer(queue, buffer_gpu_array, CL_TRUE, 0, sizeof(uchar) * this->width() * this->height() * vector_size, GPU_ARRAY, 0, NULL, NULL);
    clFinish(queue);

//    this->setWindowTitle(QString::number(tmr.elapsed()));


    QPainter p(this);
    p.drawImage(0, 0, *picture);
}

void fractal::Start()
{
    if(TextChanged())
    {
        btn->setVisible(false);
        btn->setEnabled(false);

        for(auto &i : *lineEdit_list) {i->setVisible(false); i->setEnabled(false);}
        for(auto &i : *label_list) {i->setVisible(false); i->setEnabled(false);}
        for(auto &i : *radio_list) {i->setVisible(false); i->setEnabled(false);}

        if(color == colorfull)
        {
            colors[0].setRgb(66, 30, 15);
            colors[1].setRgb(25, 7, 26);
            colors[2].setRgb(9, 1, 47);
            colors[3].setRgb(4, 4, 73);
            colors[4].setRgb(0, 7, 100);
            colors[5].setRgb(12, 44, 138);
            colors[6].setRgb(24, 82, 177);
            colors[7].setRgb(57, 125, 209);
            colors[8].setRgb(134, 181, 229);
            colors[9].setRgb(211, 236, 248);
            colors[10].setRgb(241, 233, 191);
            colors[11].setRgb(248, 201, 95);
            colors[12].setRgb(255, 170, 0);
            colors[13].setRgb(204, 128, 0);
            colors[14].setRgb(153, 87, 0);
            colors[15].setRgb(106, 52, 3);
        }

        pause = false;
        menu = false;
    }
}

bool fractal::TextChanged()
{
    std::list<QLineEdit*>::iterator i = lineEdit_list->begin();
    QString s1 = (*i)->text(); ++i;
    QString s2 = (*i)->text(); ++i;
    QString s3 = (*i)->text(); ++i;
    QString s4 = (*i)->text(); ++i;
    QString s5 = (*i)->text(); ++i;
    QString s6 = (*i)->text(); ++i;

    bool c1; bool c2; bool c3; bool c4; bool c5; bool c6; bool c7 = true;
    double a = s1.toDouble(&c1);
    double b = s2.toDouble(&c2);
    double c = s3.toDouble(&c3);
    double d = s4.toDouble(&c4);
    double e = s5.toDouble(&c5);
    double f = s6.toDouble(&c6);

    std::vector<QPalette> p(lineEdit_list->size());

    if(c1 && a >= -2 && a <= 2)
    {
        p[0].setColor(QPalette::Text, QColor(0, 169, 28));
    }
    else {p[0].setColor(QPalette::Text, QColor(235, 0, 0)); c7 = false;}

    if(c2 && b >= -2 && b <= 2)
    {
        p[1].setColor(QPalette::Text, QColor(0, 169, 28));
    }
    else {p[1].setColor(QPalette::Text, QColor(235, 0, 0)); c7 = false;}

    if(c3 && c > 0)
    {
        p[2].setColor(QPalette::Text, QColor(0, 169, 28));
    }
    else {p[2].setColor(QPalette::Text, QColor(235, 0, 0)); c7 = false;}

    if(c4 && d > 0)
    {
        p[3].setColor(QPalette::Text, QColor(0, 169, 28));
    }
    else {p[3].setColor(QPalette::Text, QColor(235, 0, 0)); c7 = false;}

    if(c5 && e < 10000 && c > -10000)
    {
        p[4].setColor(QPalette::Text, QColor(0, 169, 28));
    }
    else {p[4].setColor(QPalette::Text, QColor(235, 0, 0)); c7 = false;}

    if(c6 && f < 10000 && f > -10000)
    {
        p[5].setColor(QPalette::Text, QColor(0, 169, 28));
    }
    else {p[5].setColor(QPalette::Text, QColor(235, 0, 0)); c7 = false;}

    int j = 0;
    for(auto &i : *lineEdit_list) {i->setPalette(p[j]); ++j;}

    if(c7) {Re = a, Im = b; max_iterations = c; size = d; x0 = e; y0 = f;}
    return c7;
}

void fractal::DrawMenu()
{
    QFont font1("Bad Script", 15);
    QFont font2("Bad Script", 10);
    QFont font3("Agency FB", 15);
    font1.setPointSize(15);

    btn->setGeometry((this->width() - 290) / 2, this->height() * 0.3 + 230, 290, 50);

    std::list<QLabel*>::iterator i = label_list->begin();
    (*i)->setGeometry((this->width() - 290) / 2, this->height() * 0.3, 50, 30); ++i;
    (*i)->setGeometry((this->width() - 290) / 2 + 150, this->height() * 0.3, 50, 30); ++i;
    (*i)->setGeometry((this->width() - 290) / 2 + 280, this->height() * 0.3, 50, 30); ++i;
    (*i)->setGeometry((this->width() - 290) / 2, this->height() * 0.3 + 30, 130, 50); ++i;
    (*i)->setGeometry((this->width() - 290) / 2, this->height() * 0.3 + 70, 70, 50); ++i;
    (*i)->setGeometry((this->width() - 290) / 2, this->height() * 0.3 + 110, 30, 50); ++i;
    (*i)->setGeometry((this->width() - 290) / 2 + 155, this->height() * 0.3 + 110, 30, 50); ++i;
    (*i)->setGeometry((this->width() - 290) / 2 + 40, this->height() * 0.3 + 170, 70, 20); ++i;
    (*i)->setGeometry((this->width() - 290) / 2 + 105, this->height() * 0.3 + 170, 70, 20); ++i;
    (*i)->setGeometry((this->width() - 290) / 2 + 190, this->height() * 0.3 + 170, 70, 20); ++i;

    for(auto &i : *label_list) {i->setFont(font1);}
    i = label_list->end(); --i;
    (*i)->setFont(font2); --i;
    (*i)->setFont(font2); --i;
    (*i)->setFont(font2); --i;

    switch (color)
    {
    case colorfull:
    {
        for(auto &i : *label_list) {i->setStyleSheet("QLabel { color : white; }");}
        break;
    }
    case black_white:
    {
        for(auto &i : *label_list) {i->setStyleSheet("QLabel { color : white; }");}
        break;
    }
    case smooth:
    {
        for(auto &i : *label_list) {i->setStyleSheet("QLabel { color : black; }");}
        break;
    }
    }

    i = label_list->begin();
    (*i)->setText("C = ("); ++i;
    (*i)->setText("+ i"); ++i;
    (*i)->setText(")"); ++i;
    (*i)->setText("Max iterations = "); ++i;
    (*i)->setText("Scope = "); ++i;
    (*i)->setText("X = "); ++i;
    (*i)->setText("Y = "); ++i;
    (*i)->setText("Colorfull"); ++i;
    (*i)->setText("Black-white"); ++i;
    (*i)->setText("Smooth"); ++i;

    for(auto &i : *lineEdit_list) {i->setFont(font3);}

    std::list<QLineEdit*>::iterator j = lineEdit_list->begin();
    (*j)->setGeometry((this->width() - 290) / 2 + 50, this->height() * 0.3, 100, 30); ++j;
    (*j)->setGeometry((this->width() - 290) / 2 + 180, this->height() * 0.3, 100, 30); ++j;
    (*j)->setGeometry((this->width() - 290) / 2 + 130, this->height() * 0.3 + 40, 160, 30); ++j;
    (*j)->setGeometry((this->width() - 290) / 2 + 65, this->height() * 0.3 + 80, 225, 30); ++j;
    (*j)->setGeometry((this->width() - 290) / 2 + 30, this->height() * 0.3 + 120, 105, 30); ++j;
    (*j)->setGeometry((this->width() - 290) / 2 + 185, this->height() * 0.3 + 120, 105, 30); ++j;

    btn->setStyleSheet("QPushButton { background-color: white; }");
    btn->setFont(font1);
    btn->setText("Start");

    std::list<QRadioButton*>::iterator k = radio_list->begin();
    (*k)->setGeometry((this->width() - 290) / 2 + 55, this->height() * 0.3 + 190, 20, 20); ++k;
    (*k)->setGeometry((this->width() - 290) / 2 + 130, this->height() * 0.3 + 190, 20, 20); ++k;
    (*k)->setGeometry((this->width() - 290) / 2 + 205, this->height() * 0.3 + 190, 20, 20); ++k;
}

void fractal::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    if(menu)
    {
        DrawMenu();
        TextChanged();
        DrawBackGround();
    }
    else {//DrawFractal();
        draw_call();
    }
}

void fractal::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case 16777220:
    {
        if(menu) {Start();}
        else
        {
            bool c;
            double new_scope = string.toDouble(&c);
            if(c && new_scope > 0)
            {
                size = new_scope;
            }
            string = "";
            this->repaint();
        }
        break;
    }
    case Qt::Key_Space:
    {
        pause = pause ^ 1;
        break;
    }
    case Qt::Key_Right:
    {
        x0 += 50 / size;
        this->repaint();
        break;
    }
    case Qt::Key_Left:
    {
        x0 -= 50 / size;
        this->repaint();
        break;
    }
    case Qt::Key_Up:
    {
        y0 -= 50 / size;
        this->repaint();
        break;
    }
    case Qt::Key_Down:
    {
        y0 += 50 / size;
        this->repaint();
        break;
    }
    case Qt::Key_Plus:
    {
        size += size / 50;
        this->repaint();
        break;
    }
    case Qt::Key_Minus:
    {
        size -= size / 50;
        this->repaint();
        break;
    }
    case Qt::Key_Escape:
    {
        if(!menu) {setUp();}
        break;
    }
    case Qt::Key_S:
        if(!menu) {makePhoto();}
    }

    if(!menu && e->key() >= Qt::Key_0 && e->key() <= Qt::Key_9)
    {
        string.push_back(e->text());
    }
}

void fractal::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);

    if(!menu)
    {
        scope_time = 15;
    }

    x0 += ((e->pos().x() - this->width() / 2) / size); y0 -= ((this->height() / 2 - e->pos().y()) / size);
    this->repaint();
}

void fractal::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    init(e->size().width(), e->size().height());
}
