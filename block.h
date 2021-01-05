#ifndef BLOCK_H
#define BLOCK_H

#include <QColor>

class Block
{
private:
    int x;
    int y;
    QColor color;

public:
    Block();
    Block(int x1, int y1, QColor c);

    virtual int getX();
    virtual int getY();
    virtual QColor getColor();
    virtual void setX(int x1);
    virtual void setY(int y1);
    virtual void setColor(QColor c);
};

#endif // BLOCK_H
