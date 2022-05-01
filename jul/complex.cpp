#include "complex.h"
#include <cmath>

Complex::Complex()
{
    x = y = 0;
}

Complex::Complex(double x1, double y1)
{
    x = x1; y = y1;
}

double Complex::modul()
{
    return sqrt(x * x + y * y);
}

Complex Complex::operator+(Complex c)
{
    return Complex(x + c.x, y + c.y);
}

Complex Complex::operator*(Complex c)
{
    return Complex(x * c.x - y * c.y, y * c.x + x * c.y);
}

Complex Complex::operator*(double number)
{
    return Complex(x * number, y * number);
}

Complex Complex::operator/(double number)
{
    return Complex(x / number, y / number);
}
