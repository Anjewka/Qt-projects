#include <QApplication>
#include <model.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    model m;
    m.show();
    return app.exec();
}
