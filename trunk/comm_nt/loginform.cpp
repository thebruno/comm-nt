#include "loginform.h"
#include "ui_loginform.h"
#include <QMessageBox>

LoginForm::LoginForm(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::LoginForm)
{
    m_ui->setupUi(this);
    m_ui->cmBoxServer->addItem(QString("127.0.0.0"));
    m_ui->cmBoxServer->addItem(QString("192.168.0.1"));
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

void LoginForm::on_pushButton_clicked()
{
    std::string Message;
    if (m_ui->txtBoxLogin->text().toStdString().size() == 0)
        Message += "Enter your login\n";
    UserLogin = m_ui->txtBoxLogin->text().toStdString();
    if (m_ui->txtBoxPort->text().toStdString().size() == 0)
        Message += "\nEnter port";

    Port = atoi(m_ui->txtBoxPort->text().toStdString().c_str());
    if (Port == 0)
        Message += "\nEnter port value correctly.";

    if (Message.size() != 0) {
        QMessageBox::warning(this, QString("Error"), QString(Message.c_str()), QMessageBox::Ok,QMessageBox::Close);
        return;
    }
    Host = m_ui->cmBoxServer->itemText(m_ui->cmBoxServer->currentIndex()).toStdString();
    this->hide();
    this->accept();
}

void LoginForm::on_pushButton_2_clicked()
{
    this->hide();
    this->reject();
}
