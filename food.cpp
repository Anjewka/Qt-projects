#include "food.h"
#include <random>
#include <ctime>

Food::Food()
{
    x = y = 0;
    color = Qt::red;
}

Food::Food(double Width, double Height)
{
    std::srand(static_cast<int>(std::time(0)));
    x = rand() % int(Width);
    y = rand() % int(Height);
    color = QColor(rand() % 255, rand() % 255, rand() % 255);
    while(color == Qt::white) {color = QColor(rand() % 255, rand() % 255, rand() % 255);}
}

int Food::getX()
{
    return x;
}

int Food::getY()
{
    return y;
}

QColor Food::getColor()
{
    return color;
}

void Food::setX(int x1)
{
    x = x1;
}

void Food::setY(int y1)
{
    y = y1;
}

void Food::setColor(QColor c)
{
    color = c;
}
