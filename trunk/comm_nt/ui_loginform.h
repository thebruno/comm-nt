/********************************************************************************
** Form generated from reading ui file 'loginform.ui'
**
** Created: Sun 8. Nov 01:30:32 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LOGINFORM_H
#define UI_LOGINFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginForm
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QGroupBox *groupBox;
    QLabel *label_2;
    QLineEdit *txtBoxPort;
    QLabel *label;
    QLineEdit *txtBoxLogin;
    QLabel *label_3;
    QComboBox *cmBoxServer;

    void setupUi(QDialog *LoginForm)
    {
        if (LoginForm->objectName().isEmpty())
            LoginForm->setObjectName(QString::fromUtf8("LoginForm"));
        LoginForm->resize(220, 180);
        LoginForm->setMinimumSize(QSize(220, 180));
        LoginForm->setMaximumSize(QSize(220, 180));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/password.ico"), QSize(), QIcon::Normal, QIcon::Off);
        LoginForm->setWindowIcon(icon);
        horizontalLayoutWidget = new QWidget(LoginForm);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 130, 201, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(horizontalLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/ok.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon1);

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(horizontalLayoutWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/cancel.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_2->setIcon(icon2);

        horizontalLayout->addWidget(pushButton_2);

        groupBox = new QGroupBox(LoginForm);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 200, 111));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 80, 21, 20));
        txtBoxPort = new QLineEdit(groupBox);
        txtBoxPort->setObjectName(QString::fromUtf8("txtBoxPort"));
        txtBoxPort->setGeometry(QRect(52, 80, 141, 20));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 50, 36, 20));
        txtBoxLogin = new QLineEdit(groupBox);
        txtBoxLogin->setObjectName(QString::fromUtf8("txtBoxLogin"));
        txtBoxLogin->setGeometry(QRect(50, 20, 141, 20));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 20, 31, 20));
        cmBoxServer = new QComboBox(groupBox);
        cmBoxServer->setObjectName(QString::fromUtf8("cmBoxServer"));
        cmBoxServer->setGeometry(QRect(50, 50, 141, 22));
        cmBoxServer->setEditable(true);
        QWidget::setTabOrder(txtBoxLogin, cmBoxServer);
        QWidget::setTabOrder(cmBoxServer, txtBoxPort);
        QWidget::setTabOrder(txtBoxPort, pushButton);
        QWidget::setTabOrder(pushButton, pushButton_2);

        retranslateUi(LoginForm);

        QMetaObject::connectSlotsByName(LoginForm);
    } // setupUi

    void retranslateUi(QDialog *LoginForm)
    {
        LoginForm->setWindowTitle(QApplication::translate("LoginForm", "Login", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("LoginForm", "Login", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("LoginForm", "Cancel", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("LoginForm", "Enter Server Parameters", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("LoginForm", "Port", 0, QApplication::UnicodeUTF8));
        txtBoxPort->setText(QApplication::translate("LoginForm", "666", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("LoginForm", "Server", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("LoginForm", "Login", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(LoginForm);
    } // retranslateUi

};

namespace Ui {
    class LoginForm: public Ui_LoginForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINFORM_H
