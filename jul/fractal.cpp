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

fractal::fractal()
{
    this->setWindowIcon(QIcon("images/smooth_img.png"));
    this->resize(800, 800);
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

    Re = -0.74543;
    Im = 0.11301;
    size = 250;
    x0 = y0 = 0;
    max_iterations = 255;

    setUp();
}

void fractal::setUp()
{
    t = new QTimer(this);
    t->connect(t, &QTimer::timeout, this, &fractal::Timer);
    t->start(100);

    color = colorfull;

    stop = true;
    SetMenu();

    background = new QImage();
    background->load("images/colorfull_img.png");

    previous = 0;
    this->repaint();

    Timer();
}

void fractal::Timer()
{
    if(menu || !stop) {this->repaint();}
    if(size > 10000) {stop = true;}
    size += stop ? 0 : size / 50;

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

void fractal::DrawFractal()
{
    QPainter p(this);

    for(int x = -this->width() / 2; x<this->width() / 2; ++x)
    {
        for(int y = -this->height() / 2; y<this->height() / 2; ++y)
        {
            Complex z(x, y);
            z = z / size + Complex(x0, y0);
            c = Complex(Re, Im);

            int i = 0;
            while (z.modul() < 4 && i < max_iterations)
            {
                z = z * z + c;
                i++;
            }

            QColor col;

            switch(color)
            {
            case colorfull:
            {
                double mu = i + 1 - log(log(z.modul())) / log(2);
                col = getColor(mu);
                break;
            }
            case black_white:
            {
                double r = 255 * i / max_iterations;
                double g = 255 * i / max_iterations;
                double b = 255 * i / max_iterations;

                col = QColor(r, g, b);
                break;
            }
            case smooth:
            {
                double r = 255 * i / max_iterations;
                double g = 255 - 255 * i / max_iterations;
                double b = 255 * (z.modul() > 4 ? 1 : z.modul() / 4);

                r > 200 ? void(r -= 100) : void();

                col = QColor(r, g, b);
                break;
            }
            }

            QPen pen(col, Qt::SolidLine);
            p.setPen(pen);
            p.drawPoint(x + this->width() / 2, -y + this->height() / 2);
        }
    }
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

    return QColor(r, g, b) != Qt::white ? QColor(r, g, b) : Qt::black;
}

void fractal::makePhoto()
{
    bool tmp = stop;
    stop = true;
    img = new QImage(this->width(), this->height(), QImage::Format_ARGB32);

    for(int x = -this->width() / 2; x<this->width() / 2; ++x)
    {
        for(int y = -this->height() / 2; y<this->height() / 2; ++y)
        {
            Complex z(x, y);
            z = z / size + Complex(x0, y0);
            c = Complex(Re, Im);

            int i = 0;
            while (z.modul() < 4 && i < max_iterations)
            {
                z = z * z + c;
                i++;
            }

            QColor col = Qt::white;

            switch(color)
            {
            case colorfull:
            {
                double mu = i + 1 - log(log(z.modul())) / log(2);
                if(i > 0 && i < max_iterations) {col = getColor(mu);}
                else {col = Qt::white;}
                break;
            }
            case black_white:
            {
                double r = 255 * i / max_iterations;
                double g = 255 * i / max_iterations;
                double b = 255 * i / max_iterations;

                col = QColor(r, g, b);
                break;
            }
            case smooth:
            {
                double r = 255 * i / max_iterations;
                double g = 255 - 255 * i / max_iterations;
                double b = 255 * (z.modul() > 4 ? 1 : z.modul() / 4);

                r > 200 ? void(r -= 100) : void();

                col = QColor(r, g, b);
                break;
            }
            }
            img->setPixelColor(x + this->width() / 2, y + this->height() / 2, col);
        }
    }
    img->save("photo.png");
    stop = tmp;
}

void fractal::DrawBackGround()
{
    QPainter p(this);

    p.drawImage(QRect(0, 0, this->width(), this->height()), *background);
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

        menu = false;
        stop = false;
        this->repaint();
        stop = true;
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
    else {DrawFractal();}
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
            stop ? this->repaint() : void();
        }
        break;
    }
    case Qt::Key_Space:
    {
        stop = stop ? false : true;
        break;
    }
    case Qt::Key_Right:
    {
        x0 += 50 / size;
        stop ? this->repaint() : void();
        break;
    }
    case Qt::Key_Left:
    {
        x0 -= 50 / size;
        stop ? this->repaint() : void();
        break;
    }
    case Qt::Key_Up:
    {
        y0 += 50 / size;
        stop ? this->repaint() : void();
        break;
    }
    case Qt::Key_Down:
    {
        y0 -= 50 / size;
        stop ? this->repaint() : void();
        break;
    }
    case Qt::Key_Plus:
    {
        size += size / 50;
        stop ? this->repaint() : void();
        break;
    }
    case Qt::Key_Minus:
    {
        size -= size / 50;
        stop ? this->repaint() : void();
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
}
