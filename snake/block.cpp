#include "block.h"
#include <QColor>
#include <random>
#include <ctime>

Block::Block()
{
    x = y = 0;
    color = Qt::green;
}

Block::Block(int x1, int y1, QColor c)
{
    x = x1;
    y = y1;
    color = c;
}

int Block::getX()
{
    return x;
}

int Block::getY()
{
    return y;
}

QColor Block::getColor()
{
    return color;
}

void Block::setX(int x1)
{
    x = x1;
}

void Block::setY(int y1)
{
    y = y1;
}

void Block::setColor(QColor c)
{
    color = c;
}
