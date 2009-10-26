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

protected:
    void changeEvent(QEvent *e);

private:
    Ui::LoginForm *m_ui;
};

#endif // LOGINFORM_H
