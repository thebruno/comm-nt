#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userchat.h"
#include <windows.h>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    LogIn = new LoginForm(this );
    Communicator = 0;
    ui->listUsers->setSelectionMode(QAbstractItemView::MultiSelection );
}

void MainWindow::MessageReceived(){
    DoHandling();
}

MainWindow::~MainWindow()
{

    delete ui;
    delete LogIn;
    //delete Communicator;    
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
            connect(Communicator->QTReceiverThread,  SIGNAL(MessageReceived()), this, SLOT(MessageReceived()));
        } else
            return;
    }
}

void MainWindow::on_actionDisconnect_triggered()
{

}

void MainWindow::DoHandling(){  
    Message m;
    Communicator->NewInputMessage->Wait();
    Communicator->InputMsgsAccess->Wait();
    m = Communicator->InputMsgs.back();
    Communicator->InputMsgs.pop_back();
    Communicator->InputMsgsAccess->Release();
    //QMessageBox::information(this, QString("Message") , QString(m.ToString().c_str()), QMessageBox::Ok,QMessageBox::Close);
    switch (m.Type){
        case LOGIN: {
            // not ussed
            break;
         }
        case LOGOUT: {
            // not used
            break;
        }
        case RESULT: {
        // wynik logowania
            std::cout << "Result: " << m.ToString() << std::endl;
            Communicator->Me = m.Receiver;
            break;
        }
        case USERLIST: {
            std::cout << "User list: " << m.ToString() << std::endl;
            Communicator->DataAccess->Wait();
            Communicator->Users = m.InvolvedGroup.GroupMembers;
            HandleUserList();
            Communicator->DataAccess->Release();
            break;
        }
        case MESSAGE: {
                std::cout << "Message: " << m.ToString() << std::endl;
                break;
        }
        case GROUPMESSAGE: {
                std::cout << "Group Message: " << m.ToString() << std::endl;
                break;
        }
        default:
                break;
        }
    return;
}

void MainWindow::HandleUserList()
{   
    QIcon  icon = QIcon(QString("user.ico"));
    ui->listUsers->clear();
    std::list<User>::iterator i;
    for (i = Communicator->Users.begin(); i != Communicator->Users.end(); ++i) {
        new QListWidgetItem(icon, QString(i->Login.c_str()), ui->listUsers, 0 );
    }
    ui->listUsers->sortItems(Qt::AscendingOrder);
}
