#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QKeyEvent>
#include <pushbutton.h>
#include <QLabel>

class game : public QWidget
{
private:
    const int Width = 40;
    const int Height = 40;
    const int field_Width = 8;
    const int field_Height = 20;
    const int time = 200;
    int form;
    int color;
    int score = 0;
    QPoint center;

    int timerId;
    bool play;

    int direction;

    QVector<QPoint> blocks;
    QVector<QVector<int>> map;

public:
    game();

    void initGame();
    void Draw();
    void Move();
    void setBlocks();
    void check();
    bool checkField();
    bool checkSide();
    void checkLine();
    void gameOver();
    void pushDown(int row);
    void Stop();
    void Turn();

protected:
    void timerEvent(QTimerEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // GAME_H
