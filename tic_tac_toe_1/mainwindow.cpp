#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QTimer>
#include "game.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMinimumSize(500, 500);

    g = game(3);

    QIcon iqon("img/o.png");
    ui->checkBox->setIcon(iqon);
    iqon = QIcon("img/x.png");
    ui->checkBox_2->setIcon(iqon);

    ui->radioButton->setChecked(true);
    ui->checkBox_2->setChecked(true);

    g.CURRENT_PLAYER = Cell::PLAYER1;
    g.vs = g.OPONENTS::PLAYER_AI;

    first_player = true;

    ui->label_4->setVisible(false);
    ui->winner->setVisible(false);

    t = new QTimer(this);
    connect(t, &QTimer::timeout, this, &MainWindow::Timer);

    play = false;

    t->start(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    int i = ui->comboBox_2->currentIndex();
    switch (i)
    {
    case 0:
        g = game(3);
        break;
    case 1:
        g = game(4);
        break;
    case 2:
        g = game(5);
        break;
    case 3:
        g = game(6);
        break;
    case 4:
        g = game(7);
        break;
    }

    if(ui->radioButton->isChecked()) {g.vs = g.OPONENTS::PLAYER_AI;}
    else {g.vs = g.OPONENTS::PLAYER1_PLAYER2;}

    if(ui->checkBox->isChecked()) {g.CURRENT_PLAYER = Cell::AI; first_player = false;}
    else {g.CURRENT_PLAYER = Cell::PLAYER1; first_player = true;}

    ui->checkBox->setEnabled(false);
    ui->checkBox_2->setEnabled(false);
    ui->radioButton->setEnabled(false);
    ui->radioButton_2->setEnabled(false);

    ui->label_4->setVisible(false);
    ui->winner->setVisible(false);
}

void MainWindow::Timer()
{
    if(play)
    {
        if(g.check_winner(g.map) != Cell::NONE) {game_over();}

        if(g.CURRENT_PLAYER == Cell::AI)
        {
            QPair<int, int> k = g.move();
            g.map[k.first][k.second].players = Cell::AI;
            g.map[k.first][k.second].icon->load(first_player ? "img/o.png" : "img/x.png");
            g.CURRENT_PLAYER = Cell::PLAYER1;
        }
    }

    calculate_grid();
    calculate_layout();
    calculate_lables();
    this->repaint();
}

void MainWindow::calculate_grid()
{
    double r = std::min(width(), height()) * 0.5;
    double x = 4 * (width() - r) / 6;
    double y = (height() - r) / 2;

    g.GRID_LU = QPoint(x, y);
    g.RADIUS = r;
    for(int i = 0; i<g.map.size() ; ++i)
    {
        for(int j = 0; j<g.map[i].size() ; ++j)
        {
            g.map[i][j].lu_point.rx() = x + g.RADIUS / g.map_size * (i);
            g.map[i][j].lu_point.ry() = y + g.RADIUS / g.map_size * (j);

            g.map[i][j].rd_point.rx() = g.RADIUS / g.map_size;
            g.map[i][j].rd_point.ry() = g.RADIUS / g.map_size;
        }
    }
}

