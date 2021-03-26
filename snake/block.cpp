#include "block.h"
#include <random>
#include <ctime>

Block::Block()
{
    x = y = 0;
}

Block::Block(int x1, int y1)
{
    x = x1;
    y = y1;
}

int Block::getX()
{
    return x;
}

int Block::getY()
{
    return y;
}

void Block::setX(int x1)
{
    x = x1;
}

void Block::setY(int y1)
{
    y = y1;
}
