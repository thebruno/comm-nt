#ifndef CLIENT_H
#define CLIENT_H
#include "stdafx.h"
#include "BasicSystemClasses.h"
#include "CommonClasses.h"
#include "Socket.h"

class Client {
	Socket * Socket;
	User Me;
	std::list<User> Users;
	std::list<Group> Groups;
	std::list<Message> InputMsgs;
	std::list<Message> OutputMsgs;
	std::list<Message> InputMsgsHistory;
	std::list<Message> OutputMsgsHistory;

	SysThread * ReceiverThread;
	SysThread * SenderThread;

	SysSemaphore * DataAccess;
	SysSemaphore * InputMsgsAccess;
	SysSemaphore * OutputMsgsAccess;
	SysSemaphore * InputMsgsHistoryAccess;
	SysSemaphore * OutputMsgsHistoryAccess;

	SysSemaphore * NewInputMessage;
	SysSemaphore * NewOutputMessage;
	bool Connected;
	bool Logged;

	// sender thread api funtion
	static unsigned long __stdcall SenderFunction(void *c);
	// reciever thread api funtion
	static unsigned long __stdcall ReceiverFunction(void *c);
	// handler thread api funtion
	static unsigned long __stdcall HandlerFunction(void *c);

public:
	

	Client();
	~Client();
	Client(std::string host, int port);

	Result LogIn(User u);
	Result LogOut();

	Result Send(Message & m);
	Result Receive(Message &m);

	void DoHandling();
	void DoReceiving();
	void DoSending();
};

#endif