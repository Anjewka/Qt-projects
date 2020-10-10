#include "game.h"
#include <QDebug>
#include <QPainter>
#include <QTime>
#include <QMessageBox>

Game::Game()
{
    resize(Width * field_Width, Height * field_Height);

    this->setWindowTitle("snake");

    initGame();
}

void Game::initGame()
{
    play = true;
    dir = right;
    snake.resize(3);
    for(int i = 0; i<snake.size() ; ++i) {snake[i].rx() = snake.size() - i + 1; snake[i].ry() = 1;}
    timerId = startTimer(time);
    Fruit();
}

void Game::Draw()
{
    QPainter qp(this);
    if(play)
    {
        qp.setBrush(Qt::green);
        qp.drawEllipse(f.x() * Width, f.y() * Height, Width, Height);
        qp.setBrush(Qt::white);
        for(int i = 0; i<snake.size() ; ++i) qp.drawEllipse(snake[i].x() * Width, snake[i].y() * Height, Width, Height);
    }
    else
    {
        gameOver();
    }
}

void Game::Fruit()
{
    QTime time = QTime::currentTime();
    qsrand((uint) time.msec());
    f.rx() = qrand() % Width; f.ry() = qrand() % Height;
}

void Game::Move()
{
    for(int i = snake.size() - 1; i > 0 ; --i)
    {
        snake[i] = snake[i-1];
    }

    Check();

    switch(dir)
    {
    case left: {snake[0].rx()--; break;}
    case right: {snake[0].rx()++; break;}
    case up: {snake[0].ry()--; break;}
    case down: {snake[0].ry()++; break;}
    }
}

bool Game::Check()
{
    if(snake[0].x() >= 0 && snake[0].x() < field_Width)
    {
        if(snake[0].y() >= 0 && snake[0].y() < field_Height)
        {
            return true;
        }
    }
    return false;
}

void Game::gameOver()
{
    play = false;
    killTimer(timerId);
    initGame();
}

void Game::eat()
{
    if(snake[0] == f)
    {
        snake.push_back(QPoint(0, 0));
        Fruit();
    }
}

void Game::checkSnake()
{
    if(snake.size() > 3)
    {
        int pos = 0;
        for(int i = 1; i<snake.size() ; ++i)
        {
            if(snake[0] == snake[i])
            {
                pos = i;
                break;
            }
        }
        while(pos--) snake.pop_back();
    }
}

void Game::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);

    if(play && Check())
    {
        Move();
        eat();
        checkSnake();
        this->repaint();
    }
    else gameOver();
}

void Game::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    if(key == Qt::Key_Right && dir != left) {dir = right;}
    else if(key == Qt::Key_Left && dir != right) {dir = left;}
    else if(key == Qt::Key_Up && dir != down) {dir = up;}
    else if(key == Qt::Key_Down && dir != up) {dir = down;}
}

void Game::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    Draw();
}
