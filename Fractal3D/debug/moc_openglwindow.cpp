/****************************************************************************
** Meta object code from reading C++ file 'openglwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../openglwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'openglwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OpenGLWindow_t {
    const uint offsetsAndSize[8];
    char stringdata0[36];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_OpenGLWindow_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_OpenGLWindow_t qt_meta_stringdata_OpenGLWindow = {
    {
QT_MOC_LITERAL(0, 12), // "OpenGLWindow"
QT_MOC_LITERAL(13, 11), // "renderLater"
QT_MOC_LITERAL(25, 0), // ""
QT_MOC_LITERAL(26, 9) // "renderNow"

    },
    "OpenGLWindow\0renderLater\0\0renderNow"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OpenGLWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   26,    2, 0x0a,    1 /* Public */,
       3,    0,   27,    2, 0x0a,    2 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void OpenGLWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OpenGLWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->renderLater(); break;
        case 1: _t->renderNow(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject OpenGLWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QWindow::staticMetaObject>(),
    qt_meta_stringdata_OpenGLWindow.offsetsAndSize,
    qt_meta_data_OpenGLWindow,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_OpenGLWindow_t
, QtPrivate::TypeAndForceComplete<OpenGLWindow, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *OpenGLWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OpenGLWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OpenGLWindow.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QOpenGLExtraFunctions"))
        return static_cast< QOpenGLExtraFunctions*>(this);
    return QWindow::qt_metacast(_clname);
}

int OpenGLWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
