/********************************************************************************
** Form generated from reading ui file 'groupchatwindow.ui'
**
** Created: Fri 16. Oct 22:00:34 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_GROUPCHATWINDOW_H
#define UI_GROUPCHATWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_groupchatwindow
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *groupchatwindow)
    {
        if (groupchatwindow->objectName().isEmpty())
            groupchatwindow->setObjectName(QString::fromUtf8("groupchatwindow"));
        groupchatwindow->resize(800, 600);
        centralwidget = new QWidget(groupchatwindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        groupchatwindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(groupchatwindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        groupchatwindow->setMenuBar(menubar);
        statusbar = new QStatusBar(groupchatwindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        groupchatwindow->setStatusBar(statusbar);

        retranslateUi(groupchatwindow);

        QMetaObject::connectSlotsByName(groupchatwindow);
    } // setupUi

    void retranslateUi(QMainWindow *groupchatwindow)
    {
        groupchatwindow->setWindowTitle(QApplication::translate("groupchatwindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(groupchatwindow);
    } // retranslateUi

};

namespace Ui {
    class groupchatwindow: public Ui_groupchatwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GROUPCHATWINDOW_H
