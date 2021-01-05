#ifndef FOOD_H
#define FOOD_H

#include <QColor>

class Food
{
private:
    int x;
    int y;
    QColor color;

public:
    Food();
    Food(double Width, double Height);

    virtual int getX();
    virtual int getY();
    virtual QColor getColor();
    virtual void setX(int x1);
    virtual void setY(int y1);
    virtual void setColor(QColor c);
};

#endif // FOOD_H
