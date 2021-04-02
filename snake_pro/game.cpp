#include "game.h"
#include <QPainter>
#include <cmath>
#include <iostream>

game::game()
{
    this->resize(900, 900);
    QPalette p;
    p.setColor(QPalette::Background, Qt::white);
    this->setPalette(p);

    setUp();
}

void game::setUp()
{
    snake.resize(3);
    for(int i = 0; i<snake.size() ; ++i)
    {
        snake[i] =  new Block();
        snake[i]->setX(width() / 2 - i);
        snake[i]->setY(height() / 2);
    }

    delta = qMakePair(150, 0);

    t = new QTimer(this);
    t->connect(t, &QTimer::timeout, this, &game::Timer);
    t->start(500);
}

void game::Timer()
{
    this->repaint();
    Direction();
    Move();
}

void game::Move()
{
    for(int i = snake.size() - 1; i > 0 ; --i)
    {
        snake[i]->setX(snake[i - 1]->getX());
        snake[i]->setY(snake[i - 1]->getY());
    }

    snake[0]->setX(snake[0]->getX() + delta.first * snake[0]->r);
    snake[0]->setY(snake[0]->getY() + delta.second * snake[0]->r);
}

void game::Direction()
{
    delta.first = (delta.first) / getDistance(delta.first, delta.second, 0, 0);
    delta.second = (delta.second) / getDistance(delta.first, delta.second, 0, 0);
}

double game::getDistance(double x1, double y1, double x2, double y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void game::mouseMoveEvent(QMouseEvent *e)
{
    delta.first = e->pos().rx() - this->width() / 2;
    delta.second = e->pos().ry() - this->height() / 2;
}

void game::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter p(this);

    for(int i = 0; i<snake.size() ; ++i)
    {
        p.setBrush(Qt::green);
        p.drawEllipse(snake[i]->getX(), snake[i]->getY(), snake[i]->r, snake[i]->r);
    }
}
