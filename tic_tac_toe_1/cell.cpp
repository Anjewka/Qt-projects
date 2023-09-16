#include "cell.h"

Cell::Cell()
{
    icon = new QPixmap();
    players = NONE;
}

Cell::Cell(PLAYERS p, QPoint first, QPoint second)
{
    icon = new QPixmap();

    lu_point = first;
    rd_point = second;

    players = p;
}

bool Cell::operator==(const Cell &c)
{
    return players == c.players;
}

void Cell::operator=(const Cell &c)
{
    icon = c.icon;
    players = c.players;
    lu_point = c.lu_point;
    rd_point = c.rd_point;
}
