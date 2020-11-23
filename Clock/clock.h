#ifndef CLOCK_H
#define CLOCK_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QPushButton>

class Clock : public QWidget
{
private:
    double cx;
    double cy;
    double r;
    double rclock;
    double fs;
    double fmin;
    double fh;
    double sec;
    int min;
    int h;
    bool m;
    bool ho;
    QTimer *timer;
    QPushButton *btn;

public:
    Clock();

    void Timer();
    void DrawSec();
    void DrawMin();
    void DrawH();
    void DrawCircle();
    void setButton();
    void Start();
    void setSystemTime();
    void DrawLines();
    void upd();

protected:
    void paintEvent(QPaintEvent *e) override;
};

#endif // CLOCK_H
