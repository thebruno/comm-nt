/********************************************************************************
** Form generated from reading ui file 'userchat.ui'
**
** Created: Thu 5. Nov 22:53:38 2009
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
    QTextEdit *textEdit;
    QPlainTextEdit *plainTextEdit;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;

    void setupUi(QWidget *UserChat)
    {
        if (UserChat->objectName().isEmpty())
            UserChat->setObjectName(QString::fromUtf8("UserChat"));
        UserChat->resize(400, 300);
        groupBox = new QGroupBox(UserChat);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 371, 241));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(200, 260, 75, 23));
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(300, 270, 75, 23));
        textEdit = new QTextEdit(groupBox);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(10, 20, 351, 111));
        plainTextEdit = new QPlainTextEdit(groupBox);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 140, 351, 101));
        pushButton_3 = new QPushButton(UserChat);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(20, 260, 75, 23));
        pushButton_4 = new QPushButton(UserChat);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(290, 260, 75, 23));

        retranslateUi(UserChat);

        QMetaObject::connectSlotsByName(UserChat);
    } // setupUi

    void retranslateUi(QWidget *UserChat)
    {
        UserChat->setWindowTitle(QApplication::translate("UserChat", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("UserChat", "GroupBox", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("UserChat", "PushButton", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("UserChat", "PushButton", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("UserChat", "PushButton", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("UserChat", "PushButton", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(UserChat);
    } // retranslateUi

};

namespace Ui {
    class UserChat: public Ui_UserChat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERCHAT_H
