#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QMessageBox>
#include "chat.h"
#include "client.h"
#include "loginform.h"


namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    // login form
    LoginForm * LogIn;

    Ui::MainWindow *ui;
    // close all open chats
    void CloseAllWindows();
    // check if window with chat group g exists
    bool ChatWindowEsists(Group & g);
    // check if window for user u exists
    bool ChatWindowEsists(User & u);
    // send message to user
    void SendMsgFromGUI(Group receivers, std::string msg);
    // returns a currently logged user
    User WhoAmI();
public slots:
    // fired when new message comes
    void MessageReceived();
private:
    // handles all received messages
    void DoHandling();
    // opened user chat
    std::map<std::string, Chat *> ChatWindows;    
    // display user list in main window
    void HandleUserList();
    // closes windows if any of user logs out
    void HandleUserWindows();
    Client * Communicator;
private slots:
    void on_btnStartChat_clicked();
    void on_actionDisconnect_triggered();
    void on_actionConnect_triggered();
    void on_actionAbout_triggered();
    void on_actionExit_triggered();
};

#endif // MAINWINDOW_H
