#include <QApplication>
#include <fractal.h>
#include <QLibrary>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
//    QLibrary lib;
//    QString path = "C:/Windows/System32/OpenCL.dll";
//    app.addLibraryPath(path);

//    if(QLibrary::isLibrary(path)) {
//    lib.setFileName(path);
//    lib.load();
//    }
//    if(lib.isLoaded())
//    qDebug() << "Ok\n";
//    else
//    qDebug() << "Error " << lib.errorString() << "\n";
//    } else
//    qDebug() << "Not a library\n";

    class fractal c;
    c.show();
    return app.exec();
}
