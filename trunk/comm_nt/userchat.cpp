#include "userchat.h"
#include "ui_userchat.h"
#include "mainwindow.h"

UserChat::UserChat(QWidget *parent, QWidget *main, Group &receivers) :
    QWidget(parent),
    m_ui(new Ui::UserChat)
{
    m_ui->setupUi(this);
    Receivers = receivers;
    Main = static_cast<MainWindow*>(main);
}

UserChat::~UserChat()
{
    delete m_ui;
}

void UserChat::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void UserChat::on_btnSend_clicked()
{
    Main->SendMsgFromGUI(Receivers, m_ui->txtPlainChat->toPlainText().toStdString());
}

void UserChat::on_btnClose_clicked()
{
    this->hide();
}

void UserChat::FillMessage(Message &m) {
    m_ui->txtChatHistory->append("\n");
    m_ui->txtChatHistory->append(QString(m.Text.c_str()));
}
