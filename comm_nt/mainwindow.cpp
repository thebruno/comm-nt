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
    //CloseAllWindows();
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
                    default: {
                        QMessageBox::warning(this, QString("Error"), QString("Previous operation type not known."), QMessageBox::Ok, QMessageBox::NoButton);
                        break;
                    }
            }
            break;
        }
        case USERLIST: {
            std::cout << "User list: " << m.ToString() << std::endl;
            Communicator->DataAccess->Wait();
            Communicator->Users = m.InvolvedGroup.GroupMembers;
            Communicator->Users.sort();
            HandleUserList();
            //TODO: close windows now not used
            //HandleUserWindows();
            Communicator->DataAccess->Release();
            break;
        }
        case MESSAGE: {
                // message from other user for me
                // check if windw exists
                // if not create it
                // activate it and fill with message

                Communicator->DataAccess->Wait();
                if (!ChatWindowEsists(m.Receiver)) {
                    Group g;
                    g.GroupMembers.push_back(m.Receiver);
                    ChatWindows[m.Receiver.ToString()] = new UserChat(0, this, g);

                }
                ChatWindows[m.Receiver.ToString()]->FillMessage(m);
                ChatWindows[m.Receiver.ToString()]->show();
                ChatWindows[m.Receiver.ToString()]->raise();
                ChatWindows[m.Receiver.ToString()]->activateWindow();
                Communicator->DataAccess->Release();
                std::cout << "Message: " << m.ToString() << std::endl;
                break;
        }
        case GROUPMESSAGE: {
                // message from other users to me (check if me is in receiver group)
                // check if windw exists
                // if not create it
                // activate it and fill with message
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
        for (it = temp.begin(); it != temp.end(); ++it) {
            for (i = 0, uit = Communicator->Users.begin(); i < (*it)->type(); ++i, ++uit) {
                ;
            }
            selectedUsers.GroupMembers.push_back(*uit);
        }
        if (!ChatWindowEsists(selectedUsers)) {

            ChatWindows[selectedUsers.ToString()] = new UserChat(0, this, selectedUsers);

        }
        ChatWindows[selectedUsers.ToString()]->show();
        ChatWindows[selectedUsers.ToString()]->raise();
        ChatWindows[selectedUsers.ToString()]->activateWindow();
        Communicator->DataAccess->Release();
    }
}

void MainWindow::CloseAllWindows(){
    std::map<std::string, UserChat *>::iterator it;
    for (it = ChatWindows.begin(); it != ChatWindows.end(); ++it) {
        (*it).second->close();
        //delete (*it).second;
        ChatWindows.erase(it);
    }
}
bool MainWindow::ChatWindowEsists(Group & g){
    std::map<std::string, UserChat *>::iterator it;
    std::string key = g.ToString();
    for (it = ChatWindows.begin(); it != ChatWindows.end(); ++it) {
        if ((*it).first == key)
            return true;
    }
    return false;
}


bool MainWindow::ChatWindowEsists(User & u){
    std::map<std::string, UserChat *>::iterator it;
    std::string key = u.ToString();
    for (it = ChatWindows.begin(); it != ChatWindows.end(); ++it) {
        if ((*it).first == key)
            return true;
    }
    return false;
}

void MainWindow::SendMsgFromGUI(Group receivers, std::string msg){
    // TODO: add filtering - do not send message to yourself
    if (Communicator != 0 && Communicator->IsConnected && Communicator->IsLogged) {
        if (receivers.GroupMembers.size() == 1)
            Communicator->SendToUser(receivers.GroupMembers.front(), msg);
        else if (receivers.GroupMembers.size() > 1)
            Communicator->SendToGroup(receivers.GroupMembers, msg);
    }
}
