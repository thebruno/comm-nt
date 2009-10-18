#include "userchat.h"
#include "ui_userchat.h"

UserChat::UserChat(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::UserChat)
{
    m_ui->setupUi(this);
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
