#include <QApplication>
#include <clock.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Clock c;
    c.show();
    return app.exec();
}
