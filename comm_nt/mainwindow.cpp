#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userchat.h"
#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&thread ,  SIGNAL(ReceivedMessage(const QString &)), this, SLOT(ReceivedMessage(const QString &)));
    LogIn = new LoginForm(this );
    Communicator = 0;
   // connect(&thread ,  SIGNAL(doit()), this, SLOT(doit()));
    //Communicator = new Client("localhost", 1986, true);

}

void MainWindow::ReceivedMessage(const QString & msg){
    this->ui->pushButton->setText(msg);

}



MainWindow::~MainWindow()
{

    delete ui;
    delete LogIn;
    //delete Communicator;
    thread.terminate();
}

void MainWindow::on_pushButton_2_clicked()
{
    std::string qs = "ola";
    QString q = QString(qs.c_str());

    q.toStdString();
    LogIn->show();
}

void MainWindow::on_pushButton_clicked()
{
    ChatWindows["a"] = new UserChat;
    ChatWindows["a"]->show();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionAbout_triggered()
{

}

void MainWindow::on_actionConnect_triggered()
{
    if (Communicator != 0) {
        delete Communicator;
        Communicator = 0;
    }

    if (Communicator == 0) {

        if (LogIn->exec() == QDialog::Accepted) {
            try {
                QMessageBox::warning(this, QString(LogIn->Host.c_str()), QString(""), QMessageBox::Ok,QMessageBox::Close);
                Communicator = new Client(LogIn->Host, LogIn->Port, true);

            } catch (SocketException s) {
                QMessageBox::warning(this, QString("Error"), QString("Cannoct connect to server!"), QMessageBox::Ok,QMessageBox::Close);
                return;
            }
            try {
                Communicator->LogIn(LogIn->UserLogin);

            } catch (SocketException s) {
                QMessageBox::warning(this, QString("Error"), QString("Cannoct login to server!"), QMessageBox::Ok,QMessageBox::Close);
                return;
            }
        } else
            return;
    }
}

void MainWindow::on_actionDisconnect_triggered()
{

}
