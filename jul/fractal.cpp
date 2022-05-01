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
#include <cmath>
#include <fstream>
#include <iostream>
#include <QDir>
#include <QColormap>
#include <QResizeEvent>
#include <QElapsedTimer>
#include "windows.h"
#include <QLibrary>
#include <QRadioButton>
#include <QFontDatabase>

#include "CL/cl.h"

#define vector_size 4
#define SPEED 500

fractal::fractal()
{
    this->setWindowIcon(QIcon("source/images/colorfull_img.png"));
    this->resize(1900, 1000);
    this->setMinimumSize(300, 300);
    this->setWindowTitle("Julia");

    scope_time = 0;

    Re = 0.3;
    Im = -0.01;
    size = 250;
    x0 = 0;
    y0 = 0;
    max_iterations = 1000;
    picture = new QImage(this->width(), this->height(), QImage::Format_ARGB32);

    setUp();
}

void fractal::setUp()
{
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

    background = new QImage();
    background->load("source/images/colorfull_img.png");

    SetMenu();

    color = colorfull;

    pause = false;

    this->repaint();

    t = new QTimer(this);
    t->connect(t, &QTimer::timeout, this, &fractal::Timer);
    t->start(5);

    Timer();
}

void fractal::Timer()
{
    if(scope_time > 1)
    {
        this->setWindowTitle(QString("Приближение ") + QString::number(size));
        --scope_time;
    }
    else
    {
        this->setWindowTitle("Julia");
        scope_time = 0;
    }

    if((!menu && !pause))
    {
        size += SPEED * t->interval() / 1000;
    }

    this->repaint();
}

void fractal::SetMenu()
{    
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

    menu = true;
}

void fractal::SwapRadioButton()
{
    if((*radio_list->begin())->isChecked())
    {
        color = colorfull;
        background->load("source/images/colorfull_img.png");
    }
    else if((*(++radio_list->begin()))->isChecked())
    {
        color = black_white;
        background->load("source/images/black_white_img.png");
    }
    else
    {
        color = smooth;
        background->load("source/images/smooth_img.png");
    }
}

void fractal::makePhoto()
{
    bool tmp = pause;
    pause = true;

    init(3840, 2160);

    upd();

    clEnqueueWriteBuffer(queue, buffer_input, CL_TRUE, 0, sizeof(float) * 10, INPUT, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, buffer_palette, CL_TRUE, 0, sizeof(uchar) * vector_size * 16, PALETTE, NULL, NULL, NULL);

    size_t local_work_size[2] = {localWorkers_size, 1};
    size_t global_work_size[2] =
    {
        size_t((this->width() + local_work_size[0] - 1) / local_work_size[0] * local_work_size[0]),
        size_t((this->height() + local_work_size[1] - 1) / local_work_size[1] * local_work_size[1])
    };

    clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_work_size, local_work_size, 0, NULL, NULL);
    clEnqueueReadBuffer(queue, buffer_gpu_array, CL_TRUE, 0, sizeof(uchar) * 3840 * 2160 * vector_size, GPU_ARRAY, 0, NULL, NULL);
    clFinish(queue);

    img = new QImage(GPU_ARRAY, 3840, 2160, QImage::Format_RGBA8888);
    img->save("photo.jpg");
    init(this->height(), this->width());

    std::cout << "MAKE PHOTO";

    pause = tmp;
}

void fractal::DrawBackGround()
{
    QPainter p(this);
    p.drawImage(0, 0, *background);
}

void fractal::upd()
{
    INPUT[0] = Re; INPUT[1] = Im;
    INPUT[2] = x0; INPUT[3] = y0;
    INPUT[4] = size; INPUT[5] = max_iterations;
    INPUT[6] = this->width();
    INPUT[7] = this->height();

    switch (color)
    {
    case colorfull:
    {
        INPUT[8] = 0;
        break;
    }
    case black_white:
    {
        INPUT[8] = 1;
        break;
    }
    case smooth:
    {
        INPUT[8] = 2;
        break;
    }
    }
}

cl_device_id fractal::create_device()
{
    cl_platform_id platform;
    cl_device_id dev;
    cl_int err = 0;
    err |= clGetPlatformIDs(1, &platform, NULL);
    std::cout<<"clGetPlatformIDs:" << err << std::endl;
    err |= clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
    if(err == CL_DEVICE_NOT_FOUND)
    {
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
    }
    char deviceExtensions[2048];
    cl_int status =clGetDeviceInfo(dev, CL_DEVICE_EXTENSIONS, sizeof(deviceExtensions), deviceExtensions, 0);
    std::cout<<"DeviceExtStatus:" << status << std::endl;
    std::cout<<"DeviceExt:" << deviceExtensions << std::endl;
    if(err) {
        std::cout<<"Device:" << err << std::endl;
    }
    return dev;
}

