#ifndef CELL_H
#define CELL_H

#include <QPixmap>
#include <QPoint>

class Cell
{
public:

    enum PLAYERS
    {
        PLAYER1,
        PLAYER2,
        AI,
        NONE,
        DRAW,
    };

    PLAYERS players;

    QPixmap *icon;
    QPoint lu_point; QPoint rd_point;

    Cell();
    Cell(PLAYERS p, QPoint first, QPoint second);

    bool operator==(const Cell& c);
    void operator=(const Cell& c);
};

#endif // CELL_H
