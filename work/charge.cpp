#include "charge.h"

Charge::Charge()
{
    v = new Speed(0, 0);
    a = new acceleratio(0, 0);
    sign = "+";
    x = 1;
    y = 1;
    m = 9e-10;
    q = 1.6e-19;
    color = QColor(255, 255, 255);
}

Charge::Charge(Speed *v1, double x1, double y1, double m1, double q1, QString s, QColor c)
{
    v = v1;
    x = x1;
    y = y1;
    m = m1;
    q = q1;
    a = new acceleratio(0, 0);
    if(s == "+" || s == "-") {sign = s;}
    else sign = "+";
    color = c;
}

Speed *Charge::getV()
{
    return v;
}

double Charge::getVx()
{
    return v->getX();
}

double Charge::getVy()
{
    return v->getY();
}

acceleratio *Charge::getA()
{
    return a;
}

QString Charge::getSign()
{
    return sign;
}

QColor Charge::getColor()
{
    return color;
}

double Charge::getX()
{
    return x;
}

double Charge::getY()
{
    return y;
}

double Charge::getM()
{
    return m;
}

double Charge::getQ()
{
    return q;
}

void Charge::setSpeed(Speed *v1)
{
    v = v1;
}

void Charge::setA(acceleratio *a1)
{
    a = a1;
}

void Charge::setA(double x1, double y1)
{
    a = new acceleratio(x1, y1);
}

void Charge::setSpeed(double x1, double y1)
{
    v = new Speed(x1, y1);
}

void Charge::setSign(QString s)
{
    if(s == "+" || s == "-") {sign = s;}
}

void Charge::setColor(int a, int b, int c)
{
    color = QColor(a % 255, b % 255, c % 255);
}

void Charge::setColor(QColor c)
{
    color = c;
}

void Charge::setX(double x1)
{
    x = x1;
}

void Charge::setY(double y1)
{
    y = y1;
}

void Charge::setM(double m1)
{
    m = m1;
}

void Charge::setQ(double q1)
{
    q = q1;
}

void Charge::operator=(Charge *q1)
{
    x = q1->getX();
    y = q1->getY();
    q1->getColor();
    q = q1->getQ();
    v = q1->getV();
    m = q1->getM();
    sign = q1->getSign();
}
