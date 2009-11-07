#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QMessageBox>
#include "chat.h"
#include "Client.h"
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
    LoginForm * LogIn;

    Ui::MainWindow *ui;
    void CloseAllWindows();
    bool ChatWindowEsists(Group & g);
    bool ChatWindowEsists(User & u);
    void SendMsgFromGUI(Group receivers, std::string msg);
    User WhoAmI();
public slots:
    void MessageReceived();
private:
    void DoHandling();
    std::map<std::string, Chat *> ChatWindows;    
    void HandleUserList();
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
