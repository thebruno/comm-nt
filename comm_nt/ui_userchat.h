/********************************************************************************
** Form generated from reading ui file 'userchat.ui'
**
** Created: Fri 6. Nov 23:26:26 2009
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
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserChat
{
public:
    QGroupBox *groupBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QTextEdit *txtChatHistory;
    QPlainTextEdit *txtPlainChat;
    QPushButton *btnSend;
    QPushButton *btnClose;

    void setupUi(QWidget *UserChat)
    {
        if (UserChat->objectName().isEmpty())
            UserChat->setObjectName(QString::fromUtf8("UserChat"));
        UserChat->resize(400, 300);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/chat.ico"), QSize(), QIcon::Normal, QIcon::Off);
        UserChat->setWindowIcon(icon);
        groupBox = new QGroupBox(UserChat);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 371, 241));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(200, 260, 75, 23));
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(300, 270, 75, 23));
        txtChatHistory = new QTextEdit(groupBox);
        txtChatHistory->setObjectName(QString::fromUtf8("txtChatHistory"));
        txtChatHistory->setGeometry(QRect(10, 20, 351, 111));
        txtPlainChat = new QPlainTextEdit(groupBox);
        txtPlainChat->setObjectName(QString::fromUtf8("txtPlainChat"));
        txtPlainChat->setGeometry(QRect(10, 140, 351, 101));
        btnSend = new QPushButton(UserChat);
        btnSend->setObjectName(QString::fromUtf8("btnSend"));
        btnSend->setGeometry(QRect(20, 260, 81, 23));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/send.ico"), QSize(), QIcon::Normal, QIcon::Off);
        btnSend->setIcon(icon1);
        btnSend->setAutoDefault(true);
        btnClose = new QPushButton(UserChat);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        btnClose->setGeometry(QRect(290, 260, 75, 23));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/exit.ico"), QSize(), QIcon::Normal, QIcon::Off);
        btnClose->setIcon(icon2);

        retranslateUi(UserChat);

        QMetaObject::connectSlotsByName(UserChat);
    } // setupUi

    void retranslateUi(QWidget *UserChat)
    {
        UserChat->setWindowTitle(QApplication::translate("UserChat", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("UserChat", "GroupBox", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("UserChat", "PushButton", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("UserChat", "PushButton", 0, QApplication::UnicodeUTF8));
        btnSend->setText(QApplication::translate("UserChat", "Send", 0, QApplication::UnicodeUTF8));
        btnClose->setText(QApplication::translate("UserChat", "Close", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(UserChat);
    } // retranslateUi

};

namespace Ui {
    class UserChat: public Ui_UserChat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERCHAT_H
