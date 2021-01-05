#ifndef POSITION_H
#define POSITION_H


class position
{
private:
    int x;
    int y;

public:
    position();
    position(int x1, int y1);

    virtual int getX();
    virtual int getY();
    virtual void setX(int x1);
    virtual void setY(int y1);
};

#endif // POSITION_H
