#include "game.h"
#include <QDebug>
#include <QPainter>
#include <QTime>
#include <QMessageBox>
#include <QPushButton>

game::game()
{
    resize(Width * field_Width, Height * field_Height);

    this->setWindowTitle("Tetris");

    this->setMinimumSize(400, 1000);
    this->setMaximumSize(400, 1000);

    btnPause = new QPushButton(this);
    btnPause->setGeometry(360, 0, 40, 40);
    connect(btnPause, &QPushButton::clicked, this, &game::Pause);

    initGame();
}

void game::initGame()
{
    map.clear();
    map.resize(field_Width);
    for(int i = 0; i<map.size() ; ++i) map[i].resize(field_Height);
    for(int i = 0; i<map.size() ; ++i)
    {
        for(int j = 0; j<map[i].size() ; ++j)
        {
            map[i][j] = Qt::white;
        }
    }
    blocks.resize(4);
    play = true;
    timerId = startTimer(time);
    direction = 0;
    score = 0;
    setBlocks();
}

void game::Draw()
{
    QPainter p(this);
    if(play)
    {
        for(int i = 0; i<map.size() ; ++i)
        {
            for(int j = 0; j<map[i].size() ; ++j)
            {

                p.setBrush(map[i][j]);
                p.drawRect(i*Width, j*Height, Width, Height);
            }
        }
        p.setBrush(color);
        for(int i = 0; i<blocks.size() ; ++i)
        {
            p.drawRect(blocks[i].rx()*Width, blocks[i].ry()*Height, Width, Height);
        }
    }
    else
    {
        gameOver();
    }
}

void game::Move()
{
    if(checkField())
    {
        center.ry()++;
        for(int i = 0; i<blocks.size() ; ++i)
        {
            blocks[i].ry()++;
        }
    }
    else
    {
        checkLine();
        Stop();
        setBlocks();
    }

    if(checkSide())
    {
        if(direction == 1) {center.rx()++;}
        else if(direction == 2) {center.rx()--;}
        for(int i = 0; i<blocks.size() ; ++i)
        {
            if(direction == 1) {blocks[i].rx()++;}
            else if(direction == 2) {blocks[i].rx()--;}
        }
    }
    direction = 0;
}

void game::setBlocks()
{
    QTime time = QTime::currentTime();
    qsrand((uint) time.msec());
    form = (qrand() % 5) + 1;
    color = QColor(qrand() % 255, qrand() % 255, qrand() % 255);

    if(form == 1)
    {
        blocks[0].rx() = 0; blocks[0].ry() = 0;
        blocks[1].rx() = 0; blocks[1].ry() = 1;
        blocks[2].rx() = 0; blocks[2].ry() = 2;
        blocks[3].rx() = 1; blocks[3].ry() = 2;
        center.rx() = 1; center.ry() = 1;
    }
    else if(form == 2)
    {
        blocks[0].rx() = 0; blocks[0].ry() = 0;
        blocks[1].rx() = 0; blocks[1].ry() = 1;
        blocks[2].rx() = 1; blocks[2].ry() = 0;
        blocks[3].rx() = 1; blocks[3].ry() = 1;
    }
    else if(form == 3)
    {
        blocks[0].rx() = 0; blocks[0].ry() = 0;
        blocks[1].rx() = 0; blocks[1].ry() = 1;
        blocks[2].rx() = 0; blocks[2].ry() = 2;
        blocks[3].rx() = 0; blocks[3].ry() = 3;
        center = blocks[0];
    }
    else if(form == 4)
    {
        blocks[0].rx() = 0; blocks[0].ry() = 1;
        blocks[1].rx() = 1; blocks[1].ry() = 0;
        blocks[2].rx() = 1; blocks[2].ry() = 1;
        blocks[3].rx() = 2; blocks[3].ry() = 1;
        center.rx() = 1; center.ry() = 0;
    }
    else if(form == 5)
    {
        blocks[0].rx() = 0; blocks[0].ry() = 0;
        blocks[1].rx() = 1; blocks[1].ry() = 0;
        blocks[2].rx() = 1; blocks[2].ry() = 1;
        blocks[3].rx() = 2; blocks[3].ry() = 1;
        center.rx() = 1; center.ry() = 0;
    }
}

