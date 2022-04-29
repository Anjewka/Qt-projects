#ifndef CHARGE_H
#define CHARGE_H

#include <iostream>
#include <speed.h>
#include <QString>
#include <QColor>
#include <acceleratio.h>

class Charge
{
private:
    Speed *v;
    acceleratio *a;
    QColor color;
    QString sign;
    double x;
    double y;
    double q;
    double m;

public:
    Charge();
    Charge(Speed *v1, double x1, double y1, double m1, double q1, QString s, QColor c);

    virtual Speed *getV();
    virtual double getVx();
    virtual double getVy();
    virtual acceleratio *getA();
    virtual QString getSign();
    virtual QColor getColor();
    virtual double getX();
    virtual double getY();
    virtual double getM();
    virtual double getQ();
    virtual void setSpeed(Speed *v1);
    virtual void setSpeed(double x1, double y1);
    virtual void setA(acceleratio *a1);
    virtual void setA(double x1, double y1);
    virtual void setSign(QString s);
    virtual void setColor(QColor c);
    virtual void setColor(int a, int b, int c);
    virtual void setX(double x1);
    virtual void setY(double y1);
    virtual void setM(double m1);
    virtual void setQ(double q1);
};

#endif // CHARGE_H
