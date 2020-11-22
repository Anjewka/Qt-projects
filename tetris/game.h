#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QKeyEvent>
#include <pushbutton.h>
#include <QLabel>
#include <QPushButton>
#include <QColor>

class game : public QWidget
{
private:
    int Width = 40;
    int Height = 40;
    int field_Width = 10;
    int field_Height = 25;

    int time = 200;
    int form;
    int score = 0;
    int prev;
    QPoint center;
    QColor color;

    int timerId;
    bool play;

    int direction;

    QVector<QPoint> blocks;
    QVector<QVector<QColor>> map;

    QPushButton *btnPause;
    QLabel *score_label;

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
    void DrawLine();
    void Pause();
    void Stop();
    void Turn();

protected:
    void timerEvent(QTimerEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // GAME_H
