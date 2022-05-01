#ifndef COMPLEX_H
#define COMPLEX_H


class Complex
{
public:
    double x; double y;

    Complex();
    Complex(double x1, double y1);

    double modul();
    Complex operator+(Complex c);
    Complex operator*(Complex c);
    Complex operator*(double number);
    Complex operator/(double number);
};

#endif // COMPLEX_H
