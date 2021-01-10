#include <QApplication>
#include <game.h>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    game game;
    game.show();
    return app.exec();
}

