#include "stdafx.h"
#include "Server.h"



Server::Server(int port, int maxConnections, bool start ) {
	Started = start;
	Port = port;
	MaxConnections = maxConnections;
	if (start)
		Start();
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
		SysThread * receiverThread = new SysThread(Server::ReceiverFunction, (void *) &std::pair<Server *, Socket *>(this, s));
		UnverifiedReceiverThreads.push_back(receiverThread);
		receiverThread->Start();		
	}
}

// dodaæ multiviadomoœci...
void Server::DoReceiving(Socket *userSocket){
	Socket const * const constUserSocket = static_cast<Socket const * const>(userSocket);
	User u;
	Message m;
	std::string received = "";
	bool cont = true;
	// pobranie informacji od klienta, trzeba to zrobiæ tutaj, ¿eby odrzuciæ po³¹czenie jeœli user istnieje
	// wiadomoœæ trzeba wrzuciæ do skrzynki odbiorczej, ¿eby poinformowaæ resztê
	Result result = userSocket->ReceiveBytes(received, MESSAGE_DELIMITER);
	if (result != Result::OK ) {
		std::cout << "Cannot read from socket." << std::endl;
		return ;
	}
	if (m.Parse(received) != Result::OK){
		std::cout << "Cannot parse first message." << std::endl;
		return ;
	}
	u = m.InvolvedUser;
	received.clear();
	DataAccess->Wait();
	if (m.Type == MessageType::LOGIN){
		if (IsUserLogged(u) == false) {
			//TODO: odes³aæ, ¿e login zajêty
			std::cout << "Login not available! " << std::endl;
		}
		AddUser(u, userSocket);
		InputMsgsAccess->Wait();
		InputMsgs.push_back(m);
		InputMsgsAccess->Release();
		std::cout << "User: " << u.ToString() << " has logged in." << std::endl;
	} else {
		std::cout << "Wrong message type." << std::endl;
		//TODO: b³êdny komunikat
	}
	DataAccess->Release();
	
	if (!cont)
		return;
	while (1){			
		if (SelectSocket::CanRead(constUserSocket, true)) {
			received.clear();
			m = Message();
			Result result = userSocket->ReceiveBytes(received, MESSAGE_DELIMITER);
			if (result != Result::OK) {
				std::cout << "Cannot read data from socket." << std::endl;
				break;
			}		
			result = m.Parse(received);
			if (result != Result::OK){
				std::cout << "Parsing error." << std::endl;
				break;
			}
			std::cout << "Received: " << m.ToString() << " from user: " << u.ToString() << std::endl;

			DataAccess->Wait();

			//TODO: czy wysy³aj¹æ userowi lisetê u¿ytkowników, umieszczaæ go tam?
			// raczej tak, ale jeœli from to ten sam user - nie wyœwietlaj u usera
			InputMsgsAccess->Wait();
			InputMsgs.push_back(m);
			InputMsgsAccess->Release();
			NewMessage->Release();
			DataAccess->Release();
		}
	}
	DataAccess->Wait();
		RemoveUser(u);
		std::cout << "User: " << u.ToString() << " disconnected or has been remeved due to error(s)." << std::endl;
	DataAccess->Release();
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
		DataAccess->Wait();
		switch (m.Type) {
		case MessageType::LOGIN: {
			// rozes³aæ info do reszty
			break;
		}
		case MessageType::LOGOUT: {
			//rozes³aæ info do reszty

			break;
		}
		case MessageType::RESULT: {
			break;
		}
		case MessageType::USERLIST: {
			break;
		}
		case MessageType::MESSAGE: {
			break;
		}
		case MessageType::GROUPMESSAGE: {
			break;
		}
		default:
			std::cout << "Cannot handle this message: "<< m.ToString() << std::endl;
		}
		DataAccess->Release();
	}
}

Result Server::Send(Message m){
	return Result::OK;
}
Result Receive(Message &m){
	return Result::OK;
}

void Server::Start(){ 
	if (!Started) {
		Started = true;
		ListenSocket = new ServerSocket(Port, MaxConnections);
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
}

void Server::Stop(){
	if (Started) {
		Started = false;
		delete ListenSocket;
		ListenerThread->Terminate();
		delete ListenerThread;
		std::list<SysThread *>::iterator it;
		for(it = UnverifiedReceiverThreads.begin(); it != UnverifiedReceiverThreads.end(); it++)
		{
			(*it)->Terminate();
			delete *it;
		}
		UnverifiedReceiverThreads.clear();
		//TODO:
		// jak siê nowy user pojawia
		// to rozes³aæ info do reszty
		std::map<std::string, SysThread *>::iterator mit;
		for(mit=ReceiverThreads.begin(); mit !=ReceiverThreads.end(); )
		{
			mit->second->Terminate();
			delete mit->second;
			
		}
		ReceiverThreads.clear();
		//SenderThread->Terminate();
		//delete SenderThread;
		HandlerThread->Terminate();
		delete HandlerThread;
	}
}
// use semaphores!
bool Server::IsUserLogged(User &u) {
	std::list<User>::iterator it;
	for (it = Users.begin(); it != Users.end(); it++)
		if (*it == u) 
			return true;
	return false;
}
// use semaphores!
bool Server::IsGroupCreated(Group &g) {
	DataAccess->Wait();
	std::list<Group>::iterator it;
	for (it = Groups.begin(); it != Groups.end(); it++)
		if (*it == g)
			return true;
	return false;
}
// use semaphores!
void Server::AddUser(User &u, Socket *s){
	Users.push_back(u);
	Sockets[u.ToString()] = s;
}
// use semaphores!
void Server::AddGroup(Group &g){
	Groups.push_back(g);
}
// use semaphores!
void Server::RemoveUser(User &u){
	Users.remove(u);
	std::map<std::string, Socket *>::iterator it;
	std::string key = u.ToString();
	for(it=Sockets.begin(); it !=Sockets.end(); )
	{
		if(it->first == key)
			Sockets.erase(it++);
		else
			++it;
	}
}
// use semaphores!
void Server::RemoveGroup(Group &g){
	Groups.remove(g);
}
