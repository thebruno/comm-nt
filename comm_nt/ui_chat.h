/********************************************************************************
** Form generated from reading ui file 'chat.ui'
**
** Created: Sun 8. Nov 21:25:06 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CHAT_H
#define UI_CHAT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Chat
{
public:
    QWidget *centralwidget;
    QGroupBox *grBoxChatWith;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QTextEdit *txtChatHistory;
    QPlainTextEdit *txtPlainChat;
    QPushButton *btnSend;
    QPushButton *btnClose;

    void setupUi(QMainWindow *Chat)
    {
        if (Chat->objectName().isEmpty())
            Chat->setObjectName(QString::fromUtf8("Chat"));
        Chat->resize(380, 280);
        Chat->setMinimumSize(QSize(380, 280));
        Chat->setMaximumSize(QSize(380, 280));
        centralwidget = new QWidget(Chat);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        grBoxChatWith = new QGroupBox(centralwidget);
        grBoxChatWith->setObjectName(QString::fromUtf8("grBoxChatWith"));
        grBoxChatWith->setGeometry(QRect(10, 0, 361, 241));
        pushButton = new QPushButton(grBoxChatWith);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(200, 260, 75, 23));
        pushButton_2 = new QPushButton(grBoxChatWith);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(300, 270, 75, 23));
        txtChatHistory = new QTextEdit(grBoxChatWith);
        txtChatHistory->setObjectName(QString::fromUtf8("txtChatHistory"));
        txtChatHistory->setGeometry(QRect(10, 20, 341, 111));
        txtChatHistory->setUndoRedoEnabled(false);
        txtChatHistory->setReadOnly(true);
        txtPlainChat = new QPlainTextEdit(grBoxChatWith);
        txtPlainChat->setObjectName(QString::fromUtf8("txtPlainChat"));
        txtPlainChat->setGeometry(QRect(10, 140, 341, 91));
        btnSend = new QPushButton(centralwidget);
        btnSend->setObjectName(QString::fromUtf8("btnSend"));
        btnSend->setGeometry(QRect(10, 250, 81, 23));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/send.ico"), QSize(), QIcon::Normal, QIcon::Off);
        btnSend->setIcon(icon);
        btnSend->setAutoDefault(true);
        btnSend->setDefault(true);
        btnClose = new QPushButton(centralwidget);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        btnClose->setGeometry(QRect(300, 250, 75, 23));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/exit.ico"), QSize(), QIcon::Normal, QIcon::Off);
        btnClose->setIcon(icon1);
        Chat->setCentralWidget(centralwidget);

        retranslateUi(Chat);

        QMetaObject::connectSlotsByName(Chat);
    } // setupUi

    void retranslateUi(QMainWindow *Chat)
    {
        Chat->setWindowTitle(QApplication::translate("Chat", "Chat", 0, QApplication::UnicodeUTF8));
        grBoxChatWith->setTitle(QApplication::translate("Chat", "Chat with:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Chat", "PushButton", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Chat", "PushButton", 0, QApplication::UnicodeUTF8));
        btnSend->setText(QApplication::translate("Chat", "Send", 0, QApplication::UnicodeUTF8));
        btnSend->setShortcut(QApplication::translate("Chat", "Return", 0, QApplication::UnicodeUTF8));
        btnClose->setText(QApplication::translate("Chat", "Close", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Chat);
    } // retranslateUi

};

namespace Ui {
    class Chat: public Ui_Chat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHAT_H
