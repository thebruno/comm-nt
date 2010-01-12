#ifndef CLIENT_H
#define CLIENT_H
#include "stdafx.h"
#include "BasicSystemClasses.h"
#include "CommonClasses.h"
#include "Socket.h"
#include "ReceiverThread.h"

class MainWindow;
class ReceiverThread;

class Client {
	std::string Host;
	int Port;
	// clients IP/name
	std::string IP;
	//Communication Socket
	Socket * CommSocket;
        // currently logged user
	User Me;
	std::list<User> Users;
	std::list<Group> Groups;
	std::list<Message> InputMsgs;
	std::list<Message> OutputMsgs;
	std::list<Message> InputMsgsHistory;
	std::list<Message> OutputMsgsHistory;

        // a thread which sends messages
	SysThread * SenderThread;

        // to save critical sections
	SysSemaphore * DataAccess;
	SysSemaphore * InputMsgsAccess;
	SysSemaphore * OutputMsgsAccess;

	SysSemaphore * NewInputMessage;
	SysSemaphore * NewOutputMessage;
	bool IsLogged;
	bool IsConnected;
	// sender thread api funtion
        static void* SenderFunction(void *c);
public:
        bool IsUserLogged(User &u);
        ReceiverThread * QTReceiverThread;
	Client();
	~Client();
	Client(std::string host, int port, bool connect = false);
        // connect to and disconnet from server
	Result Connect(std::string host, int port);
	Result Disconnect();

        // log in and out
	Result LogIn(std::string login);
	Result LogOut();

        // send and receive messages
	Result Send(Message & m);
	void SendAsynchronously(Message & m);	
	void SendToUser(User recipient, std::string text);
	void SendToGroup(Group g, std::string text);
        Result Receive(Message &m);

        // send messages
	void DoSending();
	std::string PrintUsers();
        friend class MainWindow;
        friend class ReceiverThread;
};

#endif
