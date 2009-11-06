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
    if (Communicator != 0)
        delete Communicator;
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

        if (LogIn->execExtended() == QDialog::Accepted) {
            try {                
                Communicator = new Client(LogIn->Host, LogIn->Port, true);

            } catch (SocketException s) {
                QMessageBox::warning(this, QString("Error"), QString("Cannoct connect to server!"), QMessageBox::Ok,QMessageBox::Close);
                return;
            }
            try {
                Communicator->LogIn(LogIn->UserLogin);

            } catch (SocketException s) {
                QMessageBox::warning(this, QString("Error"), QString("Cannoct login to server!"), QMessageBox::Ok, QMessageBox::NoButton);
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
            switch (m.PreviousOperation){
                case LOGIN: {
                    if (m.PreviousResult == OK) {
                        Communicator->DataAccess->Wait();
                        Communicator->Me = m.Receiver;
                        Communicator->IsLogged = true;
                        Communicator->DataAccess->Release();

                    } else if(m.PreviousResult == FAILED) {
                        ; //actually nothing to do
                    }
                    QMessageBox::information(this, QString("Login Status"), QString(m.PreviusOperationInfo.c_str()), QMessageBox::Ok, QMessageBox::NoButton);
                    break;
                }
            }
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

// odpalaæ w semaforach
void MainWindow::HandleUserList()
{   
    QIcon  icon = QIcon(QString(":/icons/user.ico"));
    ui->listUsers->clear();
    std::list<User>::iterator it;
    Communicator->Users.sort();
    int i = 0;
    for (it = Communicator->Users.begin(); it != Communicator->Users.end(); ++it, i++) {
        new QListWidgetItem(icon, QString(it->Login.c_str()), ui->listUsers, i);
    }
}

void MainWindow::on_btnStartChat_clicked()
{    
    if (Communicator != 0 && Communicator->IsConnected && Communicator->IsLogged) {
        Communicator->DataAccess->Wait();
        QList<QListWidgetItem*> temp = ui->listUsers->selectedItems();
        Group selectedUsers;
        QList<QListWidgetItem*>::iterator it;
        std::list<User>::iterator uit;
        int i;
        QListWidgetItem * item;
        for (it = temp.begin(); it != temp.end(); ++it) {
            for (i = 0, uit = Communicator->Users.begin(); i < (*it)->type(); ++i, ++uit) {
                ;
            }
            selectedUsers.GroupMembers.push_back(*uit);
        }
        //if (ChatWindows.
        ChatWindows["a"] = new UserChat;
        ChatWindows["a"]->show();
        Communicator->DataAccess->Release();
    }
}
