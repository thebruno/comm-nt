#ifndef RECEIVERTHREAD_H
#define RECEIVERTHREAD_H
#include <qthread.h>
#include <qwaitcondition.h>
#include <qmutex.h>

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
