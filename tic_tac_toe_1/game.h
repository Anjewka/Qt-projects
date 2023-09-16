#ifndef GAME_H
#define GAME_H

#include <cell.h>

class game
{
public:
    QVector<QVector<Cell>> map;
    QPoint GRID_LU;
    double RADIUS;
    Cell::PLAYERS CURRENT_PLAYER;

    enum OPONENTS{
        PLAYER1_PLAYER2,
        PLAYER_AI,
    };

    OPONENTS vs;

    int map_size;

    bool first;

    game();

    game(const int& size);

    void init();

    void calculate_grid();

    Cell::PLAYERS check_winner(const QVector<QVector<Cell>> &m);

    QPair<int, int> move();

    int ai_player_move(QVector<QVector<Cell>> m, int depth, const Cell::PLAYERS&& pl, int a, int b);

    void game_over();

    void resize(const int& new_size);
};

#endif // GAME_H
