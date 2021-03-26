#ifndef BLOCK_H
#define BLOCK_H

class Block
{
private:
    int x;
    int y;

public:
    Block();
    Block(int x1, int y1);

    virtual int getX();
    virtual int getY();
    virtual void setX(int x1);
    virtual void setY(int y1);
};

#endif // BLOCK_H
