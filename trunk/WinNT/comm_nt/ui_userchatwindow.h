/********************************************************************************
** Form generated from reading ui file 'userchatwindow.ui'
**
** Created: Fri 16. Oct 22:00:34 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_USERCHATWINDOW_H
#define UI_USERCHATWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_userchatwindow
{
public:

    void setupUi(QWidget *userchatwindow)
    {
        if (userchatwindow->objectName().isEmpty())
            userchatwindow->setObjectName(QString::fromUtf8("userchatwindow"));
        userchatwindow->resize(400, 300);

        retranslateUi(userchatwindow);

        QMetaObject::connectSlotsByName(userchatwindow);
    } // setupUi

    void retranslateUi(QWidget *userchatwindow)
    {
        userchatwindow->setWindowTitle(QApplication::translate("userchatwindow", "Form", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(userchatwindow);
    } // retranslateUi

};

namespace Ui {
    class userchatwindow: public Ui_userchatwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERCHATWINDOW_H
