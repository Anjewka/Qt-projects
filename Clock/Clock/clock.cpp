#include "clock.h"
#include <QPalette>
#include <QPainter>
#include <QTime>
#include <cmath>
#include <QPainterPath>

Clock::Clock()
{
    this->resize(900, 900);
    this->setWindowTitle("Tik-tak");
    this->setMinimumSize(900, 900);
    this->setMaximumSize(900, 900);
    QPalette p;
    p.setColor(QPalette::Background, Qt::white);
    this->setPalette(p);

    fs = 90;
    sec = 0;

    fmin = 90;
    min = 0;

    fh = 90;
    h = 0;

    cx = std::min(this->width(), this->height()) / 2;
    cy = std::min(this->width(), this->height()) / 2;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Clock::Timer);
    setButton();
}

void Clock::Timer()
{
    this->repaint();
}

void Clock::DrawSec()
{
    QPainter p(this);
    QPen pen(Qt::red, 2, Qt::SolidLine);
    p.setPen(pen);

    double x1 = r * cos(fs * 3.1415926 / 180);
    double y1 = r * sin(fs * 3.1415926 / 180);

    if(x1 > 0 || x1 < 0) {x1 += cx;}
    else if(x1 == 0) {x1 = cx;}

    if(y1 > 0 || y1 < 0) {y1 = cy - y1;}
    else if(y1 == 0) {y1 = cy;}

    double x2 = (x1 - cx) / r * 30;
    double y2 = (y1 - cy) / r * 30;

    sec += 1;
    fs -= 6;

    if(fs + 270 == 0) {fs = 90;}

    p.setRenderHint(QPainter::Antialiasing);
    p.drawLine(cx - x2, cy - y2, x1, y1);
}

void Clock::DrawMin()
{
    QPainter p(this);
    QPen pen(Qt::blue, 3, Qt::SolidLine);
    p.setPen(pen);

    double x1 = r * 0.85 * cos(fmin * 3.1415926 / 180);
    double y1 = r * 0.85 * sin(fmin * 3.1415926 / 180);

    if(x1 > 0 || x1 < 0) {x1 += cx;}
    else if(x1 == 0) {x1 = cx;}

    if(y1 > 0 || y1 < 0) {y1 = cy - y1;}
    else if(y1 == 0) {y1 = cy;}

    double x2 = (x1 - cx) / r * 20;
    double y2 = (y1 - cy) / r * 20;

    if(fmin + 270 == 0) {fmin = 90;}
    fmin -= 0.1;

    p.setRenderHint(QPainter::Antialiasing);
    p.drawLine(cx - x2, cy - y2, x1, y1);

    if(sec - 60 == 0)
    {
        sec = 0;
        min++;
    }
}

void Clock::DrawH()
{
    QPainter p(this);
    QPen pen(Qt::green, 5, Qt::SolidLine);
    p.setPen(pen);

    double x1 = r * 0.7 * cos(fh * 3.1415926 / 180);
    double y1 = r * 0.7 * sin(fh * 3.1415926 / 180);

    if(x1 > 0 || x1 < 0) {x1 += cx;}
    else if(x1 == 0) {x1 = cx;}

    if(y1 > 0 || y1 < 0) {y1 = cy - y1;}
    else if(y1 == 0) {y1 = cy;}

    double x2 = (x1 - cx) / r * 20;
    double y2 = (y1 - cy) / r * 20;

    fh += 0.1/60;
    if(fh + 270 == 0) {fh = 90;}

    p.setRenderHint(QPainter::Antialiasing);
    p.drawLine(cx - x2, cy - y2, x1, y1);

    if(min - 60 == 0)
    {
        min = 0;
        h++;
    }
}

void Clock::DrawCircle()
{
    QPainter p(this);

    p.setRenderHint(QPainter::Antialiasing);
    QPen apen;
    apen = QPen(Qt::black);
    apen.setWidth(8);
    p.setPen(apen);
    p.drawEllipse(cx - rclock - 40, cy - rclock - 40, 2 * rclock + 80, 2 * rclock + 80);

    apen = QPen(QColor("red"));
    apen.setWidth(8);
    p.setPen(apen);
    p.drawArc(cx - rclock - 10, cy - rclock - 10, 2 * rclock + 20, 2 * rclock + 20, 90 * 16, (fs - 90) * 16);

    apen = QPen(Qt::blue);
    apen.setWidth(8);
    p.setPen(apen);
    p.drawArc(cx - rclock - 20, cy - rclock - 20, 2 * rclock + 40, 2 * rclock + 40, 90 * 16, (fmin - 90) * 16);

    apen = QPen(Qt::green);
    apen.setWidth(8);
    p.setPen(apen);
    p.drawArc(cx - rclock - 30, cy - rclock - 30, 2 * rclock + 60, 2 * rclock + 60, 90 * 16, (fh - 90) * 16);
}

void Clock::setButton()
{
    QPalette p;
    btn = new QPushButton(this);
    p.setColor(QPalette::Background, Qt::white);
    btn->setPalette(p);
    btn->setGeometry(0.4 * this->width(), 0.05 * this->height(), 0.2 * this->width(), 0.05 * this->height());
    btn->setText("set system time");
    connect(btn, &QPushButton::clicked, this, &Clock::setSystemTime);
}

void Clock::Start()
{
    timer->start(1000);
}

void Clock::setSystemTime()
{
    timer->stop();

    QTime time = QTime::currentTime();
    QString timestring = time.toString();
    QString str;

    if(timestring[6] != '0') {str = timestring[6];} str += timestring[7];
    fs = -6 * str.toDouble() + 90 + 18; sec = str.toInt(); str.clear();

    if(timestring[3] != '0') {str = timestring[3];} str += timestring[4];
    fmin = -6 * str.toDouble() + 90 - 0.1 * sec; min = str.toInt(); str.clear();

    if(timestring[0] != '0') {str = timestring[0];} str += timestring[1];
    fh = -30 * (str.toInt() % 12) + 90 - 0.5 * min; h = str.toInt();

    Start();
}

void Clock::DrawLines()
{

    QPainter p(this);
    QPen pen;
    pen = QPen(Qt::black, 4, Qt::SolidLine);
    p.setPen(pen);
    int f = 90;
    while(f >= -270)
    {
        double x1 = cx + rclock * cos(f * 3.1415926 / 180);
        double y1 = cy - rclock * sin(f * 3.1415926 / 180);
        double x2 = cx + rclock * cos(f * 3.1415926 / 180) * 0.95;
        double y2 = cy - rclock * sin(f * 3.1415926 / 180) * 0.95;
        double dx = cx + rclock * cos(f * 3.1415926 / 180) * 0.9;
        double dy = cy - rclock * sin(f * 3.1415926 / 180) * 0.9;

        p.setRenderHint(QPainter::Antialiasing);
        if(f % 30 == 0)
        {
            p.drawLine(x1, y1, dx, dy);
        }
        p.drawLine(x1, y1, x2, y2);
        f -= 6;
    }

    QPen pn(Qt::black, 2, Qt::SolidLine);
    p.setPen(pn);
    p.setBrush(Qt::white);
    p.drawEllipse(cx - 10, cy - 10, 20, 20);
}

void Clock::upd()
{
    r = std::min(this->width(), this->height()) / 4;
    rclock = 1.2 * r;
    cx = std::min(this->width(), this->height()) / 2;
    cy = std::min(this->width(), this->height()) / 2;
}

void Clock::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    DrawCircle();
    DrawSec();
    DrawMin();
    DrawH();
    DrawLines();
    upd();
}
