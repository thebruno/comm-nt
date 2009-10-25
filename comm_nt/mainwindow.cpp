#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userchat.h"
#include <windows.h>

class Foo : public QObject
    {
        Q_OBJECT
    public:
        Foo(){}
        int value() const { return val; }
    public slots:
        void setValue( int v) {
            if ( v != val ) {
                val = v;
                emit valueChanged(v);
            }
        }

    signals:
        void valueChanged( int );
    private:
        int val;
    };


DWORD WINAPI MyThreadFunction( LPVOID lpParam )
{
    static int a = 0;    
    char buffer [10];
    a++;
    itoa(a, buffer,10);
    MainWindow * window = reinterpret_cast<MainWindow*>(lpParam);
    window->ui->pushButton->setText(QString(buffer));
    UserChat u;
    //window->u[0].setWindowTitle("ala");
    //window->u[1].close();
    return 0;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&thread ,  SIGNAL(ReceivedMessage(const QString &)), this, SLOT(ReceivedMessage(const QString &)));
   // connect(&thread ,  SIGNAL(doit()), this, SLOT(doit()));

}

void MainWindow::ReceivedMessage(const QString & msg){
    this->ui->pushButton->setText(msg);

}



MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    //CreateThread(0, 0, MyThreadFunction, this ,0, 0);

    std::string qs = "ola";
    QString q = QString(qs.c_str());

    q.toStdString();




}

void MainWindow::on_pushButton_clicked()
{
    u = new UserChat[5];
    u[0].show();
    u[1].show();
}
