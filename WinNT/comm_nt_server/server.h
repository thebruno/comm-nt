#ifndef SERVER_H
#define SERVER_H
#include "stdafx.h"
#include "BasicSystemClasses.h"
#include "CommonClasses.h"
#include "Socket.h"

class Server {
private:
	int Port;
	int MaxConnections;
	bool Started;
	// this socket listen for new user to connect
	ServerSocket* ListenSocket; 
	// sockets indexed by users
	std::map<std::string, Socket*> Sockets;
	// currently logged users
	std::list<User> Users;
	// currently created groups
	std::list<Group> Groups;
	// buffer from all users
	std::list<Message> InputMsgs;
	// buffer for all users
	std::list<Message> OutputMsgs;
	// for safe access to Sockets, Users and Groupes
	SysSemaphore * DataAccess; 
	// for safe access to input buffer
	SysSemaphore * InputMsgsAccess; 
	// for safe access to output buffer
	SysSemaphore * OutputMsgsAccess; 
	// inform handle thread, that new message arrived
	SysSemaphore * NewMessage;

	// listener thread api funtion
	static unsigned long __stdcall ListenerFunction(void*s);
	// reciever thread api funtion
	static unsigned long __stdcall ReceiverFunction(void*s);
	// handler thread api funtion
	static unsigned long __stdcall HandlerFunction(void*s);
	
	// listens for user to connect
	SysThread *ListenerThread;
	// dodaæ czyszenie w destruktorze
	// receives data from client, every user has particular thread
	std::map<std::string, SysThread *> ReceiverThreads;
	// threads just after user connection and before authorising
	std::map <Socket *, SysThread *> UnverifiedReceiverThreads;
	// handles messages which have arrived from users
	SysThread *HandlerThread;
	// threads just after user disconnection, they are waiting for cleanup
	std::list<SysThread*> ThreadsToDelete;

	//methods - not used if static is enough
	// beffore calling any of this methods, be sure that correct semaphores are owned
	// checks whether user with the same login and ip if currently logged in
	bool IsUserLogged(User &u);
	// checks whether group with the same mambers has already been created
	bool IsGroupCreated(Group &g);
	// removes user and his/her socket, put thread to delete list
	void RemoveUser(User &u);
	// remove group
	void RemoveGroup(Group &g);
	// add user and his/her socket
	void AddUser(User &u, Socket *s);
	// add group
	void AddGroup(Group &g);
	// associate thread, user and socket together
	void MapThreadToUser(User & u);
	// clean up threads - debries from disconnected users
	void DeleteNotUsedThreads();

public:
	Server(int port, int maxConnections = 10, bool start = true);
	~Server();
	// send message to "Receiver" (message field)
	Result Send(Message& m);
	// send message, use particular socket
	Result Send(Message& m, Socket * userSocket);
	// receive message from "Sender" (message field)
	Result Receive(Message &m);
	// receive message, use particular socket
	Result Receive(Message &m, Socket * userSocket);
	
	//threads bodies
	// listen for user
	void DoListening();
	// receive data from user
	void DoReceiving(Socket * userSocket);
	// handle messages
	void DoHandling();
	// stop server
	void Stop();
	// start server
	void Start();
	// show current users
	std::string PrintUsers();
};
#endif