bool game::checkField()
{
    for(int i = 0; i<blocks.size() ; ++i)
    {
        if(blocks[i].y() + 1 == field_Height || map[blocks[i].x()][blocks[i].y() + 1] != Qt::white)
        {
            for(int j = 0; j<blocks.size() ; ++j)
            {
                map[blocks[j].x()][blocks[j].y()] = color;
            }
            return false;
        }
    }
    return true;
}

bool game::checkSide()
{
    if(direction == 1)
    {
        for(int i = 0; i<blocks.size() ; ++i)
        {
            if(blocks[i].x() + 1 == field_Width || map[blocks[i].x() + 1][blocks[i].y()] != Qt::white)
            {
                return false;
            }
        }
        return true;
    }
    else if(direction == 2)
    {
        for(int i = 0; i<blocks.size() ; ++i)
        {
            if(blocks[i].x() == 0 || map[blocks[i].x() - 1][blocks[i].y()] != Qt::white)
            {
                return false;
            }
        }
        return true;
    }
    else if(direction == 3)
    {
        Turn();
        return true;
    }
}

void game::checkLine()
{
    for(int y = 0; y<field_Height ; ++y)
    {
        int count = 0;
        for(int x = 0; x<field_Width ; ++x)
        {
           if(map[x][y] == Qt::white) {count++;}
        }
        if(!count)
        {
            for(int x = 0; x<field_Width ; ++x)
            {
                map[x][y] = Qt::white;
            }
            score++;
            pushDown(y - 1);
        }
    }
}

void game::gameOver()
{
    play = false;
    QPushButton *btn = new QPushButton("Game Over, your score: " + QString::number(score), this);
    btn->showMaximized();
    btn->resize(200, 200);
    killTimer(timerId);
    initGame();
}

void game::pushDown(int row)
{
    for(int y = row; y > 0 ; --y)
    {
        for(int x = 0; x < field_Width ; ++x)
        {
            map[x][y + 1] = map[x][y];
        }
    }
}

void game::DrawLine()
{
    QPainter p(this);
    p.setBrush(Qt::white);
    p.drawRect(0, 0, 400, 1000);
}

void game::Pause()
{
    play = false;
}

void game::Stop()
{
    for(int i = 0; i<field_Width ; ++i)
    {
        if(map[i][3] != Qt::white)
        {
            gameOver();
        }
    }
}

void game::Turn()
{
    if(form != 2)
    {
        QVector<QPoint> n = blocks;
        for(int i = 0; i<n.size() ; ++i)
        {
            n[i].rx() -= center.x();
            n[i].ry() -= center.y();
            n[i].ry() *= -1;
        }
        if(form != 3)
        {
            for(int i = 0; i<n.size() ; ++i)
            {
                int tmp = n[i].x();
                n[i].rx() = n[i].y();
                n[i].ry() = -1*tmp;
            }
        }
        else
        {
            for(int i = 0; i<n.size() ; ++i)
            {
                int tmp = n[i].x();
                n[i].rx() = -1*n[i].y();
                n[i].ry() = tmp;
            }
        }
        for(int i = 0; i<n.size() ; ++i)
        {
            n[i].rx() += center.rx();
            n[i].ry() = center.ry() - n[i].ry();
        }
        bool c = true;
        for(int i = 0; i<n.size() ; ++i)
        {
            if(n[i].x() < 0 || n[i].x() >= field_Width) {c = false; break;}
            else if(n[i].y() >= field_Height) {c = false; break;}
            else if(map[n[i].x()][n[i].y()] != Qt::white) {c = false; break;}
        }
        if(c)
        {
            blocks = n;
            if(form == 3)
            {
                if(center == blocks[3]) {center = blocks[0];}
                else {center = blocks[3];}
            }
        }
    }
}

void game::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);
    this->repaint();
    if(play)
    {
        Move();
        Stop();
    }
}

void game::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if(play) {Draw();}
    else {DrawLine();}
}

void game::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if(key == Qt::Key_Right) {direction = 1;}
    else if(key == Qt::Key_Left) {direction = 2;}
    else if(key == Qt::Key_Up && form != 2) {direction = 3;}
}
