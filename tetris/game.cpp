#include "game.h"
#include <QPainter>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QPushButton>
#include <QPainterPath>
#include <position.h>
#include <QApplication>
#include <fstream>

game::game()
{
    resize(Width * field_Width, Height * field_Height);

    this->setWindowTitle("Tetris");
    QPalette p;
    p.setColor(QPalette::Background, Qt::white);
    this->setPalette(p);

    bt1 = new QPushButton(this);
    bt2 = new QPushButton(this);

    bt1->setEnabled(false);
    bt1->setVisible(false);
    bt2->setEnabled(false);
    bt2->setVisible(false);

    connect(bt2, &QPushButton::clicked, this, &QApplication::quit);
    connect(bt1, &QPushButton::clicked, this, &game::restartGame);
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

    block = new figure();
    blocks = block->getBlocks();

    play = true;
    menu = false;

    t1 = new QTimer(this);

    connect(t1, &QTimer::timeout, this, &game::timeEvent);

    t1->start(0);

    direction = 0;
    score = 0;
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
                p.drawRect(i * Width, j * Height, Width, Height);
            }
        }

        p.setBrush(block->getColor());
        for(int i = 0; i<blocks.size() ; ++i)
        {
            p.drawRect(blocks[i].getX() * Width, blocks[i].getY() * Height, Width, Height);
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
        block->getC()->setY(block->getC()->getY() + 1);

        for(int i = 0; i<blocks.size() ; ++i)
        {
            blocks[i].setY(blocks[i].getY() + 1);
        }
    }
    else
    {
        checkLine();
        Stop();

        block = new figure();
        blocks = block->getBlocks();
    }
}

bool game::checkField()
{
    for(int i = 0; i<blocks.size() ; ++i)
    {
        if(blocks[i].getY() + 1 == field_Height || map[blocks[i].getX()][blocks[i].getY() + 1] != Qt::white)
        {
            for(int j = 0; j<blocks.size() ; ++j)
            {
                map[blocks[j].getX()][blocks[j].getY()] = block->getColor();
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
            if(blocks[i].getX() + 1 == field_Width || map[blocks[i].getX() + 1][blocks[i].getY()] != Qt::white)
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
            if(blocks[i].getX() == 0 || map[blocks[i].getX() - 1][blocks[i].getY()] != Qt::white)
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
    else {return true;}
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
    menu = true;

    bt1->setVisible(true);
    bt1->setEnabled(true);
    bt2->setVisible(true);
    bt2->setEnabled(true);
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

void game::DrawMenu()
{
    bt1->setGeometry(0.2 * this->width(), 0.43 * this->height(), 0.3 * this->width(), 0.05 * this->height());
    bt2->setGeometry(0.5 * this->width(), 0.43 * this->height(), 0.3 * this->width(), 0.05 * this->height());

    bt1->setText("Restart");
    bt2->setText("Quit");

    QPainter p(this);
    QPainterPath pa;

    p.setRenderHint(QPainter::Antialiasing);

    QFont font;
    font.setPointSize(30);
    pa.addText(QPointF(0.2 * this->width(), 0.4 * this->height()), font, "Your score: " + QString::number(score));
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
    pa.addText(QPointF(0.2 * this->width(), 0.35 * this->height()), font, "Your record: " + QString::number(record));
    p.setBrush(Qt::black);
    p.drawPath(pa);

    std::ofstream rewrite;
    rewrite.open("records.txt");
    if(rewrite.is_open()) {rewrite << record;}
    rewrite.close();
}

void game::MoveRightLeft()
{
    if(checkSide())
    {
        if(direction == 1)
        {
            int k = block->getC()->getX();
            block->getC()->setX(k + 1);
        }
        else if(direction == 2)
        {
            int k = block->getC()->getX();
            block->getC()->setX(k - 1);
        }
        for(int i = 0; i<blocks.size() ; ++i)
        {
            if(direction == 1) {blocks[i].setX(blocks[i].getX() + 1);}
            else if(direction == 2) {blocks[i].setX(blocks[i].getX() - 1);}
        }
    }

    if(direction == 4) {Move();}
    direction = 0;
}

void game::DrawScore()
{
    QPainter p(this);
    QPainterPath pa;

    p.setRenderHint(QPainter::Antialiasing);

    QFont font("Arial", 50, Qt::black);
    pa.addText(QPointF(0.45 * this->width(), 0.1 * this->height()), font, QString::number(score));
    p.setBrush(Qt::black);
    p.drawPath(pa);
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

void game::restartGame()
{
    play = false;

    bt1->setEnabled(false);
    bt1->setVisible(false);
    bt2->setEnabled(false);
    bt2->setVisible(false);

    initGame();
}

void game::Turn()
{
    if(block->getForm() != 2)
    {
        QVector<position> n;
        n.resize(4);
        for(int i = 0; i<n.size() ; ++i)
        {
            n[i].setX(blocks[i].getX());
            n[i].setY(blocks[i].getY());
        }

        for(int i = 0; i<n.size() ; ++i)
        {
            n[i].setX(block->getC()->getX() - n[i].getX());
            n[i].setY(n[i].getY() - block->getC()->getY());
        }

        for(int i = 0; i<n.size() ; ++i)
        {
            int tmp = n[i].getX();
            n[i].setX(n[i].getY());
            n[i].setY(-1 * tmp);
        }

        for(int i = 0; i<n.size() ; ++i)
        {
            n[i].setX(n[i].getX() + block->getC()->getX());
            n[i].setY(block->getC()->getY() - n[i].getY());
        }

        bool c = true;

        for(int i = 0; i<n.size() ; ++i)
        {
            if(n[i].getX() < 0 || n[i].getX() >= field_Width || n[i].getY() >= field_Height || n[i].getY() < 0) {c = false; break;}
            else if(map[n[i].getX()][n[i].getY()] != Qt::white) {c = false; break;}
        }

        if(c)
        {
            for(int i = 0; i<n.size() ; ++i)
            {
                blocks[i].setX(n[i].getX());
                blocks[i].setY(n[i].getY());
            }

            if(block->getForm() == 3)
            {
                if(block->getC()->getX() == blocks[3].getX() && block->getC()->getY() == blocks[3].getY())
                {
                    block->getC()->setX(blocks[0].getX());
                    block->getC()->setY(blocks[0].getY());
                }
                else
                {
                    block->getC()->setX(blocks[3].getX());
                    block->getC()->setY(blocks[3].getY());
                }
            }
        }
    }
}

void game::timeEvent()
{
    this->repaint();

    if(play)
    {
        if(time == 250) {Timer(); time = 0;}
        else {time++;}
        MoveRightLeft();
        Stop();
    }
}

void game::Timer()
{
    if(play)
    {
        Move();
        Stop();
    }
}

void game::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if(play) {Draw(); DrawScore();}
    else {DrawMenu();}
}

void game::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if(key == Qt::Key_Right) {direction = 1;}
    else if(key == Qt::Key_Left) {direction = 2;}
    else if(key == Qt::Key_Up && block->getForm() != 2) {direction = 3;}
    else if(key == Qt::Key_Down) {direction = 4;}
}
