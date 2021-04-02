#include "block.h"
#include <QColor>
#include <random>
#include <ctime>

Block::Block()
{
    x = y = 0;
    r = 30;
}

Block::Block(double x1, double y1, double r1)
{
    x = x1;
    y = y1;
    r = r1;
}

double Block::getX()
{
    return x;
}

double Block::getY()
{
    return y;
}

void Block::setX(double x1)
{
    x = x1;
}

void Block::setY(double y1)
{
    y = y1;
}
