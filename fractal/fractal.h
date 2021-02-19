#ifndef FRACTAL_H
#define FRACTAL_H

#include <QWidget>
#include <QTimer>
#include <QPaintEvent>
#include <complex.h>

class fractal : public QWidget
{
private:
    QTimer *t;
    double size;
    double cx; double cy;
    Complex c;

public:
    fractal();

    void Timer();
    void Draw();

protected:
    void paintEvent(QPaintEvent *e) override;
};

#endif // FRACTAL_H
