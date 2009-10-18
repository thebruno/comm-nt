#include <QtGui/QApplication>
#include "mainwindow.h"
#include "userchat.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    UserChat u;
    u.show();

    w.show();
    return a.exec();
}
