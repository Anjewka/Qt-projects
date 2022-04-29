#ifndef ACCELERATIO_H
#define ACCELERATIO_H


class acceleratio
{
private:
    double x;
    double y;

public:
    acceleratio();
    acceleratio(double x1, double y1);

    virtual double getX();
    virtual double getY();
    virtual void setX(double x1);
    virtual void setY(double y1);

    void operator=(acceleratio a1);
};

#endif // ACCELERATIO_H
