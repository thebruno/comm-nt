#include "loginform.h"
#include "ui_loginform.h"

LoginForm::LoginForm(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::LoginForm)
{
    m_ui->setupUi(this);
}

LoginForm::~LoginForm()
{
    delete m_ui;
}

void LoginForm::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
