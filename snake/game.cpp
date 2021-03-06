#include "game.h"
#include <QDebug>
#include <QPainter>
#include <QTime>
#include <QPainterPath>
#include <QMessageBox>
#include <ctime>
#include <random>
#include <QPushButton>
#include <QApplication>
#include <QLabel>
#include <QGridLayout>
#include <fstream>

Game::Game()
{
    resize(Width * field_Width, Height * field_Height);

    this->setMinimumSize(400, 400);

    QPalette p;
    p.setColor(QPalette::Background, Qt::white);
    this->setPalette(p);

    this->setWindowTitle("snake");

    bt1 = new QPushButton(this);
    bt2 = new QPushButton(this);

    bt1->setEnabled(false);
    bt1->setVisible(false);
    bt2->setEnabled(false);
    bt2->setVisible(false);

    connect(bt2, &QPushButton::clicked, this, &QApplication::quit);
    connect(bt1, &QPushButton::clicked, this, &Game::restartGame);

    t1 = new QTimer(this);
    t2 = new QTimer(this);

    connect(t1, &QTimer::timeout, this, &Game::reDraw);
    connect(t2, &QTimer::timeout, this, &Game::timeEvent);

    t1->start(0);
    t2->start(100);

    initGame();
}

void Game::initGame()
{
    play = true;
    menu = false;

    direction = 0;
    snake.resize(3);

    snake_sprite.load("sprites/snake.png");

    for(int i = 0; i<snake.size() ; ++i)
    {
        snake[i] = new Block();
        snake[i]->setX(field_Width / 2 - i);
        snake[i]->setY(field_Height / 2);
    }

    fruit = new Food(field_Width, field_Height);
    score = 0;
}

void Game::Draw()
{
    QPainter p(this);
    p.setBrush(QColor(226, 233, 127));
    p.drawRect(0, 0, this->width(), this->height());
    p.setRenderHint(QPainter::Antialiasing);
    if(play)
    {
        int posx = 120; int posy = 0;
        snake_sprite = snake_sprite.scaled(5 * Width, 4 * Height);
        if(snake[0]->getX() == snake[1]->getX()) {if(snake[0]->getY() < snake[1]->getY()) {posx = 90; posy = 0;}}
        if(snake[0]->getX() < snake[1]->getX()) {if(snake[0]->getY() == snake[1]->getY()) {posx = 90; posy = 30;}}
        if(snake[0]->getX() == snake[1]->getX()) {if(snake[0]->getY() > snake[1]->getY()) {posx = 120; posy = 30;}}
        p.drawPixmap(snake[0]->getX() * field_Width, snake[0]->getY() * field_Height, Width, Height, snake_sprite, posx, posy, Width, Height);
        for(int i = 1; i<snake.size() - 1 ; ++i)
        {
            posx = 30; posy = 0;
            if(snake[i]->getY() != snake[i - 1]->getY()) {posx = 60; posy = 30;}
            if(snake[i]->getX() < snake[i - 1]->getX() && snake[i]->getY() == snake[i - 1]->getY()) {if(snake[i]->getX() == snake[i + 1]->getX() && snake[i]->getY() < snake[i + 1]->getY()) {posx = 0; posy = 0;}}
            if(snake[i]->getX() < snake[i + 1]->getX() && snake[i]->getY() == snake[i + 1]->getY()) {if(snake[i]->getX() == snake[i - 1]->getX() && snake[i]->getY() < snake[i - 1]->getY()) {posx = 0; posy = 0;}}
            if(snake[i]->getX() < snake[i - 1]->getX() && snake[i]->getY() == snake[i - 1]->getY()) {if(snake[i]->getX() == snake[i + 1]->getX() && snake[i]->getY() > snake[i + 1]->getY()) {posx = 0; posy = 30;}}
            if(snake[i]->getX() < snake[i + 1]->getX() && snake[i]->getY() == snake[i + 1]->getY()) {if(snake[i]->getX() == snake[i - 1]->getX() && snake[i]->getY() > snake[i - 1]->getY()) {posx = 0; posy = 30;}}
            if(snake[i]->getX() == snake[i - 1]->getX() && snake[i]->getY() < snake[i - 1]->getY()) {if(snake[i]->getX() > snake[i + 1]->getX() && snake[i]->getY() == snake[i + 1]->getY()) {posx = 60; posy = 0;}}
            if(snake[i]->getX() == snake[i + 1]->getX() && snake[i]->getY() < snake[i + 1]->getY()) {if(snake[i]->getX() > snake[i - 1]->getX() && snake[i]->getY() == snake[i - 1]->getY()) {posx = 60; posy = 0;}}
            if(snake[i]->getX() > snake[i - 1]->getX() && snake[i]->getY() == snake[i - 1]->getY()) {if(snake[i]->getX() == snake[i + 1]->getX() && snake[i]->getY() > snake[i + 1]->getY()) {posx = 60; posy = 60;}}
            if(snake[i]->getX() > snake[i + 1]->getX() && snake[i]->getY() == snake[i + 1]->getY()) {if(snake[i]->getX() == snake[i - 1]->getX() && snake[i]->getY() > snake[i - 1]->getY()) {posx = 60; posy = 60;}}
            p.drawPixmap(snake[i]->getX() * field_Width, snake[i]->getY() * field_Height, Width, Height, snake_sprite, posx, posy, Width, Height);
        }
        posx = 90; posy = 90; int end = snake.size() - 1;
        if(snake[end]->getX() == snake[end - 1]->getX()) {if(snake[end]->getY() < snake[end - 1]->getY()) {posx = 120; posy = 90;}}
        if(snake[end]->getX() < snake[end - 1]->getX()) {if(snake[end]->getY() == snake[end - 1]->getY()) {posx = 120; posy = 60;}}
        if(snake[end]->getX() == snake[end - 1]->getX()) {if(snake[end]->getY() > snake[end - 1]->getY()) {posx = 90; posy = 60;}}
        p.drawPixmap(snake[end]->getX() * field_Width, snake[end]->getY() * field_Height, Width, Height, snake_sprite, posx, posy, Width, Height);

        fruit->sprite = fruit->sprite.scaled(5 * Width, 4 * Height);
        p.drawPixmap(fruit->getX() * field_Width, fruit->getY() * field_Height, Width, Height, fruit->sprite, 0, 90, Width, Height);
    }
    else
    {
        gameOver();
    }
}

