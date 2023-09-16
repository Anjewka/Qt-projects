#include "game.h"
#include "game.h"
#include <cell.h>
#include <iostream>
#include <cmath>
#include <QPair>
#include <QPainter>
#include <QElapsedTimer>
#include <iostream>
#include <QTime>
#include <unordered_map>

game::game()
{
    map_size = 3;

    map.resize(map_size);
    for(int i = 0; i<map.size() ; ++i) {map[i].resize(map_size);}

    vs = PLAYER_AI;
    CURRENT_PLAYER = Cell::PLAYER1;

    init();
}

game::game(const int& size)
{
    map_size = size;

    map.resize(map_size);
    for(int i = 0; i<map.size() ; ++i) {map[i].resize(map_size);}

    vs = PLAYER_AI;
    CURRENT_PLAYER = Cell::PLAYER1;

    init();
}

void game::init()
{
    for(int  i = 0; i<map.size() ; ++i)
    {
        for(int j = 0; j<map[i].size() ; ++j)
        {
            map[i][j].players = Cell::NONE;
            map[i][j].icon = new QPixmap();
        }
    }

    CURRENT_PLAYER = Cell::AI;

    first = true;
}

Cell::PLAYERS game::check_winner(const QVector<QVector<Cell>> &m)
{
    bool k = true;
    for(int j = 0; j<m[0].size(); ++j)
    {
        k = true;
        for(int i = 1; i<m.size() ; ++i)
        {
            if(m[i][j].players != m[i - 1][j].players || m[i][j].players == Cell::NONE)
            {
                k = false;
            }
        }
        if(k) {return m[0][j].players;}
    }
    k = true;
    for(int j = 0; j<m[0].size(); ++j)
    {
        k = true;
        for(int i = 1; i<m.size() ; ++i) {if(m[j][i].players != m[j][i - 1].players || m[j][i].players == Cell::NONE) {k = false;}}
        if(k) {return m[j][0].players;}
    }
    k = true;
    for(int i = 1; i<m.size() ; ++i)
    {
        if(m[i - 1][i - 1].players != m[i][i].players || m[i][i].players == Cell::NONE) {k = false;}
    }
    if(k) {return m[0][0].players;}
    k = true;
    for(int i = 1; i<m.size(); ++i)
    {
        if(m[i - 1][m[i].size() - i].players != m[i][m[i].size() - i - 1].players || m[i][m[i].size() - i - 1].players == Cell::NONE) {k = false;}
    }
    if(k) {return m[0][m[0].size() - 1].players;}

    for(int i = 0; i<m.size() ; ++i)
    {
        for(int j = 0; j<m[i].size() ; ++j)
        {
            if(m[i][j].players == Cell::NONE)
            {
                return Cell::NONE;
            }
        }
    }

    return Cell::DRAW;
}

QPair<int, int> game::move()
{
//    QTime t;
//    t.start();

//    if(map_size > 3 && first)
//    {
//        first = false;
//        return (map[0][0].players == Cell::NONE ? QPair<int, int>(0, 0) : QPair<int, int>(map_size - 1, map_size - 1));
//    }
    int best = -1000000;

    int alpha = -1000000;
    int beta = 1000000;

    QPair<int, int> best_move = qMakePair(0, 0);
    for(int i = 0; i<map.size() ; ++i)
    {
        for(int j = 0; j<map[i].size() ; ++j)
        if(map[i][j].players == Cell::NONE)
        {
            map[i][j].players = Cell::AI;
            int score = ai_player_move(map, 0, Cell::PLAYER1, alpha, beta);
            map[i][j].players = Cell::NONE;
            if(score > best) {best = score; best_move.first = i; best_move.second = j;}
        }
    }
//    std::cout << t.elapsed() << '\n';
    return best_move;
}

int game::ai_player_move(QVector<QVector<Cell>> m, int depth, const Cell::PLAYERS&& pl, int a, int b)
{
    if(depth == 6 - (map_size - 3) && map_size > 3) {return 0;}

    Cell::PLAYERS p = check_winner(m);
    if(p == Cell::AI) {return 1;}
    else if(p == Cell::PLAYER1) {return -1;}
    else if(p == Cell::DRAW) {return 0;}

    int best = pl == Cell::AI ? -1000000 : 1000000;
    for(int i = 0; i<m.size() ; ++i)
    {
        for(int j = 0; j<m[i].size() ; ++j)
        {
            if(m[i][j].players == Cell::NONE)
            {
                m[i][j].players = pl;
                int score = ai_player_move(m, depth + 1, pl == Cell::AI ? Cell::PLAYER1 : Cell::AI, a, b);
                m[i][j].players = Cell::NONE;

                if(pl == Cell::AI)
                {
                    best = std::max(score, best);
                    a = std::max(a, best);
                    if(best > a) {return best;}
                }
                else
                {
                    best = std::min(score, best);
                    b = std::min(b, best);
                    if(best < b) {return best;}
                }
            }
        }
    }
    return best;
}
