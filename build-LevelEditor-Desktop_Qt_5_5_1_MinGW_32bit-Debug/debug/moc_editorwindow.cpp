/****************************************************************************
** Meta object code from reading C++ file 'editorwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../LevelEditor/editorwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'editorwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_EditorWindow_t {
    QByteArrayData data[13];
    char stringdata0[278];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EditorWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EditorWindow_t qt_meta_stringdata_EditorWindow = {
    {
QT_MOC_LITERAL(0, 0, 12), // "EditorWindow"
QT_MOC_LITERAL(1, 13, 22), // "on_save_button_clicked"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 7), // "checked"
QT_MOC_LITERAL(4, 45, 29), // "on_button_update_size_clicked"
QT_MOC_LITERAL(5, 75, 29), // "on_slider_height_valueChanged"
QT_MOC_LITERAL(6, 105, 5), // "value"
QT_MOC_LITERAL(7, 111, 28), // "on_slider_width_valueChanged"
QT_MOC_LITERAL(8, 140, 26), // "on_button_shift_up_clicked"
QT_MOC_LITERAL(9, 167, 28), // "on_button_shift_left_clicked"
QT_MOC_LITERAL(10, 196, 28), // "on_button_shift_down_clicked"
QT_MOC_LITERAL(11, 225, 29), // "on_button_shift_right_clicked"
QT_MOC_LITERAL(12, 255, 22) // "on_load_button_clicked"

    },
    "EditorWindow\0on_save_button_clicked\0"
    "\0checked\0on_button_update_size_clicked\0"
    "on_slider_height_valueChanged\0value\0"
    "on_slider_width_valueChanged\0"
    "on_button_shift_up_clicked\0"
    "on_button_shift_left_clicked\0"
    "on_button_shift_down_clicked\0"
    "on_button_shift_right_clicked\0"
    "on_load_button_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EditorWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x08 /* Private */,
       4,    0,   62,    2, 0x08 /* Private */,
       5,    1,   63,    2, 0x08 /* Private */,
       7,    1,   66,    2, 0x08 /* Private */,
       8,    0,   69,    2, 0x08 /* Private */,
       9,    0,   70,    2, 0x08 /* Private */,
      10,    0,   71,    2, 0x08 /* Private */,
      11,    0,   72,    2, 0x08 /* Private */,
      12,    0,   73,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void EditorWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EditorWindow *_t = static_cast<EditorWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_save_button_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->on_button_update_size_clicked(); break;
        case 2: _t->on_slider_height_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_slider_width_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_button_shift_up_clicked(); break;
        case 5: _t->on_button_shift_left_clicked(); break;
        case 6: _t->on_button_shift_down_clicked(); break;
        case 7: _t->on_button_shift_right_clicked(); break;
        case 8: _t->on_load_button_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject EditorWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_EditorWindow.data,
      qt_meta_data_EditorWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *EditorWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EditorWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_EditorWindow.stringdata0))
        return static_cast<void*>(const_cast< EditorWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int EditorWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
