#include "food.h"
#include <random>
#include <ctime>

Food::Food()
{
    x = y = 0;
    sprite.load("sprites/snake.png");
}

Food::Food(double Width, double Height)
{
    std::srand(static_cast<int>(std::time(0)));
    x = rand() % int(Width);
    y = rand() % int(Height);
    sprite.load("sprites/snake.png");
}

int Food::getX()
{
    return x;
}

int Food::getY()
{
    return y;
}

void Food::setX(int x1)
{
    x = x1;
}

void Food::setY(int y1)
{
    y = y1;
}
