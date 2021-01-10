#include "game.h"

#include <QPixmap>
#include <QPainter>
#include <QPalette>
#include <QPair>

game::game()
{
    doodle = new QPixmap(".//images/texture/soccer-left-odskok@2x.png");
    field = new QPixmap(".//images/texture/backgrond-soccer.png");
    texture = new QPixmap(".//images/texture/game-tiles-soccer@2x.png");

    t1 = new QTimer(this);

    std::srand(static_cast<int>(std::time(0)));

    lands.resize(rand() % 10);

    for(int i = 0 ; i<lands.size() ; ++i)
    {
        lands[i] = qMakePair(rand() % 600, rand() % 1000);
    }

    this->resize(640, 1024);

    lands[0].first = 300;
    lands[0].second = 700;

    doodle_x = lands[0].first + 10;
    doodle_y = lands[0].second - 105;

    top = doodle_y - 250;

    vy = -0.7;
    vx = 0;

    play = true;

    score = 0;

    connect(t1, &QTimer::timeout, this, &game::timer);
    t1->start(1);
}

void game::timer()
{
    this->repaint();
}

void game::DrawMap()
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, *field, 0, 0, 640, 1024);
}

void game::DrawLands()
{
    QPainter painter(this);

    for(int i = 0; i<lands.size() ; ++i)
    {
        painter.drawPixmap(lands[i].first, lands[i].second - score, *texture, 0, 0, 115, 40);
    }
}

void game::DrawDoodle()
{
    QPainter painter(this);
    painter.drawPixmap(doodle_x, doodle_y - score, *doodle, 0, 10, 96, 105);
}

void game::checkLand()
{
    for(int i = 0; i<lands.size() ; ++i)
    {
        if(doodle_y + vy + 105 >= lands[i].second && doodle_y + vy + 105 <= lands[i].second + 40)
        {
            if(doodle_x + 96 >= lands[i].first && doodle_x + 96 <= lands[i].first + 115)
            {
                if(vy > 0)
                {
                    doodle_y = lands[i].second - 105;
                    vy *= -1;
                    if(score + 700 > doodle_y) {score = doodle_y - 700;}
                    top = doodle_y - 250 - score;
                }
            }
        }
    }
}

void game::swapDoodleTexture()
{
//    doodle = new QPixmap(".//images/texture/soccer-right-odskok@2x.png");
}

void game::checkGame()
{
    if(this->height() <= doodle_y) {play = false;}
}

void game::checkTop()
{
    if(top >= doodle_y + vy) {vy *= -1;}
}

void game::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if(key == Qt::Key_Right) {doodle_x += 3; if(doodle_x > this->width()) {doodle_x -= this->width();} swapDoodleTexture();}
    else if(key == Qt::Key_Left) {doodle_x -= 3; if(doodle_x < 0) {doodle_x += this->width();} swapDoodleTexture();}
}

void game::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    if(play)
    {
        doodle_y += vy;
        doodle_x += vx;
        vx = 0;
        checkTop();
        checkLand();
        checkGame();
    }

    DrawMap();
    DrawDoodle();
    DrawLands();
}
