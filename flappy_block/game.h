#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>

class game : public QWidget
{
private:
    double x; double y;
    double g;
    double a;
    int score;
    double speed;
    QTimer *t;
    std::vector<std::pair<std::pair<double, double>, bool>> hinter;
    std::vector<std::pair<double, double>> block;

public:
    game();

    void Timer();
    void Draw();
    void updateScene();
    void drawHinter();
    void drawScore();
    void Gravity();
    void checkScore();
    void Hinter();
    void setUp();
    void checkDeath();

protected:
    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
};

#endif // GAME_H
