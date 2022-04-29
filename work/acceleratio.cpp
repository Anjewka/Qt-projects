#include "acceleratio.h"

acceleratio::acceleratio()
{
    x = 0;
    y = 0;
}

acceleratio::acceleratio(double x1, double y1)
{
    x = x1;
    y = y1;
}

double acceleratio::getX()
{
    return x;
}

double acceleratio::getY()
{
    return y;
}

void acceleratio::setX(double x1)
{
    x = x1;
}

void acceleratio::setY(double y1)
{
    y = y1;
}

void acceleratio::operator=(acceleratio a1)
{
    x = a1.getX();
    y = a1.getY();
}
