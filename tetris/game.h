#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QColor>
#include <QTimer>
#include <figure.h>

class game : public QWidget
{
private:
    int Width = 40;
    int Height = 40;
    int field_Width = 10;
    int field_Height = 25;

    int score = 0;

    bool play;
    bool menu;

    int direction;

    QTimer* t1;

    int time = 0;

    QVector<QVector<QColor>> map;
    figure *block;

    QVector<position> blocks;
    QPushButton *bt1, *bt2;

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
    void setEnabledVisible_label(QLabel *l, bool t);
    void setEnabledVisible_Button(QPushButton *b, bool t);
    void DrawMenu();
    void restartGame();
    void MoveRightLeft();
    void DrawScore();
    void timeEvent();
    void setShape();
    void DrawLine();
    void Pause();
    void Stop();
    void Turn();
    void Timer();

protected:
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // GAME_H