cl_program fractal::build_program(cl_context ctx)
{
    int err = 0;
    std::string src = get_program_text();
    const char* src_text = src.data();
    size_t src_length = src.size();

    cl_program program = clCreateProgramWithSource(ctx, 1, &src_text, &src_length, &err);
    if(err) {
        std::cout<<"Source:" << src.data() << std::endl;
        std::cout<<"Program0:" << err << std::endl;
    }
    err |= clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

    if(err) {
        std::cout<<"Program1:" << err << std::endl;
    }
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
    cl_uint number_platforms;

    clGetPlatformIDs(0, NULL, &number_platforms);
    cl_platform_id *platform_ids = new cl_platform_id[number_platforms];
    clGetPlatformIDs(number_platforms, platform_ids, &number_platforms);

    std::cout << "nr_platforms: " << number_platforms << std::endl;
    std::cout << "platform: " << platform_ids[0] << std::endl;
    char pbuf[100];
    clGetPlatformInfo(platform_ids[0], CL_PLATFORM_VENDOR, sizeof(pbuf), pbuf, NULL);
    std::cout << "platformInfo: " << pbuf << std::endl;

    std::vector<cl_device_id> devices;

    cl_device_id device_id = create_device();

    clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(localWorkers_size), &localWorkers_size, NULL);
    std::cout << "localWorkers: " << localWorkers_size << std::endl;


    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if(err) {std::cout << "Error: " << err << std::endl;}


    queue = clCreateCommandQueue(context, device_id, 0, &err);
    if(err) {std::cout << "Error: " << err << std::endl;}

    std::string str = get_program_text();


    program = build_program(context);

    std::vector<char> buildLog;
    size_t logSize;
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, nullptr, &logSize);
    std::cout << "logsize:" <<logSize<< std::endl;
    if(logSize <= 50000)
    buildLog.resize(logSize);
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, logSize, buildLog.data(), nullptr);

    std::cout << buildLog.data() << "\n";


    buffer_gpu_array = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(uchar) * w * h * vector_size, NULL, &err);
    buffer_input = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * 10, INPUT, NULL);
    buffer_palette = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uchar) * vector_size * 16, PALETTE, NULL);

    kernel = clCreateKernel(program, "main2", &err);
    if(err) {std::cout << "Ker: " << err << std::endl;}

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer_gpu_array);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer_input);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer_palette);

}

std::string fractal::get_program_text()
{
    std::ifstream t;
    if(color == colorfull)
    {
        t = std::ifstream("source/GPU_colorfull.cl");
    }
    else if(color == black_white)
    {
        t = std::ifstream("source/GPU_black_white.cl");
    }
    else
    {
        t = std::ifstream("source/GPU_smooth.cl");
    }
    std::cout << "Current path is:" << QDir::currentPath().toStdString() << '\n'; // (1)
    return std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
}

void fractal::draw_call()
{
    upd();

    clEnqueueWriteBuffer(queue, buffer_input, CL_TRUE, 0, sizeof(float) * 10, INPUT, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, buffer_palette, CL_TRUE, 0, sizeof(uchar) * vector_size * 16, PALETTE, NULL, NULL, NULL);

    size_t local_work_size[2] = {localWorkers_size, 1};
    size_t global_work_size[2] =
    {
        size_t((this->width() + local_work_size[0] - 1) / local_work_size[0] * local_work_size[0]),
        size_t((this->height() + local_work_size[1] - 1) / local_work_size[1] * local_work_size[1])
    };

    clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_work_size, local_work_size, 0, NULL, NULL);
    clEnqueueReadBuffer(queue, buffer_gpu_array, CL_TRUE, 0, sizeof(uchar) * this->width() * this->height() * vector_size, GPU_ARRAY, 0, NULL, NULL);
    clFinish(queue);

    picture = new QImage(GPU_ARRAY, this->width(), this->height(), QImage::Format_RGBA8888);

    QPainter p(this);
    p.drawImage(0, 0, *picture);
}

