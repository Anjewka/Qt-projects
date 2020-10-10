#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QKeyEvent>

class Game : public QWidget
{
public:
    Game();

private:
    const int Width = 20;
    const int Height = 20;
    const int field_Width = 20;
    const int field_Height = 20;
    const int time = 150;

    int timerId;

    enum Directions {left, right, up, down};
    Directions dir = right;

    bool play;
    QVector<QPoint> snake;

    QPoint f;

    void initGame();
    void Draw();
    void Fruit();
    void Move();
    bool Check();
    void gameOver();
    void eat();
    void checkSnake();

protected:
    void timerEvent(QTimerEvent*) override;
    void keyPressEvent(QKeyEvent *) override;
    void paintEvent(QPaintEvent *event) override;
};

#endif // GAME_H
