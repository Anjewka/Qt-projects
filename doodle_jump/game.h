#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QVector>
#include <QKeyEvent>

class game : public QWidget
{
private:
    QPixmap *doodle;
    QPixmap *texture;
    QPixmap *field;

    QVector<QPair<int, int>> lands;
    double doodle_x;
    double doodle_y;
    double vx;
    double vy;
    double top;
    double topx;

    QTimer *t1;
    QTimer *t2;

    bool play;

    int score;

public:
    game();

    void timer();
    void DrawMap();
    void DrawLands();
    void DrawDoodle();
    void checkLand();
    void swapDoodleTexture();
    void checkGame();
    void checkTop();

protected:
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // GAME_H