void MainWindow::draw_grid()
{
    QPainter p(this);

    for(int i = 0; i<g.map.size() ; ++i)
    {
        for(int j = 0; j<g.map[i].size() ; ++j)
        {
            if(g.map[i][j].players != Cell::NONE)
            {
                g.map[i][j].icon = new QPixmap(g.map[i][j].icon->scaled(g.RADIUS / (2 * g.map_size),
                g.RADIUS / (2 * g.map_size), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
            p.setBrush(QColor(150, 150, 150));
            p.drawRect(g.map[i][j].lu_point.rx(), g.map[i][j].lu_point.ry(), g.map[i][j].rd_point.rx(), g.map[i][j].rd_point.ry());
            p.drawPixmap(g.map[i][j].lu_point.rx() + g.RADIUS / (4 * g.map_size), g.map[i][j].lu_point.ry() + g.RADIUS / (4 * g.map_size), *g.map[i][j].icon);
        }
    }
}

void MainWindow::calculate_layout()
{
    int x_len = g.map[0][g.map_size - 1].lu_point.rx();
    int y_len = g.map[0][0].lu_point.ry();
    int delta =  g.map[0][g.map_size - 1].rd_point.ry() + g.map[0][g.map_size - 1].lu_point.ry() -
            g.map[0][0].lu_point.ry();
    QPoint lu = QPoint(x_len * 0.2, y_len + 0.2 * delta);
    QPoint rd = QPoint(x_len * 0.8, y_len + 0.8 * delta);

    ui->verticalLayout->setGeometry(QRect(lu, rd));
}

void MainWindow::calculate_lables()
{
    int x = g.map[g.map_size / 2][0].lu_point.rx();
    int y = g.map[g.map_size / 2][0].lu_point.ry();

    ui->winner->setGeometry(x, y - 50, 100, 50);
    ui->label_4->setGeometry(x + 100, y - 50, 50, 50);
}

void MainWindow::game_over()
{
    ui->checkBox->setEnabled(true);
    ui->checkBox_2->setEnabled(true);
    ui->radioButton->setEnabled(true);
    ui->radioButton_2->setEnabled(true);

    Cell::PLAYERS win = g.check_winner(g.map);
    if(win != Cell::DRAW)
    {
        ui->winner->setText("Победитель");

        QPixmap mp;
        if(win == Cell::PLAYER1) {mp = QPixmap(first_player ? "img/x.png" : "img/o.png");}
        else if(win == Cell::PLAYER2 || win == Cell::AI){mp = QPixmap(!first_player ? "img/x.png" : "img/o.png");}
        ui->label_4->setPixmap(mp.scaled(25, 25, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        ui->label_4->setVisible(true);
        ui->winner->setVisible(true);
    } else {
        ui->winner->setText("ничья");
        ui->winner->setVisible(true);
    }

    play = false;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    draw_grid();
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    double x = e->pos().rx();
    double y = e->pos().ry();

    if(play)
    {
        for(int i = 0; i<g.map.size() ; ++i)
        {
            for(int j = 0; j<g.map[i].size() ; ++j)
            {
                if(g.map[i][j].players == Cell::NONE)
                {
                    double mx1 = g.map[i][j].lu_point.rx();
                    double my1 = g.map[i][j].lu_point.ry();
                    double mx2 = g.map[i][j].rd_point.rx();
                    double my2 = g.map[i][j].rd_point.ry();

                    if(mx1 < x && my1 < y && mx2 + mx1 > x && my2 + my1 > y)
                    {
                        if (g.CURRENT_PLAYER == Cell::PLAYER1)
                        {
                            g.map[i][j].players = Cell::PLAYER1;
                            g.map[i][j].icon->load(first_player ? "img/x.png" : "img/o.png");
                            g.vs == g.PLAYER_AI ? g.CURRENT_PLAYER = Cell::AI : g.CURRENT_PLAYER = Cell::PLAYER2;
                        }
                        else if(g.CURRENT_PLAYER == Cell::PLAYER2)
                        {
                            g.map[i][j].players = Cell::PLAYER2;
                            g.map[i][j].icon->load("img/o.png");
                            g.CURRENT_PLAYER = Cell::PLAYER1;
                        }
                    }
                }
            }
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    init();
    play = true;
}

void MainWindow::on_checkBox_clicked()
{
    if(!play)
    {
        ui->checkBox_2->setChecked(false);
        ui->checkBox->setChecked(true);
    }
}

void MainWindow::on_checkBox_2_clicked()
{
    if(!play)
    {
        ui->checkBox_2->setChecked(true);
        ui->checkBox->setChecked(false);
    }
}

void MainWindow::on_radioButton_clicked()
{
    if(!play)
    {
        ui->radioButton->setChecked(true);
        ui->radioButton_2->setChecked(false);
    }
}


void MainWindow::on_radioButton_2_clicked()
{
    if(!play)
    {
        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(true);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    game_over();
}

