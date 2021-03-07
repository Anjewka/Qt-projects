#ifndef FRACTAL_H
#define FRACTAL_H

#include <QWidget>
#include <QTimer>
#include <QPaintEvent>
#include <complex.h>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QTimerEvent>
#include <QKeyEvent>
#include <QLinearGradient>
#include <QMouseEvent>

class fractal : public QWidget
{
private:
    QTimer *t;
    double size;
    double Re, Im;
    double x0, y0;
    double mouse_x, mouse_y;
    double max_iterations;
    Complex c;

    bool menu;

    enum Color
    {
        colorfull,
        black_white,
        smooth
    };

    Color color;

    bool stop;

    QPixmap *background;

    QPushButton *btn;

    std::list<QLineEdit*> *lineEdit_list;
    std::list<QLabel*> *label_list;
    std::list<QRadioButton*> *radio_list;

    QString string;
    int scope_time;

public:
    fractal();

    void Timer();
    void DrawFractal();
    void DrawMenu();
    void SetMenu();
    void setUp();
    void Start();
    bool TextChanged();
    void SwapRadioButton();

protected:
    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
};

#endif // FRACTAL_H
