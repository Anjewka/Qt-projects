QT += gui
QT += opengl
QT += widgets

CONFIG += c++11 console
CONFIG += console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        glprogram.cpp \
        main.cpp \
        openglwindow.cpp

# Default rules for deployment.

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    glprogram.h \
    openglwindow.h

DISTFILES += \
    assets/compute_shader.glsl \
    assets/fragment_shader.glsl \
    assets/metall.jpg \
    assets/palette.png \
    assets/palette3.png \
    assets/ray_tracing.glsl \
    assets/vertex_shader.glsl


