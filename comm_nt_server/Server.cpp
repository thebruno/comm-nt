#include "stdafx.h"
#include "Server.h"



Server::Server(int port, int maxConnections)
{
	Start(port, maxConnections);
}
Server::~Server(){
	delete NewMessage;
	delete InputMsgsAccess;
	delete OutputMsgsAccess;
	delete DataAccess;
}

//ok
unsigned long __stdcall Server::ListenerFunction(void*s){
	Server * S = reinterpret_cast<Server*>(s);
	S->DoListening();
	return 0;
}

unsigned long __stdcall Server::ReceiverFunction(void*s){
	std::pair<Server *, Socket *> * SSpair = reinterpret_cast<std::pair<Server *, Socket *> *>(s);
	Server * S = SSpair->first;
	Socket * userSocket = SSpair->second;
	S->DoReceiving(userSocket);

	return 0;
}

//unsigned long __stdcall Server::SenderFunction(void*s){
//	Server * S = reinterpret_cast<Server*>(s);
//	S->DoSending();
//	return 0;
//}

unsigned long __stdcall Server::HandlerFunction(void*s){
	Server * S = (Server *)(s);
	S->DoHandling();
	return 0;
}

void Server::DoListening(){
	while (1) {
		Socket* s = ListenSocket->Accept();
		SysThread * temp = new SysThread(Server::ReceiverFunction, (void *) &std::pair<Server *, Socket *>(this, s));
		temp->Start();
		//ReceiverThreads.push_back(
	}
}

// dodaæ multiviadomoœci...
void Server::DoReceiving(Socket *userSocket){
	while (1){
		std::string received = "";
		Socket const * const temp = static_cast<Socket const * const>(userSocket);
		if (SelectSocket::CanRead(temp, true)) {
			Result temp = userSocket->ReceiveBytes(received, MESSAGE_DELIMITER);
			// odczytane wszystko?
			if (temp == Result::OK && received != "" ){
				Message m;
				m.Parse(received);
				//DataAccess -> Wait();
				// dodaj mapê <user, socket> przy pierwszym odczycie
				// czy wysy³aj¹æ userowi lisetê u¿ytkowników, umieszczaæ go tam?
				// raczej tak, ale jeœli from to ten sam user - nie wyœwietlaj u usera
				InputMsgsAccess->Wait();
				InputMsgs.push_back(m);
				InputMsgsAccess->Release();
				NewMessage->Release();
				std::cout << received << std::endl;
				received = "";
			}
		}
	}
}

//void Server::DoSending(){}

void Server::DoHandling(){
	while(1){
		NewMessage->Wait();
		InputMsgsAccess->Wait();
		// odwo³aj siê do socketa przy wysy³aniu
		// Sockets[User];
		Message m = InputMsgs.back();
		InputMsgs.pop_back();
		InputMsgsAccess->Release();
	}
}

Result Server::Send(Message m){
	return Result::OK;
}
Result Receive(Message &m){
	return Result::OK;
}

void Server::Start(int port, int maxConnections){
	ListenSocket = new ServerSocket(port, maxConnections);
	InputMsgsAccess = new SysSemaphore(1,1);
	DataAccess = new SysSemaphore(1,1);
	OutputMsgsAccess = new SysSemaphore(1,1);
	NewMessage = new SysSemaphore(0,MESSAGE_QUEUE_LENGTH);
	ListenerThread = new SysThread(Server::ListenerFunction, this);
	HandlerThread = new SysThread(Server::HandlerFunction,  &*this);

	HandlerThread->Start();
	ListenerThread->Start();
	//CreateThread(0, 0, Server::ListenerFunction, this, 0, 0);
}

void Server::Stop(){
	delete ListenSocket;
	ListenerThread->Terminate();
	delete ListenerThread;
	std::list<SysThread *>::iterator it;
	for(it = ReceiverThreads.begin(); it != ReceiverThreads.end(); it++)
	{
		(*it)->Terminate();
		delete *it;
	}
	//SenderThread->Terminate();
	//delete SenderThread;
	HandlerThread->Terminate();
	delete HandlerThread;
}