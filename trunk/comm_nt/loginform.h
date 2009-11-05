#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QtGui/QDialog>

namespace Ui {
    class LoginForm;
}

class LoginForm : public QDialog {
    Q_OBJECT
public:
    LoginForm(QWidget *parent = 0);
    ~LoginForm();
    std::string UserLogin;
    std::string Host;
    int Port;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::LoginForm *m_ui;

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // LOGINFORM_H
