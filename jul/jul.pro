QT += widgets
CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += \
        complex.cpp \
        fractal.cpp \
        main.cpp

HEADERS += \
    include/CL/cl.h \
    include/CL/cl_d3d10.h \
    include/CL/cl_d3d11.h \
    include/CL/cl_dx9_media_sharing.h \
    include/CL/cl_dx9_media_sharing_intel.h \
    include/CL/cl_egl.h \
    include/CL/cl_ext.h \
    include/CL/cl_ext_intel.h \
    include/CL/cl_gl.h \
    include/CL/cl_gl_ext.h \
    include/CL/cl_half.h \
    include/CL/cl_icd.h \
    include/CL/cl_layer.h \
    include/CL/cl_platform.h \
    include/CL/cl_va_api_media_sharing_intel.h \
    include/CL/cl_version.h \
    include/CL/opencl.h \
    complex.h \
    fractal.h


LIBS += -L"$$PWD/lib/OpenCL/lib/" -lOpenCL

INCLUDEPATH += "$$PWD/lib/OpenCL/include/"

HEADERS += \
    $$files(*.h, true)  \
    $$files(*.hpp, true)

DISTFILES += \
    GPU.cl

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

