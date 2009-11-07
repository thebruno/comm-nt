#include <QtGui/QApplication>
#include <QTextCodec.h>
#include <QDesktopWidget.h>
#include "mainwindow.h"
#include "chat.h"
#include "loginform.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("ISO8859-2"));
    QApplication a(argc, argv);
    QDesktopWidget * desktop = a.desktop();
    MainWindow Main;    
    Main.setGeometry(desktop->width() / 2 - Main.width() / 2, desktop->height() / 2 - Main.height() / 2, 0, 0);
    Main.show();
    return a.exec();
}
