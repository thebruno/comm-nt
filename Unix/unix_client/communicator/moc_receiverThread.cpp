/****************************************************************************
** Meta object code from reading C++ file 'receiverThread.h'
**
** Created: Wed Jan 13 10:10:08 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "receiverThread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'receiverThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ReceiverThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_ReceiverThread[] = {
    "ReceiverThread\0\0MessageReceived()\0"
};

const QMetaObject ReceiverThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ReceiverThread,
      qt_meta_data_ReceiverThread, 0 }
};

const QMetaObject *ReceiverThread::metaObject() const
{
    return &staticMetaObject;
}

void *ReceiverThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ReceiverThread))
        return static_cast<void*>(const_cast< ReceiverThread*>(this));
    return QThread::qt_metacast(_clname);
}

int ReceiverThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: MessageReceived(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ReceiverThread::MessageReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
