#ifndef FOOD_H
#define FOOD_H

#include <QColor>
#include <QPixmap>

class Food
{
private:
    int x;
    int y;
    QColor color;

public:
    Food();
    Food(double Width, double Height);

    QPixmap sprite;

    virtual int getX();
    virtual int getY();
    virtual void setX(int x1);
    virtual void setY(int y1);
};

#endif // FOOD_H