void fractal::Start()
{
    if(TextChanged())
    {
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
    }

    btn->setVisible(false);
    btn->setEnabled(false);

    for(auto &i : *lineEdit_list) {i->setVisible(false); i->setEnabled(false);}
    for(auto &i : *label_list) {i->setVisible(false); i->setEnabled(false);}
    for(auto &i : *radio_list) {i->setVisible(false); i->setEnabled(false);}

    pause = false;
    menu = false;

    init(this->width(), this->height());
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
    int id = QFontDatabase::addApplicationFont("source/BadScript-Regular.ttf");
    QString f1 = QFontDatabase::applicationFontFamilies(id).at(0);

    id = QFontDatabase::addApplicationFont("source/agencyfb_reg.ttf");
    QString f2 = QFontDatabase::applicationFontFamilies(id).at(0);

    QFont font1(f1, 15);
    QFont font2(f1, 10);
    QFont font3(f2, 15);
    font1.setPointSize(15);

    btn->setGeometry((this->width() - 290) / 2, this->height() * 0.3 + 230, 290, 50);

    std::list<QLabel*>::iterator i = label_list->begin();
    (*i)->setGeometry((this->width() - 290) / 2, this->height() * 0.3, 50, 30); ++i;
    (*i)->setGeometry((this->width() - 290) / 2 + 150, this->height() * 0.3, 50, 30); ++i;
    (*i)->setGeometry((this->width() - 290) / 2 + 280, this->height() * 0.3, 50, 30); ++i;
    (*i)->setGeometry((this->width() - 290) / 2, this->height() * 0.3 + 30, 130, 50); ++i;
    (*i)->setGeometry((this->width() - 290) / 2, this->height() * 0.3 + 70, 130, 50); ++i;
    (*i)->setGeometry((this->width() - 290) / 2, this->height() * 0.3 + 110, 30, 50); ++i;
    (*i)->setGeometry((this->width() - 290) / 2 + 155, this->height() * 0.3 + 110, 30, 50); ++i;
    (*i)->setGeometry((this->width() - 290) / 2 + 40, this->height() * 0.3 + 170, 70, 25); ++i;
    (*i)->setGeometry((this->width() - 290) / 2 + 105, this->height() * 0.3 + 170, 70, 25); ++i;
    (*i)->setGeometry((this->width() - 290) / 2 + 190, this->height() * 0.3 + 170, 70, 25); ++i;

    for(auto &i : *label_list) {i->setFont(font1);}
    i = label_list->end(); --i;
    (*i)->setFont(font2); --i;
    (*i)->setFont(font2); --i;
    (*i)->setFont(font2); --i;

    switch (color)
    {
    case fractal::colorfull:
    {
        for(auto &i : *label_list) {i->setStyleSheet("QLabel { color : white; }");}
        break;
    }
    case fractal::black_white:
    {
        for(auto &i : *label_list) {i->setStyleSheet("QLabel { color : white; }");}
        break;
    }
    case fractal::smooth:
    {
        for(auto &i : *label_list) {i->setStyleSheet("QLabel { color : black; }");}
        break;
    }
    }

    i = label_list->begin();
    (*i)->setText("C = ("); ++i;
    (*i)->setText("+ i"); ++i;
    (*i)->setText(")"); ++i;
    (*i)->setText("Итерации"); ++i;
    (*i)->setText("Приближение"); ++i;
    (*i)->setText("X = "); ++i;
    (*i)->setText("Y = "); ++i;
    (*i)->setText("Цветной"); ++i;
    (*i)->setText("Черно-белый"); ++i;
    (*i)->setText("Гладкий"); ++i;

    for(auto &i : *lineEdit_list) {i->setFont(font3);}

    std::list<QLineEdit*>::iterator j = lineEdit_list->begin();
    (*j)->setGeometry((this->width() - 290) / 2 + 50, this->height() * 0.3, 100, 30); ++j;
    (*j)->setGeometry((this->width() - 290) / 2 + 180, this->height() * 0.3, 100, 30); ++j;
    (*j)->setGeometry((this->width() - 290) / 2 + 130, this->height() * 0.3 + 40, 160, 30); ++j;
    (*j)->setGeometry((this->width() - 290) / 2 + 130, this->height() * 0.3 + 80, 160, 30); ++j;
    (*j)->setGeometry((this->width() - 290) / 2 + 30, this->height() * 0.3 + 120, 105, 30); ++j;
    (*j)->setGeometry((this->width() - 290) / 2 + 185, this->height() * 0.3 + 120, 105, 30); ++j;

    btn->setStyleSheet("QPushButton { background-color: white; }");
    btn->setFont(font1);
    btn->setText("Начать");

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
    else
    {
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
        x0 += SPEED * 10 / size * t->interval() / 1000;
        this->repaint();
        break;
    }
    case Qt::Key_Left:
    {
        x0 -= SPEED * 10 / size * t->interval() / 1000;
        this->repaint();
        break;
    }
    case Qt::Key_Up:
    {
        y0 -= SPEED * 10 / size * t->interval() / 1000;
        this->repaint();
        break;
    }
    case Qt::Key_Down:
    {
        y0 += SPEED * 10 / size * t->interval() / 1000;
        this->repaint();
        break;
    }
    case Qt::Key_Plus:
    {
        size += size * t->interval() / 1000;
        this->repaint();
        break;
    }
    case Qt::Key_Minus:
    {
        size -= size * t->interval() / 1000;
        this->repaint();
        break;
    }
    case Qt::Key_Escape:
    {
        if(!menu) {pause = true; menu = true; setUp();}
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

void fractal::mouseMoveEvent(QMouseEvent *e)
{
    x0 -= (e->pos().x() - mouse.rx()) / size;
    y0 += (-e->pos().y() + mouse.ry()) / size;

    mouse.rx() = e->pos().x();
    mouse.ry() = e->pos().y();
}

void fractal::mousePressEvent(QMouseEvent *e)
{
    mouse.rx() = e->pos().x();
    mouse.ry() = e->pos().y();
}

void fractal::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);

    if(!menu) {init(this->width(), this->height());}
}

void fractal::wheelEvent(QWheelEvent *e)
{
    if(e->angleDelta().y() > 0) { size += 10 * size * t->interval() / 1000; }
    else { size -= 10 * size * t->interval() / 1000; }

//    x0 -= (e->globalPosition().x() - mouse.rx()) / size;
//    y0 += (-e->globalPosition().y() + mouse.ry()) / size;

//    mouse.rx() = e->globalPosition().x();
//    mouse.ry() = e->globalPosition().y();
}
