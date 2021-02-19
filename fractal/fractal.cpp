#include "fractal.h"
#include <QPalette>
#include <QPainter>
#include <complex.h>
#include <QColor>
#include <QPen>

fractal::fractal()
{
    this->resize(800, 800);
    t = new QTimer(this);
    t->connect(t, &QTimer::timeout, this, &fractal::Timer);
    t->start(1);
    size = 0.005;
    cx = this->width() / 2;
    cy = this->height() / 2;
}

void fractal::Timer()
{
    this->repaint();
    size -= 0.00001;
    cx += 1;
}

void fractal::Draw()
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    for(int x = -this->width() / 2; x<this->width() / 2 ; ++x)
    {
        for(int y = -this->height() / 2; y<this->height() / 2 ; ++y)
        {
            Complex z(x * size, y * size);
            c = Complex(-0.2, 0.75);
            int i = 0;
            while (z.modul() < 16 && i < 50)
            {
                Complex tmp = z;
                z.x = z.x * z.x - z.y * z.y + c.x;
                z.y = 2 * tmp.x * tmp.y + c.y;
                i++;
            }

            double r = 3 + i * 3 * 2;
            double g = 2 + i * 3 * 3;
            double b = 1 + i * 3 * 1;

            QPen pen(QColor(r, g, b), 1, Qt::SolidLine);
            p.setPen(pen);
            p.drawPoint(x + this->width() / 2, y + this->height() / 2);
        }
    }
}

void fractal::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    Draw();
}
