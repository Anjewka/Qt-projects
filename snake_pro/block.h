#ifndef BLOCK_H
#define BLOCK_H

#include <QColor>
#include <QPixmap>
#include <QQueue>
#include <QPair>

class Block
{
private:
    double x;
    double y;

public:
    Block();
    Block(double x1, double y1, double r);

    double r;

    virtual double getX();
    virtual double getY();
    virtual void setX(double x1);
    virtual void setY(double y1);
};

#endif // BLOCK_H
