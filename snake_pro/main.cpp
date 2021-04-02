#include <QApplication>
#include <game.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    game game;
    game.show();
    return app.exec();
}
