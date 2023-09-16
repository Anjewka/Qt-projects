#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QTimer>
#include <QMouseEvent>
#include <QLabel>
#include "cell.h"
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void draw();

    void init();

    void Timer();

    void calculate_grid();

    void draw_grid();

    void calculate_layout();

    void calculate_lables();

    void game_over();

    void drawLine();

    ~MainWindow();

private:
    Ui::MainWindow *ui;

    game g;

    QTimer *t;

    bool play;

    bool first_player;

    QLabel winner;

protected:
    void paintEvent(QPaintEvent *e) override;

    void mousePressEvent(QMouseEvent *e) override;
private slots:
    void on_pushButton_clicked();
    void on_checkBox_clicked();
    void on_checkBox_2_clicked();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_pushButton_2_clicked();
};
#endif // MAINWINDOW_H
