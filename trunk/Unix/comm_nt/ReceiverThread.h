#ifndef RECEIVERTHREAD_H
#define RECEIVERTHREAD_H
#include <QThread.h>
#include <QWaitCondition.h>
#include <QMutex.h>

class Client;
class ReceiverThread : public QThread
{
    Q_OBJECT

public:
    // pointer to communicator
    Client * Communicator;
    ReceiverThread(void * client);
    ~ReceiverThread();

signals:
    void MessageReceived();

protected:
    void run();
};

#endif // RECEIVERTHREAD_H
