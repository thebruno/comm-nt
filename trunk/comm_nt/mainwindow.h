#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QMessageBox>
#include "userchat.h"
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
public slots:
    void MessageReceived();
private:
    void DoHandling();
    std::map<std::string, UserChat *> ChatWindows;
    Client * Communicator;
    void HandleUserList();
private slots:
    void on_actionDisconnect_triggered();
    void on_actionConnect_triggered();
    void on_actionAbout_triggered();
    void on_actionExit_triggered();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H
