#ifndef USERCHAT_H
#define USERCHAT_H

#include <QtGui/QWidget>
#include "CommonClasses.h"

namespace Ui {
    class UserChat;
}
class MainWindow;

class UserChat : public QWidget {
    Q_OBJECT
public:
    UserChat(QWidget *parent, QWidget *main, Group & receivers);
    ~UserChat();
    MainWindow * Main;
    void FillMessage(Message& m);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::UserChat *m_ui;
    Group Receivers;

private slots:
    void on_btnClose_clicked();
    void on_btnSend_clicked();
};

#endif // USERCHAT_H
