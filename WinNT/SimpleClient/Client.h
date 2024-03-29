#ifndef CLIENT_H
#define CLIENT_H
#include "stdafx.h"
#include "BasicSystemClasses.h"
#include "CommonClasses.h"
#include "Socket.h"

class Client {
	std::string Host;
	int Port;
	// clients IP/name
	std::string IP;
	//Communication Socket
	Socket * CommSocket;
	User Me;
	std::list<User> Users;
	std::list<Group> Groups;
	std::list<Message> InputMsgs;
	std::list<Message> OutputMsgs;
	std::list<Message> InputMsgsHistory;
	std::list<Message> OutputMsgsHistory;

	SysThread * ReceiverThread;
	SysThread * SenderThread;
	SysThread * HandlerThread;

	SysSemaphore * DataAccess;
	SysSemaphore * InputMsgsAccess;
	SysSemaphore * OutputMsgsAccess;
	//SysSemaphore * InputMsgsHistoryAccess;
	//SysSemaphore * OutputMsgsHistoryAccess;

	SysSemaphore * NewInputMessage;
	SysSemaphore * NewOutputMessage;
	bool IsLogged;
	bool IsConnected;

	// sender thread api funtion
	static unsigned long __stdcall SenderFunction(void *c);
	// reciever thread api funtion
	static unsigned long __stdcall ReceiverFunction(void *c);
	// handler thread api funtion
	static unsigned long __stdcall HandlerFunction(void *c);

public:
	

	Client();
	~Client();
	Client(std::string host, int port, bool connect = false);

	Result Connect(std::string host, int port);
	Result Disconnect();

	Result LogIn(std::string login);
	Result LogOut();

	Result Send(Message & m);
	void SendAsynchronously(Message & m);
	Result Receive(Message &m);
	void SendToUser(User recipient, std::string text);
	void SendToGroup(Group g, std::string text);

	void DoHandling();
	void DoReceiving();
	void DoSending();
	std::string PrintUsers();

};

#endif