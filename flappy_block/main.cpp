#include <QApplication>
#include <game.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    game c;
    c.show();
    return app.exec();
}
