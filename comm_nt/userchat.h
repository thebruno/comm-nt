#ifndef USERCHAT_H
#define USERCHAT_H

#include <QtGui/QWidget>

namespace Ui {
    class UserChat;
}

class UserChat : public QWidget {
    Q_OBJECT
public:
    UserChat(QWidget *parent = 0);
    ~UserChat();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::UserChat *m_ui;

private slots:
    void on_btnClose_clicked();
    void on_btnSend_clicked();
};

#endif // USERCHAT_H
