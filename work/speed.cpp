#include "speed.h"
#include <cmath>

Speed::Speed()
{
    x = 0;
    y = 0;
    modulSpeed = 0;
}

Speed::Speed(double x1, double y1)
{
    x = x1;
    y = y1;
    countSpeed();
}

double Speed::getX()
{
    return x;
}

double Speed::getY()
{
    return y;
}

void Speed::setX(double x1)
{
    x = x1;
    countSpeed();
}

void Speed::setY(double y1)
{
    y = y1;
    countSpeed();
}

void Speed::countSpeed()
{
    modulSpeed = sqrt(pow(x, 2) + pow(y, 2));
}

double Speed::getModulSpeed()
{
    return modulSpeed;
}

void Speed::operator=(Speed v1)
{
    x = v1.getX();
    y = v1.getY();
    countSpeed();
}


