#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QTimer>
#include <block.h>
#include <QMouseEvent>
#include <QPaintEvent>

class game : public QWidget
{
private:
    QPair<double, double> delta;

    QTimer *t;
    QVector<Block*> snake;

public:
    game();

    void setUp();
    void Timer();
    void Move();
    void Direction();
    double getDistance(double x1, double y1, double x2, double y2);

protected:
    void mouseMoveEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *e) override;
};

#endif // GAME_H