void Game::Move()
{
    for(int i = snake.size() - 1; i > 0 ; --i)
    {
        snake[i]->setX(snake[i - 1]->getX());
        snake[i]->setY(snake[i - 1]->getY());
    }

    switch(direction)
    {
    case 0: {snake[0]->setX(snake[0]->getX() + 1); break;}
    case 1: {snake[0]->setX(snake[0]->getX() - 1); break;}
    case 2: {snake[0]->setY(snake[0]->getY() - 1); break;}
    case 3: {snake[0]->setY(snake[0]->getY() + 1); break;}
    }
}

void Game::Check()
{
    if(snake[0]->getX() < 0 || snake[0]->getX() >= field_Width || snake[0]->getY() < 0 || snake[0]->getY() >= field_Height)
    {
        gameOver();
    }
}

void Game::gameOver()
{
    play = false;
    menu = true;

    bt1->setVisible(true);
    bt1->setEnabled(true);
    bt2->setVisible(true);
    bt2->setEnabled(true);
}

void Game::drawMenu()
{
    bt1->setGeometry(0.3 * this->width(), 0.5 * this->height(), 0.2 * this->width(), 0.1 * this->height());
    bt2->setGeometry(0.5 * this->width(), 0.5 * this->height(), 0.2 * this->width(), 0.1 * this->height());

    bt1->setText("Restart");
    bt2->setText("Quit");

    QPainter p(this);
    QPainterPath pa;

    p.setRenderHint(QPainter::Antialiasing);

    QFont font;
    font.setPointSize(30);
    pa.addText(QPointF(0.3 * this->width(), 0.45 * this->height()), font, "Your score: " + QString::number(score));
    p.setBrush(Qt::black);
    p.drawPath(pa);

    std::ifstream input;
    int record;
    input.open("records.txt");
    if(input.is_open()) {input >> record;}
    else record = 0;
    input.close();

    if(score > record) {record = score;}

    font.setPointSize(30);
    pa.addText(QPointF(0.3 * this->width(), 0.3 * this->height()), font, "Your record: " + QString::number(record));
    p.setBrush(Qt::black);
    p.drawPath(pa);

    std::ofstream rewrite;
    rewrite.open("records.txt");
    if(rewrite.is_open()) {rewrite << record;}
    rewrite.close();
}

void Game::reDraw()
{
    this->repaint();
}

void Game::eat()
{
    if(snake[0]->getX() == fruit->getX() && snake[0]->getY() == fruit->getY())
    {
        snake.push_back(new Block(-1, -1));
        score++;

        fruit = new Food(field_Width, field_Height);
    }
}

void Game::checkSnake()
{
    if(snake.size() > 4)
    {
        int pos = snake.size();
        for(int i = 1; i<snake.size() ; ++i)
        {
            if(snake[0]->getX() == snake[i]->getX() && snake[0]->getY() == snake[i]->getY())
            {
                pos = i;
                break;
            }
        }

        if(pos > 4)
        {
            pos = snake.size() - pos;
            while(pos--) snake.pop_back();
        }
    }
}

void Game::restartGame()
{
    menu = false;

    bt1->setEnabled(false);
    bt1->setVisible(false);
    bt2->setEnabled(false);
    bt2->setVisible(false);

    initGame();
}

void Game::upd()
{
    field_Height = Height = sqrt(this->height());
    field_Width = Width = sqrt(this->width());
}

void Game::timeEvent()
{
    if(play)
    {
        Move();
        eat();
        checkSnake();
        Check();
    }

    upd();
}

void Game::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    if(key == Qt::Key_Right && direction != 1) {direction = 0;}
    else if(key == Qt::Key_Left && direction != 0) {direction = 1;}
    else if(key == Qt::Key_Up && direction != 3) {direction = 2;}
    else if(key == Qt::Key_Down && direction != 2) {direction = 3;}
}

void Game::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if(play) {Draw();}
    else {drawMenu();}
}
