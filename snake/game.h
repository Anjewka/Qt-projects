#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QKeyEvent>
#include <QPushButton>
#include <QLabel>
#include <block.h>
#include <food.h>
#include <QGridLayout>

class Game : public QWidget
{
private:
    double Width = 30;
    double Height = 30;
    double field_Width = 30;
    double field_Height = 30;
    int time = 150;

    int timerId;
    int score;

    int direction;
    bool play;
    bool menu;
    QVector<Block*> snake;
    Food *fruit;

    QPushButton *bt1;
    QPushButton *bt2;


public:
    Game();
    void initGame();
    void Draw();
    void Move();
    void Check();
    void gameOver();
    void drawMenu();
    void eat();
    void checkSnake();
    void restartGame();
    void pause();
    void upd();

protected:
    void timerEvent(QTimerEvent*) override;
    void keyPressEvent(QKeyEvent *) override;
    void paintEvent(QPaintEvent *event) override;
};

#endif // GAME_H
