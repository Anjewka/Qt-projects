#include <QApplication>
#include <fractal.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    class fractal c;
    c.show();
    return app.exec();
}
