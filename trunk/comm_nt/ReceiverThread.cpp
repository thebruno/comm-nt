#include "ReceiverThread.h"
#include "Client.h"
ReceiverThread::ReceiverThread(void * client){
    Communicator = static_cast<Client *> (client);
}
ReceiverThread::~ReceiverThread(){
}

void ReceiverThread::run(){
    while (1){
    // do receiving
        if (SelectSocket::CanRead(Communicator->CommSocket, true)) {
            Message m = Message();
            Result result = Communicator->Receive(m);
            if (result != OK) {
                std::cout << " koniec 1" <<std::endl;
                Communicator->InputMsgsAccess->Wait();
                Communicator->InputMsgs.push_back(Message(LOGOUT, DateTimeNow(), User(), User(), Group(), ""));
                Communicator->InputMsgsAccess->Release();
                Communicator->NewInputMessage->Release();
                emit MessageReceived();
                break;
            }
            std::cout << "Received: " << m.ToString() << " from user: " << m.Sender.ToString() << std::endl;
            Communicator->InputMsgsAccess->Wait();
            Communicator->InputMsgs.push_back(m);
            Communicator->InputMsgsAccess->Release();
            Communicator->NewInputMessage->Release();
            emit MessageReceived();
            if (m.Type == LOGOUT) {
                std::cout << "Log Out" << std::endl;
                return;
            }
        }
    }
    return;
}

