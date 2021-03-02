#include "game.h"
#include <QPalette>
#include <QPainter>
#include <iostream>
#include <QPainterPath>

game::game()
{
    setUp();
}

void game::setUp()
{
    this->resize(900, 900);
    QPalette p;
    p.setColor(QPalette::Background, Qt::white);
    this->setPalette(p);

    t = new QTimer(this);
    t->connect(t, &QTimer::timeout, this, &game::Timer);

    x = 200;
    y = 450;

    g = 0.005;
    a = 0;

    speed = 0.2;
    t->start(0);

    score = 0;

    hinter.resize(7);
    hinter[0] = std::make_pair(490, 450);
    hinter[1] = std::make_pair(710, 300);
    hinter[2] = std::make_pair(930, 300);
    hinter[3] = std::make_pair(1150, 300);
    hinter[4] = std::make_pair(1370, 450);
    hinter[5] = std::make_pair(1590, 260);
    hinter[6] = std::make_pair(1810, 500);
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
        i.first -= speed;
    }
}

void game::drawHinter()
{
    QPainter p(this);

    for(auto & i : hinter)
    {
        p.setBrush(Qt::black);
        p.drawRect(i.first, 0, 40, i.second);
        p.drawRect(i.first, i.second + 200, 40, this->height() - i.second);
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

    if(y + a + 40 < this->height() && y + a > 0) {y += a;}
}

void game::checkScore()
{
    if(hinter[score].first + 40 <= x) {score++;}
}

void game::Hinter()
{
    if(hinter[0].first + 40 < 0)
    {
        hinter.erase(hinter.begin());
        std::pair<int, int> f = *(--hinter.end());
        int sec = rand() % 600;
        if(sec < 200) {sec += 200;}
        else if(sec > 600) {sec -= 200;}
        hinter.push_back(std::make_pair(f.first + 220, sec));
    }
}

void game::checkDeath()
{
    for(auto & i : hinter)
    {
        if(i.first <= x + 40 && x <= i.first + 40)
        {
            if(i.second >= y || i.second + 200 <= y + 40)
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

    if(key == Qt::Key_Space) {a = -0.9;}
}


