/********************************************************************************
** Form generated from reading ui file 'userchat.ui'
**
** Created: Fri 16. Oct 22:03:05 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_USERCHAT_H
#define UI_USERCHAT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserChat
{
public:

    void setupUi(QWidget *UserChat)
    {
        if (UserChat->objectName().isEmpty())
            UserChat->setObjectName(QString::fromUtf8("UserChat"));
        UserChat->resize(400, 300);

        retranslateUi(UserChat);

        QMetaObject::connectSlotsByName(UserChat);
    } // setupUi

    void retranslateUi(QWidget *UserChat)
    {
        UserChat->setWindowTitle(QApplication::translate("UserChat", "Form", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(UserChat);
    } // retranslateUi

};

namespace Ui {
    class UserChat: public Ui_UserChat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERCHAT_H
