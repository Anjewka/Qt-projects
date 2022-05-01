#include <QApplication>
#include <fractal.h>
#include <QLibrary>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLibrary lib;
    QString path = "source/OpenCL.dll";
    app.addLibraryPath(path);

    if(QLibrary::isLibrary(path)) {
        lib.setFileName(path);
        lib.load();
    }

    if(lib.isLoaded()) {
        std::cout << "Ok\n";
    } else {
        std::cout << "Error " << "\n";
    }

    class fractal c;
    c.show();
    return app.exec();
}
