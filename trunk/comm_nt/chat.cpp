#include "chat.h"
#include "ui_chat.h"
#include "mainwindow.h"

Chat::Chat(Group &receivers, QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::Chat)
{
    m_ui->setupUi(this);
    Receivers = receivers;
    Main = static_cast<MainWindow*>(parent);
}

Chat::~Chat()
{
    delete m_ui;
}

void Chat::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void Chat::on_btnSend_clicked()
{
    //TODO: dodaæ sprawdzenie po³¹czenia (Communicator not null, connected and logged, semafor wzi¹æ
    FillMessage(Main->WhoAmI().Login, DateTimeNow(),  m_ui->txtPlainChat->toPlainText().toStdString());
    Main->SendMsgFromGUI(Receivers, m_ui->txtPlainChat->toPlainText().toStdString());
    m_ui->txtPlainChat->clear();
}

void Chat::on_btnClose_clicked()
{
    this->hide();
}

void Chat::FillMessage(Message &m) {
    std::string temp;
    temp.append("<B><FONT COLOR=\"BLUE\">").append(m.Sender.Login).append(": ").append(m.Created).append("</FONT></B>");
    m_ui->txtChatHistory->append(QString(temp.c_str()));
    m_ui->txtChatHistory->append(QString(m.Text.c_str()));
}

void Chat::FillMessage(std::string sender, std::string created, std::string text) {
    std::string temp;
    temp.append("<B><FONT COLOR=\"BLACK\">").append(sender).append(": ").append(created).append("</FONT></B>");
    m_ui->txtChatHistory->append(QString(temp.c_str()));
    m_ui->txtChatHistory->append(QString(text.c_str()));
}
