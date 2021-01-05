#include "figure.h"
#include <ctime>
#include <random>

figure::figure()
{
    blocks.resize(4);

    std::srand(static_cast<int>(std::time(0)));

    center = new position();

    form = (rand() % 6) + 1;
    color = QColor(rand() % 255, rand() % 255, rand() % 255);
    setShape();
}

int figure::getForm()
{
    return form;
}

QVector<position> figure::getBlocks()
{
    return blocks;
}

position* figure::getC()
{
    return center;
}

void figure::setCx(int x1)
{
    center->setX(x1);
}

void figure::setCy(int y1)
{
    center->setY(y1);
}

QColor figure::getColor()
{
    return color;
}

void figure::setShape()
{
    if(form == 1)
    {
        blocks[0].setX(0); blocks[0].setY(0);
        blocks[1].setX(0); blocks[1].setY(1);
        blocks[2].setX(0); blocks[2].setY(2);
        blocks[3].setX(1); blocks[3].setY(2);
        getC()->setX(1); getC()->setY(1);
    }
    else if(form == 2)
    {
        blocks[0].setX(0); blocks[0].setY(0);
        blocks[1].setX(0); blocks[1].setY(1);
        blocks[2].setX(1); blocks[2].setY(0);
        blocks[3].setX(1); blocks[3].setY(1);
    }
    else if(form == 3)
    {
        blocks[0].setX(0); blocks[0].setY(0);
        blocks[1].setX(0); blocks[1].setY(1);
        blocks[2].setX(0); blocks[2].setY(2);
        blocks[3].setX(0); blocks[3].setY(3);
        getC()->setX(0); getC()->setY(0);
    }
    else if(form == 4)
    {
        blocks[0].setX(0); blocks[0].setY(1);
        blocks[1].setX(1); blocks[1].setY(0);
        blocks[2].setX(1); blocks[2].setY(1);
        blocks[3].setX(2); blocks[3].setY(1);
        getC()->setX(1); getC()->setY(0);
    }
    else if(form == 5)
    {
        blocks[0].setX(0); blocks[0].setY(0);
        blocks[1].setX(1); blocks[1].setY(0);
        blocks[2].setX(1); blocks[2].setY(1);
        blocks[3].setX(2); blocks[3].setY(1);
        getC()->setX(1); getC()->setY(0);
    }
    else if(form == 6)
    {
        blocks[0].setX(1); blocks[0].setY(0);
        blocks[1].setX(1); blocks[1].setY(1);
        blocks[2].setX(1); blocks[2].setY(2);
        blocks[3].setX(0); blocks[3].setY(2);
        getC()->setX(0); getC()->setY(1);
    }
    else if(form == 7)
    {
        blocks[0].setX(1); blocks[0].setY(0);
        blocks[1].setX(2); blocks[1].setY(0);
        blocks[2].setX(1); blocks[2].setY(1);
        blocks[3].setX(0); blocks[3].setY(1);
        getC()->setX(1); getC()->setY(0);
    }
}


void figure::setBlocks(QVector<position> b)
{
    blocks = b;
}

void figure::setC(position c1)
{
    center->setX(c1.getX());
    center->setY(c1.getY());
}

void figure::setColor(QColor c)
{
    color = c;
}
