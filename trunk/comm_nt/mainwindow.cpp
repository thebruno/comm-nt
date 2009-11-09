#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chat.h"
#include <windows.h>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    LogIn = new LoginForm(this );
    Communicator = 0;
    ui->listUsers->setSelectionMode(QAbstractItemView::MultiSelection );
    HandleUserList();
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
    on_actionDisconnect_triggered();
    this->close();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, QString("About..."), QString("Communicator 1.0 for Windows NT.\nWritten by Konrad Ba³ys inf9, OS2, POLSL PL."), QMessageBox::Ok, QMessageBox::NoButton);
}

void MainWindow::on_actionConnect_triggered()
{
    on_actionDisconnect_triggered();

    if (Communicator == 0) {

        if (LogIn->execExtended() == QDialog::Accepted) {
            try {                
                Communicator = new Client(LogIn->Host, LogIn->Port, true);

            } catch (SocketException s) {
                QMessageBox::warning(this, QString("Error"), QString("Cannot connect to server!"), QMessageBox::Ok,QMessageBox::NoButton);
                return;
            }
            try {
                Communicator->LogIn(LogIn->UserLogin);

            } catch (SocketException s) {
                QMessageBox::warning(this, QString("Error"), QString("Cannot login to server!"), QMessageBox::Ok, QMessageBox::NoButton);
                return;
            }
            connect(Communicator->QTReceiverThread,  SIGNAL(MessageReceived()), this, SLOT(MessageReceived()));
        } else
            return;
    }
}

