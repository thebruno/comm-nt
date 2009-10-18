#ifndef SERVER_H
#define SERVER_H
#include "stdafx.h"
#include "Utilities.h"
#include "BasicSystemClasses.h"
#include "Socket.h"


enum CommandType
{
	CONNECT, LOGIN, DISCONNECT, LOGOUT, MESSAGE, GETUSERS, GETMESSAGE
};

std::string ToString(CommandType command);

CommandType ToEnum(std::string s);



class Packet 
{
public:
	CommandType Command;
	std::string Content;
	std::string SourceIP;
	std::string SourceUser;
	std::string DestinationUser;
	std::string DestintionGroup;
	std::string Pack();
	Packet(std::string s);
};

class UserQueue
{

	std::list<Packet*> Input;
	std::list<Packet*> Output;
	HANDLE InputMutex, OutputMutex;
	Socket * UserSocket;
public:
	UserQueue(Socket * s)
	{
		UserSocket = s;
		InputMutex = CreateMutex(0, false, 0);
		OutputMutex = CreateMutex(0, false, 0);
	}

	~UserQueue()
	{
		CloseHandle(InputMutex);
		CloseHandle(OutputMutex);
	}
	void Write(Packet * packet){
		Output.push_back(packet);
	}
	Packet * Read()
	{
		Packet * temp = Input.front();
		Input.pop_front();
	}
	bool CanRead()
	{		
		return !Input.empty();
	}
};

// ok
class User{
};

class Group{
};

class Message{
};


class Server {
private:
	ServerSocket* ListenSocket;
	std::list<User*> Users;
	std::list<Group*> Grupes;
	// buffer from all users
	std::list<Message*> InputMsgs;
	// buffer for all users
	std::list<Message*> OutputMsgs;
	SysSemaphore * InputMsgsAccess;
	SysSemaphore * OutputMsgsAccess;
	
	static unsigned long __stdcall ListenerFunction(void*s);
	static unsigned long __stdcall ReceiverFunction(void*s);
	static unsigned long __stdcall SenderFunction(void*s);
	static unsigned long __stdcall HandlerFunction(void*s);
	
	//static unsigned long (__stdcall *Handler)(void*s);
	
	SysThread *ListenerThread;
	std::list<SysThread *> ReceiverThread;
	SysThread *SenderThread;
	SysThread *HandlerThread;
	void HandleMessage(Message* message);

	//methods - not used if static is enough


public:
	Server(int port, int maxConnections);
	//threads body
	void DoListening();
	void DoReceiving();
	void DoSending();
	void DoHandling();
	// accept user and create therad? 
	//AcceptUser - 
};
#endif