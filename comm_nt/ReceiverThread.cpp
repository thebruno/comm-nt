#include "ReceiverThread.h"

ReceiverThread::ReceiverThread(){
    start();
}
ReceiverThread::~ReceiverThread(){
    {
    //QMutexLocker locker(&mutex);
    newMessage.wakeOne();
    }
    wait();
}

void ReceiverThread::run(){
    while(1) {
        msleep(2000);
        {
         //   QMutexLocker locker(&mutex);

        }
        emit ReceivedMessage(QString("test"));
        //emit doit();
        msleep(5000);
    }
}

