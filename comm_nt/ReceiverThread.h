#ifndef RECEIVERTHREAD_H
#define RECEIVERTHREAD_H
#include <QThread.h>
#include <QWaitCondition.h>
#include <QMutex.h>
class ReceiverThread : public QThread
{
    Q_OBJECT

public:
    ReceiverThread();
    ~ReceiverThread();


signals:
    void ReceivedMessage(const QString &);


protected:
    void run();

private:

    QWaitCondition newMessage;
    QMutex mutex;
};

#endif // RECEIVERTHREAD_H
