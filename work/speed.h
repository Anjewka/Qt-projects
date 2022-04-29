#ifndef SPEED_H
#define SPEED_H


class Speed
{
private:
    double x;
    double y;
    double modulSpeed;

public:
    Speed();
    Speed(double x1, double y1);

    virtual double getX();
    virtual double getY();
    virtual double getModulSpeed();
    virtual void setX(double x1);
    virtual void setY(double y1);
    virtual void countSpeed();

    void operator=(Speed v1);
};

#endif // SPEED_H