void MainWindow::on_actionDisconnect_triggered()
{
    if (Communicator != 0) {
        Communicator->LogOut();
        Communicator->Disconnect();
        delete Communicator;
        Communicator = 0;
        HandleUserList();
        HandleUserWindows();
    }
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
            // disconnected by server
            on_actionDisconnect_triggered();
            QMessageBox::warning(this, QString("ERROR") , QString("Connection to server lost."), QMessageBox::Ok,QMessageBox::NoButton);
            break;
        }
        case RESULT: {
        // wynik logowania
            std::cout << "Result: " << m.ToString() << std::endl;
            switch (m.PreviousOperation){
                case LOGIN: {
                    //QMessageBox::information(this, QString("Result"), QString(), QMessageBox::Ok, QMessageBox::NoButton);
                    if (m.PreviousResult == OK) {
                        Communicator->DataAccess->Wait();
                        Communicator->Me = m.Receiver;                        
                        Communicator->IsLogged = true;
                        Communicator->DataAccess->Release();
                        HandleUserList();
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
            HandleUserWindows();
            //TODO: close windows now not used
            Communicator->DataAccess->Release();
            //QMessageBox::information(this, QString("UserList"), QString(), QMessageBox::Ok, QMessageBox::NoButton);
            break;
        }
        case MESSAGE: {
                // message from other user for me
                // check if windw exists
                // if not create it
                // activate it and fill with message

                Communicator->DataAccess->Wait();
                if (!ChatWindowEsists(m.Sender)) {
                    Group g;
                    g.GroupMembers.push_back(m.Sender);
                    ChatWindows[m.Sender.ToString()] = new Chat(g, this);

                }
                ChatWindows[m.Sender.ToString()]->FillMessage(m);
                ChatWindows[m.Sender.ToString()]->showAndPosition(this);
                ChatWindows[m.Sender.ToString()]->raise();
                ChatWindows[m.Sender.ToString()]->activateWindow();
                Communicator->DataAccess->Release();
                std::cout << "Message: " << m.ToString() << std::endl;
                break;
        }
        case GROUPMESSAGE: {
                // message from other users to me (check if me is in receiver group)
                // check if windw exists
                // if not create it
                // activate it and fill with message
                 Communicator->DataAccess->Wait();
                if (!ChatWindowEsists(m.InvolvedGroup)) {
                    m.InvolvedGroup.GroupMembers.sort();
                    ChatWindows[m.InvolvedGroup.ToString()] = new Chat(m.InvolvedGroup, this);

                }
                ChatWindows[m.InvolvedGroup.ToString()]->FillMessage(m);
                ChatWindows[m.InvolvedGroup.ToString()]->showAndPosition(this);
                ChatWindows[m.InvolvedGroup.ToString()]->raise();
                ChatWindows[m.InvolvedGroup.ToString()]->activateWindow();
                Communicator->DataAccess->Release();
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
    if (Communicator != 0 && Communicator->IsConnected && Communicator->IsLogged) {
        QIcon  icon = QIcon(QString(":/icons/user.ico"));
        ui->listUsers->clear();
        std::list<User>::iterator it;
        int i = 0;
        for (it = Communicator->Users.begin(); it != Communicator->Users.end(); ++it, i++) {
            new QListWidgetItem(icon, QString(it->Login.c_str()), ui->listUsers, i);
        }
        ui->lblStatus->setText(QString("Currently logged as: ").append(QString(Communicator->Me.Login.c_str())));
    }
    else {
        ui->listUsers->clear();
        ui->lblStatus->setText((QString("You are not logged.")));
    }
}

void MainWindow::HandleUserWindows (){
    if (Communicator == 0 || !Communicator->IsConnected || !Communicator->IsLogged) {
        // close all windows
        CloseAllWindows();
    } else {
        std::map<std::string, Chat *>::iterator it;
        std::list<User>::iterator uit;
        bool found = false;
        for (it = ChatWindows.begin(); it != ChatWindows.end(); ) {
            found = false;
            Group temp = (*it).second->GetReceivers();
            for (uit = temp.GroupMembers.begin(); uit != temp.GroupMembers.end(); ++uit) {
                // remove windows, from which any user disappeared
                if (!Communicator->IsUserLogged(*uit)){
                    //(*it).second->close();
                    delete (*it).second;
                    ChatWindows.erase(it++);
                    // iterator has been invalidated
                    found = true;
                    break;
                }
            }
            if (!found)
                ++it;
        }
    }
}

void MainWindow::on_btnStartChat_clicked()
{    
    if (Communicator != 0 && Communicator->IsConnected && Communicator->IsLogged) {
        Communicator->DataAccess->Wait();
        QList<QListWidgetItem*> temp = ui->listUsers->selectedItems();
        if (temp.size() == 0) {
            QMessageBox::information(this, QString("Information"), QString("Please select at least one user to chat with."), QMessageBox::Ok,QMessageBox::NoButton);
            return;
        }
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
        for (uit = selectedUsers.GroupMembers.begin(); uit != selectedUsers.GroupMembers.end(); ++uit) {
            if (*uit == Communicator->Me) {
                Communicator->DataAccess->Release();
                QMessageBox::warning(this, QString("Error"), QString("You cannot chat with yourself!"), QMessageBox::Ok,QMessageBox::NoButton);
                return;
            }
        }
        // add me for involved group
        selectedUsers.GroupMembers.push_back(WhoAmI());
        selectedUsers.GroupMembers.sort();
        if (!ChatWindowEsists(selectedUsers)) {
            ChatWindows[selectedUsers.ToString()] = new Chat(selectedUsers, this );
        }

        ChatWindows[selectedUsers.ToString()]->showAndPosition(this);
        ChatWindows[selectedUsers.ToString()]->raise();
        ChatWindows[selectedUsers.ToString()]->activateWindow();
        Communicator->DataAccess->Release();
    }
}

void MainWindow::CloseAllWindows(){
    std::map<std::string, Chat *>::iterator it;
    for (it = ChatWindows.begin(); it != ChatWindows.end(); ) {
        (*it).second->close();
        delete (*it).second;
        ChatWindows.erase(it++);
    }
}
bool MainWindow::ChatWindowEsists(Group & g){
    std::map<std::string, Chat *>::iterator it;
    std::string key = g.ToString();
    for (it = ChatWindows.begin(); it != ChatWindows.end(); ++it) {
        if ((*it).first == key)
            return true;
    }
    return false;
}


bool MainWindow::ChatWindowEsists(User & u){
    std::map<std::string, Chat *>::iterator it;
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
        else if (receivers.GroupMembers.size() > 1){
            //TODO: check if this is right
            Communicator->SendToGroup(receivers.GroupMembers, msg);
        }
    }
}
User MainWindow::WhoAmI(){
    if (Communicator != 0 && Communicator->IsConnected && Communicator->IsLogged)
        return Communicator->Me;
    else
        return User();
}
