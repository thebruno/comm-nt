#ifndef SERVER_H
#define SERVER_H
#include "stdafx.h"
#include "Utilities.h"
#include "BasicSystemClasses.h"
#include "Socket.h"

#define MESSAGE_SEPARATOR 0xFF
#define MESSAGE_GROUP_USER_SEPARATOR 0xFE
#define MESSAGE_USER_SEPARATOR 0xFD
#define MESSAGE_QUEUE_LENGTH 1024
enum MessageType
{
	// s = server
	// c = client
	// | - begining end ending of message
	// # - separator = 0xff
	NOTSET,
	LOGIN, // add user to list on server, c->s, |LOGIN#USER#|
	LOGOUT, // disconnectiong from server, c->s, |LOGOUT#USER#|
	RESULT, // result of performed operation, s->c, |RESULT#OPERATION#RESULT#|
	USERLIST, // list of users, s->c, |USERLIST#USER1#USER2#...#USERN#|
	MESSAGE, // simple text message, c->s->c, |MESSAGE#FROM USER#TO USER#|
	GROUPMESSAGE, // simple text message for group of recipients, c->s->c1, c2, c3, |MESSAGE#FROM USER#USER1#USER2#...#USERN|
};

std::string ToString(MessageType command);
MessageType ToEnum(std::string s);


//HANDLE InputMutex;
//InputMutex = CreateMutex(0, false, 0);
//CloseHandle(InputMutex);


struct User : public Serializable{
	std::string Login;
	std::string IP;
	Socket * UserSocket;
	// byc mo¿e nie trzeba bêdzie u¿ywaæ
	SysSemaphore* UserSocketAccess;
	User(std::string login, std::string ip, ClientSocket * userSocket);
	User(ClientSocket * userSocket);
	User();
	~User();
	virtual std::string ToString();
	virtual Result Parse(std::string &object);
	bool operator==(const User & u2) const;

};

struct Group: public Serializable{
	std::list<User > GroupMembers;
	virtual std::string ToString();
	virtual Result Parse(std::string &object);
	bool operator==(const Group & u1) const;
};

struct Message: public Serializable {
	MessageType Type;
	User Sender;
	Group Recipients;
	std::string Text;
	Message(MessageType type, User sender, Group recipients, std::string text);
	Message();
	virtual std::string ToString();
	virtual Result Parse(std::string &object);
};

class Server {
private:
	ServerSocket* ListenSocket;
	std::list<User> Users;
	std::list<Group> Grupes;
	// buffer from all users
	std::list<Message> InputMsgs;
	// buffer for all users
	std::list<Message> OutputMsgs;
	SysSemaphore * InputMsgsAccess;
	SysSemaphore * OutputMsgsAccess;
	SysSemaphore * NewMessage;

	
	static unsigned long __stdcall ListenerFunction(void*s);
	static unsigned long __stdcall ReceiverFunction(void*s);
	//static unsigned long __stdcall SenderFunction(void*s);
	static unsigned long __stdcall HandlerFunction(void*s);
	
	//static unsigned long (__stdcall *Handler)(void*s);
	
	SysThread *ListenerThread;
	std::list<SysThread *> ReceiverThreads;
	//SysThread *SenderThread;
	SysThread *HandlerThread;
	void HandleMessage(Message* message);

	//methods - not used if static is enough
	User HandleClientConnection(Socket* s);

public:
	Server(int port, int maxConnections);
	~Server();
	Result Send(Message m);
	Result Receive(Message &m);
	//threads body
	void DoListening();
	void DoReceiving(Socket * userSocket);
	//void DoSending();
	void DoHandling();
	// accept user and create thread? 
	//AcceptUser - 
	void Stop();
	void Start(int port, int maxConnections);
};
#endif