#include "openglwindow.h"
#include "glprogram.h"

#include <QGuiApplication>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QtMath>
#include <QDebug>
#include <QDirIterator>
#include <QDir>
#include <QFileInfo>
#include <QPushButton>

void copyAndReplaceFolderContents(const QString &fromDir, const QString &toDir, bool copyAndRemove = false) {
    if(!QFileInfo::exists(toDir)) {QDir().mkdir(toDir);}

    QDirIterator it(fromDir, QDirIterator::Subdirectories);
    QDir dir(fromDir);
    const int absSourcePathLength = dir.absoluteFilePath(fromDir).length();

    while (it.hasNext()){
        it.next();
        const auto fileInfo = it.fileInfo();
        if(!fileInfo.isHidden()) { //filters dot and dotdot
            const QString subPathStructure = fileInfo.absoluteFilePath().mid(absSourcePathLength);
            const QString constructedAbsolutePath = toDir + subPathStructure;

            if(fileInfo.isDir()){
                //Create directory in target folder
//                dir.mkpath(constructedAbsolutePath);
            } else if(fileInfo.isFile()) {
                //Copy File to target directory

//                Remove file at target location, if it exists, or QFile::copy will fail
                QFile::remove(constructedAbsolutePath);
                QFile::copy(fileInfo.absoluteFilePath(), constructedAbsolutePath);
            }
        }
    }

    if(copyAndRemove)
        dir.removeRecursively();
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    if (QFileInfo::exists("assets"))
    {
        copyAndReplaceFolderContents("D:\\Fractal3D\\assets\\", ".\\assets\\");
    }

    QFile file("/assets");
    if(file.exists()) {std::cout << "True";}

    QSurfaceFormat format;
    format.setSamples(16);

    glprogram gl;
    gl.setFormat(format);
    gl.resize(800, 800);
    gl.show();

    gl.setAnimating(true);

    return app.exec();
}
