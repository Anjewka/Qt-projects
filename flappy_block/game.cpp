#include "game.h"
#include <QPalette>
#include <QPainter>
#include <iostream>
#include <QPainterPath>

game::game()
{
    setUp();
    t = new QTimer(this);
    t->connect(t, &QTimer::timeout, this, &game::Timer);
    t->start(10);
}

void game::setUp()
{
    this->resize(900, 900);

    x = 200;
    y = 450;

    g = 0.3;
    a = 0;

    speed = 1.3;

    score = 0;

    hinter.resize(7);
    hinter[0] = std::make_pair(std::make_pair(490, 450), true);
    hinter[1] = std::make_pair(std::make_pair(710, 300), true);
    hinter[2] = std::make_pair(std::make_pair(930, 300), true);
    hinter[3] = std::make_pair(std::make_pair(1150, 300), true);
    hinter[4] = std::make_pair(std::make_pair(1370, 450), true);
    hinter[5] = std::make_pair(std::make_pair(1590, 260), true);
    hinter[6] = std::make_pair(std::make_pair(1810, 500), true);
}

void game::Timer()
{
    this->repaint();

    updateScene();
    Gravity();
    Hinter();
    checkDeath();
    checkScore();
}

void game::Draw()
{
    QPainter p(this);
    p.setBrush(Qt::black);
    p.drawRect(x, y, 40, 40);
}

void game::updateScene()
{
    for(auto & i : hinter)
    {
        i.first.first -= speed;
    }
}

void game::drawHinter()
{
    QPainter p(this);

    for(auto & i : hinter)
    {
        p.setBrush(Qt::black);
        p.drawRect(i.first.first, 0, 40, i.first.second);
        p.drawRect(i.first.first, i.first.second + 200, 40, this->height() - i.second);
    }
}

void game::drawScore()
{
    QPainter p(this);
    QPainterPath pa;

    p.setRenderHint(QPainter::Antialiasing);

    QFont font("Bad script", 50, Qt::black);
    pa.addText(QPointF(0.45 * this->width(), 0.1 * this->height()), font, QString::number(score));
    p.setBrush(Qt::black);
    p.drawPath(pa);
}

void game::Gravity()
{
    if(a != 0) {a += g;}
    else {a = g;}

    y += a;
}

void game::checkScore()
{
    for(int i = 0; i<int(hinter.size()) ; ++i)
    {
        if(hinter[i].second && hinter[i].first.first < 200)
        {
            score++;
            hinter[i].second = false;
        }
    }
}

void game::Hinter()
{
    if(hinter[0].first.first + 40 < 0)
    {
        hinter.erase(hinter.begin());
        std::pair<int, int> f = (*(--hinter.end())).first;
        int sec = rand() % 600;
        if(sec < 200) {sec += 200;}
        else if(sec > 600) {sec -= 200;}
        hinter.push_back(std::make_pair(std::make_pair(f.first + 220, sec), true));
    }
}

void game::checkDeath()
{
    for(auto & i : hinter)
    {
        if(i.first.first <= x + 40 && x <= i.first.first + 40)
        {
            if(i.first.second >= y || i.first.second + 200 <= y + 40)
            {
                setUp();
            }
        }
    }
}

void game::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    Draw();
    drawHinter();
    drawScore();
}

void game::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();

    if(key == Qt::Key_Space) {a = -7.2;}
}
