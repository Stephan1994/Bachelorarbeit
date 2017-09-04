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
    QByteArrayData data[8];
    char stringdata0[139];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CustomMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CustomMainWindow_t qt_meta_stringdata_CustomMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 16), // "CustomMainWindow"
QT_MOC_LITERAL(1, 17, 21), // "slotLeftButtonPressed"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 22), // "slotRightButtonPressed"
QT_MOC_LITERAL(4, 63, 19), // "slotUpButtonPressed"
QT_MOC_LITERAL(5, 83, 21), // "slotDownButtonPressed"
QT_MOC_LITERAL(6, 105, 18), // "slotButtonReleased"
QT_MOC_LITERAL(7, 124, 14) // "slotDisconnect"

    },
    "CustomMainWindow\0slotLeftButtonPressed\0"
    "\0slotRightButtonPressed\0slotUpButtonPressed\0"
    "slotDownButtonPressed\0slotButtonReleased\0"
    "slotDisconnect"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CustomMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x0a /* Public */,
       4,    0,   46,    2, 0x0a /* Public */,
       5,    0,   47,    2, 0x0a /* Public */,
       6,    0,   48,    2, 0x0a /* Public */,
       7,    0,   49,    2, 0x0a /* Public */,

 // slots: parameters
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
        case 0: _t->slotLeftButtonPressed(); break;
        case 1: _t->slotRightButtonPressed(); break;
        case 2: _t->slotUpButtonPressed(); break;
        case 3: _t->slotDownButtonPressed(); break;
        case 4: _t->slotButtonReleased(); break;
        case 5: _t->slotDisconnect(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
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
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
