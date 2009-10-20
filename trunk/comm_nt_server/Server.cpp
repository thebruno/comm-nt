#include "stdafx.h"
#include "Server.h"

std::string ToString(MessageType type) {
	switch (type)
	{
		case MessageType::NOTSET:
			return "NOTSET";
		case MessageType::LOGIN:
			return "LOGIN";
		case MessageType::LOGOUT:
			return "LOGOUT";
		case MessageType::RESULT:
			return "RESULT";			
		case MessageType::USERLIST:
			return "USERLIST";
		case MessageType::MESSAGE:
			return "MESSAGE";
		case MessageType::GROUPMESSAGE:
			return "GROUPMESSAGE";
	}
}

MessageType Parse(std::string s)
{
	if (strcmp(s.c_str(), "NOTSET") == 0)
	{
		return MessageType::NOTSET;
	} 
	else if (strcmp(s.c_str(), "LOGIN") == 0)
	{
		return MessageType::LOGIN;
	} 
	else if (strcmp(s.c_str(), "LOGOUT") == 0)
	{
		return MessageType::LOGOUT;
	} 
	else if (strcmp(s.c_str(), "RESULT") == 0)
	{
		return MessageType::RESULT;
	} 
	else if (strcmp(s.c_str(), "USERLIST") == 0)
	{
		return MessageType::USERLIST;
	}
	else if (strcmp(s.c_str(), "GROUPMESSAGE") == 0)
	{
		return MessageType::GROUPMESSAGE;
	} 
}


User::User(std::string login, std::string ip, ClientSocket * userSocket): Login(login), IP(ip), UserSocket(userSocket) {
	// dodaæ inicjalizacje semafora do socketa
}
User::User( ClientSocket * userSocket): UserSocket(userSocket) {
	// dodaæ inicjalizacje semafora do socketa
}
User::User()
{
	UserSocket = 0;
}

User::~User() {
	delete UserSocket;
	UserSocket = 0;
}

std::string User::ToString() {
	std::string toReturn;
	toReturn.append(this->Login);
	toReturn += MESSAGE_USER_SEPARATOR;
	toReturn.append(this->IP);
	return toReturn;
}

Result User::Parse(std::string& s) {
	std::vector<std::string> splitString = split(s, MESSAGE_USER_SEPARATOR);
	if (splitString.size() < 2 || splitString[0] == "" || splitString[1] == "")
		return Result::FAILED;
	this->Login = splitString[0];
	this->IP = splitString[1];

	return Result::OK;
}

bool User::operator==(const User& u) const {
	if (u.Login == this->Login && u.IP == this->IP)
		return true;
	else 
		return false;

}

std::string Group::ToString(){
	std::string toReturn;
	std::list<User>::iterator it;
	for (it = GroupMembers.begin(); it != GroupMembers.end(); it++) {
		toReturn.append((*it).ToString());
		toReturn += MESSAGE_GROUP_USER_SEPARATOR;
	}
	toReturn = toReturn.substr(0, toReturn.size() - 1);
	return toReturn;
}

Result Group::Parse(std::string &s){
	User user;
	std::vector<std::string> splitUsers = split(s, MESSAGE_GROUP_USER_SEPARATOR);
	if (splitUsers.empty())
		return Result::FAILED;
	std::vector<std::string>::iterator it;
	for (it = splitUsers.begin(); it != splitUsers.end(); it++) {
		user.Parse(*it);
		GroupMembers.push_back(user);
	}
	return Result::OK;
}

bool Group::operator==(const Group &g) const {
	if (GroupMembers.size() != g.GroupMembers.size())
		return false;
	std::list<User>::const_iterator it1, it2;
	bool found = false;
	for (it1 = GroupMembers.begin(); it1 != GroupMembers.end(); it1++) {
		for (it2 = g.GroupMembers.begin(); it2 != g.GroupMembers.end(); it2++) {
			if (*it1 == *it2) {
				found = true;
				break;
			}
		}
		if (!found)
			return false;
		else 
			found = false;
	}
	return true;
}



Message::Message(MessageType type, User sender, Group recipients, std::string text):
	Type(type), Sender(sender), Recipients(recipients), Text(text){
}

Message::Message(){}

std::string Message::ToString(){
	std::string toReturn;
	toReturn.append(::ToString(Type));
	toReturn += MESSAGE_SEPARATOR;
	toReturn.append(Sender.ToString());
	toReturn += MESSAGE_SEPARATOR;
	toReturn.append(Recipients.ToString());
	toReturn += MESSAGE_SEPARATOR;
	toReturn.append(Text);
	return toReturn;
}

Result Message::Parse(std::string &s){
	User user;
	std::vector<std::string> splitFields = split(s, MESSAGE_SEPARATOR);
	if (splitFields.empty())
		return Result::FAILED;
	Type = ::Parse(splitFields[0]);
	Sender.Parse(splitFields[1]);
	Recipients.Parse(splitFields[2]);
	Text = splitFields[3];
	return Result::OK;
}


Server::Server(int port, int maxConnections)
{
	Start(port, maxConnections);
}
Server::~Server(){
	delete NewMessage;
	delete InputMsgsAccess;
	delete OutputMsgsAccess;
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
			Result temp = userSocket->ReceiveBytes(received);
			// odczytane wszystko?
			if (temp == Result::EMPTY && received != "" ){
				Message m;
				m.Parse(received);
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