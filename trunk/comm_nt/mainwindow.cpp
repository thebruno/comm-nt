#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userchat.h"
#include <windows.h>

DWORD WINAPI MyThreadFunction( LPVOID lpParam )
{
    static int a = 0;    
    char buffer [10];
    a++;
    itoa(a, buffer,10);
    MainWindow * window = reinterpret_cast<MainWindow*>(lpParam);
    window->ui->pushButton->setText(QString(buffer));
    UserChat u;
    //window->u[0].setWindowTitle("ala");
    //window->u[1].close();
    return 0;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    CreateThread(0, 0, MyThreadFunction, this ,0, 0);

    std::string qs = "ola";
    QString q = QString(qs.c_str());

    q.toStdString();

    this->ui->pushButton->setText(q);

}

void MainWindow::on_pushButton_clicked()
{
    u = new UserChat[5];
    u[0].show();
    u[1].show();
}
