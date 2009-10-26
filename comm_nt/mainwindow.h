#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <userchat.h>
#include "ReceiverThread.h"
#include "Client.h"

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

    UserChat * u;
    Ui::MainWindow *ui;
public slots:
    void ReceivedMessage(const QString & msg);
private:
    Client * Communicator;
    ReceiverThread thread;
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H
