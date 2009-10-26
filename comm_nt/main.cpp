#include <QtGui/QApplication>
#include "mainwindow.h"
#include "userchat.h"
#include "loginform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginForm l;
    l.show();

    return a.exec();
}
