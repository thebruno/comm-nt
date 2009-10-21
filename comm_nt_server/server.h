#ifndef SERVER_H
#define SERVER_H
#include "stdafx.h"
#include "BasicSystemClasses.h"
#include "CommonClasses.h"
#include "Socket.h"

//TODO:
// po kilku minutach nieu�ywania - trzeba wywali� user�w
// doda� komunikat - disconnected?
class Server {
private:
	int Port;
	int MaxConnections;
	bool Started;
	ServerSocket* ListenSocket;
	// sockets indexed by users
	std::map<std::string, Socket*> Sockets;
	std::list<User> Users;
	std::list<Group> Groups;
	// buffer from all users
	std::list<Message> InputMsgs;
	// buffer for all users
	std::list<Message> OutputMsgs;
	SysSemaphore * DataAccess; //for Sockets, Users and Groupes
	SysSemaphore * InputMsgsAccess; // input buffer
	SysSemaphore * OutputMsgsAccess; // output buffer
	SysSemaphore * NewMessage;

	
	static unsigned long __stdcall ListenerFunction(void*s);
	static unsigned long __stdcall ReceiverFunction(void*s);
	//static unsigned long __stdcall SenderFunction(void*s);
	static unsigned long __stdcall HandlerFunction(void*s);
	
	//static unsigned long (__stdcall *Handler)(void*s);
	
	SysThread *ListenerThread;
	// doda� czyszenie w destruktorze
	std::map<std::string, SysThread *> ReceiverThreads;
	std::map <Socket *, SysThread *> UnverifiedReceiverThreads;
	//SysThread *SenderThread;
	SysThread *HandlerThread;
	std::list<SysThread*> ThreadsToDelete;
	void DeleteNotUsedThreads();
	void HandleMessage(Message* message);

	//methods - not used if static is enough
	User HandleClientConnection(Socket* s);
	bool IsUserLogged(User &u);
	bool IsGroupCreated(Group &g);
	void RemoveUser(User &u);
	void RemoveGroup(Group &g);
	void AddUser(User &u, Socket *s);
	void AddGroup(Group &g);
	void MapThreadToUser(User & u);

public:
	Server(int port, int maxConnections = 10, bool start = true);
	~Server();
	Result SendMessages();
	Result Send(Message& m);
	Result Receive(Message &m);
	Result Receive(Message &m, Socket * userSocket);
	//threads body
	void DoListening();
	void DoReceiving(Socket * userSocket);
	//void DoSending();
	void DoHandling();
	// accept user and create thread? 
	//AcceptUser - 
	void Stop();
	void Start();
};
#endif