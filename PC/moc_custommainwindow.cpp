/****************************************************************************
** Meta object code from reading C++ file 'custommainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "custommainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'custommainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CustomMainWindow_t {
    QByteArrayData data[11];
    char stringdata0[176];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CustomMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CustomMainWindow_t qt_meta_stringdata_CustomMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 16), // "CustomMainWindow"
QT_MOC_LITERAL(1, 17, 11), // "newDistance"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 8), // "distance"
QT_MOC_LITERAL(4, 39, 21), // "slotLeftButtonPressed"
QT_MOC_LITERAL(5, 61, 22), // "slotRightButtonPressed"
QT_MOC_LITERAL(6, 84, 19), // "slotUpButtonPressed"
QT_MOC_LITERAL(7, 104, 21), // "slotDownButtonPressed"
QT_MOC_LITERAL(8, 126, 18), // "slotButtonReleased"
QT_MOC_LITERAL(9, 145, 14), // "slotDisconnect"
QT_MOC_LITERAL(10, 160, 15) // "slotGetDistance"

    },
    "CustomMainWindow\0newDistance\0\0distance\0"
    "slotLeftButtonPressed\0slotRightButtonPressed\0"
    "slotUpButtonPressed\0slotDownButtonPressed\0"
    "slotButtonReleased\0slotDisconnect\0"
    "slotGetDistance"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CustomMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   57,    2, 0x0a /* Public */,
       5,    0,   58,    2, 0x0a /* Public */,
       6,    0,   59,    2, 0x0a /* Public */,
       7,    0,   60,    2, 0x0a /* Public */,
       8,    0,   61,    2, 0x0a /* Public */,
       9,    0,   62,    2, 0x0a /* Public */,
      10,    0,   63,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CustomMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CustomMainWindow *_t = static_cast<CustomMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newDistance((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->slotLeftButtonPressed(); break;
        case 2: _t->slotRightButtonPressed(); break;
        case 3: _t->slotUpButtonPressed(); break;
        case 4: _t->slotDownButtonPressed(); break;
        case 5: _t->slotButtonReleased(); break;
        case 6: _t->slotDisconnect(); break;
        case 7: _t->slotGetDistance(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CustomMainWindow::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CustomMainWindow::newDistance)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CustomMainWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CustomMainWindow.data,
      qt_meta_data_CustomMainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CustomMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CustomMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CustomMainWindow.stringdata0))
        return static_cast<void*>(const_cast< CustomMainWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int CustomMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CustomMainWindow::newDistance(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
