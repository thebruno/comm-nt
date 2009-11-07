#include <QtGui/QApplication>
#include <QTextCodec.h>
#include "mainwindow.h"
#include "chat.h"
#include "loginform.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("ISO8859-2"));
    QApplication a(argc, argv);
    MainWindow Main;
    Main.show();
    return a.exec();
}
