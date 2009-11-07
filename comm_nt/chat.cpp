#include "chat.h"
#include "ui_chat.h"
#include "mainwindow.h"

Chat::Chat(Group &receivers, QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::Chat)
{
    m_ui->setupUi(this);
    Receivers = receivers;
    std::list<User>::iterator it;
    std::string title = "Chat members: ";
    for (it = Receivers.GroupMembers.begin(); it != Receivers.GroupMembers.end(); ++it) {
        title.append((*it).Login).append(", ");

    }
    title = title.substr(0, title.size() - 2);
    m_ui->grBoxChatWith->setTitle(QString(title.c_str()));
    Main = static_cast<MainWindow*>(parent);    
    m_ui->txtPlainChat->setFocus();
    //this->setAttribute(Qt::WA_DeleteOnClose, true);
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
    if (m_ui->txtPlainChat->toPlainText().toStdString().size() != 0){
        //TODO: dodaæ sprawdzenie po³¹czenia (Communicator not null, connected and logged, semafor wzi¹æ
        FillMessage(Main->WhoAmI().Login, DateTimeNow(),  m_ui->txtPlainChat->toPlainText().toStdString());
        Main->SendMsgFromGUI(Receivers, m_ui->txtPlainChat->toPlainText().toStdString());
        m_ui->txtPlainChat->clear();
    }
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


Group Chat::GetReceivers(){
    return Receivers;
}
void Chat::showAndPosition(QWidget * parent) {
    QRect r = parent->geometry();
    if (!this->isVisible()) {
        int centerX = r.left() + r.width() / 2;
        int centerY = r.top() + r.height() / 2;
        this->setGeometry(centerX - this->width()/2, centerY - this->height() / 2, 0, 0);
    }
    this->show();
}
