#ifndef CHAT_H
#define CHAT_H

#include <QtGui/QMainWindow>
#include "CommonClasses.h"

namespace Ui {
    class Chat;
}
class MainWindow;

class Chat : public QMainWindow {
    Q_OBJECT
public:
    Chat(Group & receivers, QWidget *parent = 0);
    ~Chat();
    // reference to parent window
    MainWindow * Main;
    // fill message history chat with message from other user
    void FillMessage(Message& m);
    // fill message history chat with current user message
    void FillMessage(std::string sender, std::string created, std::string text);
    // get this window chat recivers
    Group GetReceivers();
    void showAndPosition(QWidget * parent);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::Chat *m_ui;
    Group Receivers;
private slots:
    void on_btnClose_clicked();
    void on_btnSend_clicked();
};

#endif // CHAT_H
