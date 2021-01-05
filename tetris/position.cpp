#include "position.h"

position::position()
{
    x = y = 0;
}

position::position(int x1, int y1)
{
    x = x1;
    y = y1;
}

int position::getX()
{
    return x;
}

int position::getY()
{
    return y;
}

void position::setX(int x1)
{
    x = x1;
}

void position::setY(int y1)
{
    y = y1;
}
