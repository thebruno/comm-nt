#include "stdafx.h"
#include "Server.h"



Server::Server(int port, int maxConnections, bool start ) {
	Started = false;
	Port = port;
	MaxConnections = maxConnections;
	if (start)
		Start();
}
Server::~Server(){
	Stop();
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
		UnverifiedReceiverThreads[s] = receiverThread;
		receiverThread->Start();		
	}
}

// dodaæ multiviadomoœci...
void Server::DoReceiving(Socket *userSocket){
	Socket const * const constUserSocket = static_cast<Socket const * const>(userSocket);
	User u;
	Message m;
	bool cont = true;
	// pobranie informacji od klienta, trzeba to zrobiæ tutaj, ¿eby odrzuciæ po³¹czenie jeœli user istnieje
	// wiadomoœæ trzeba wrzuciæ do skrzynki odbiorczej, ¿eby poinformowaæ resztê
	Result result = Receive(m, userSocket);
	if (result != Result::OK)
		return;
	u = m.Sender;
	DataAccess->Wait();
	if (m.Type == MessageType::LOGIN){
		if (IsUserLogged(u) == true) {
			//TODO: odes³aæ, ¿e login zajêty
			std::cout << "Login not available! " << std::endl;
		} else {
			AddUser(u, userSocket);
			InputMsgsAccess->Wait();
			InputMsgs.push_back(m);
			InputMsgsAccess->Release();
			std::cout << "User: " << u.ToString() << " has logged in." << std::endl;
		}
	} else {
		std::cout << "Wrong message type." << std::endl;
		//TODO: b³êdny komunikat
	}
	DataAccess->Release();
	
	if (!cont)
		return;
	NewMessage->Release();
	while (1){			
		if (SelectSocket::CanRead(constUserSocket, true)) {
			m = Message();

			Result result = Receive(m, userSocket);		
			if (result != Result::OK)
				break;
			std::cout << "Received: " << m.ToString() << " from user: " << u.ToString() << std::endl;

			//DataAccess->Wait();

			//TODO: czy wysy³aj¹æ userowi lisetê u¿ytkowników, umieszczaæ go tam?
			// raczej tak, ale jeœli from to ten sam user - nie wyœwietlaj u usera
			InputMsgsAccess->Wait();
			InputMsgs.push_back(m);
			InputMsgsAccess->Release();
			NewMessage->Release();
			//DataAccess->Release();
			if (m.Type == MessageType::LOGOUT)
				return;
		}
	}
	DataAccess->Wait();
	RemoveUser(u);
	std::cout << "User: " << u.ToString() << " disconnected or has been remeved due to error(s)." << std::endl;
	DataAccess->Release();
	// emulowanie roz³¹czenia tak jakby user przys³a³, ¿e siê roz³¹czy³
	InputMsgsAccess->Wait();	
	InputMsgs.push_back(Message(MessageType::LOGOUT, u, User(), Group(), ""));
	InputMsgsAccess->Release();
	NewMessage->Release();
	return;
}

//void Server::DoSending(){}

void Server::DoHandling(){
	std::list<User>::iterator uIt;
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
			// user ju¿ dodany przy odborze
			MapThreadToUser(m.Sender);
			//OutputMsgsAccess->Wait();

			for (uIt = Users.begin(); uIt != Users.end(); ++uIt) {
				Message newMessage = Message(MessageType::USERLIST, User(), *uIt, Group(Users), "");
				this->Send(newMessage);
				//OutputMsgs.push_back(newMessage);
			}
			// add messages for all users
			//OutputMsgsAccess->Release();
			// rozes³aæ info do reszty
			break;
		}
		case MessageType::LOGOUT: {
			// Usuwanie usera zrobione w w¹tku usera
			// usuwanie w¹tku usera w DoHandling
			//rozes³aæ info do reszty
			for (uIt = Users.begin(); uIt != Users.end(); ++uIt) {
				Message newMessage = Message(MessageType::USERLIST, User(), *uIt, Group(Users), "");
				this->Send(newMessage);
				//OutputMsgs.push_back(newMessage);
			}
			break;
		}
		case MessageType::RESULT: {
			break;
		}
		case MessageType::USERLIST: {
			// cannot occure at server side
			break;
		}
		case MessageType::MESSAGE: {
			this->Send(m);
			break;
		}
		case MessageType::GROUPMESSAGE: {
			// pass it to user group
			break;
		}
		default:
			std::cout << "Cannot handle this message: "<< m.ToString() << std::endl;
		}
		// sprawdziæ timeouty
		SendMessages();
		DeleteNotUsedThreads();
		DataAccess->Release();
	}
}

// do wywalenia!!
Result Server::SendMessages(){
	return Result::OK;
}
// sprawdzanie b³êdów? try/catch?
Result Server::Send(Message& m){
	return Sockets[m.Receiver.ToString()]->SendBytes(m.ToString(), MESSAGE_DELIMITER);
}

Result Server::Receive(Message &m, Socket * userSocket){
	std::string received;
	Result result = userSocket->ReceiveBytes(received, MESSAGE_DELIMITER);
	if (result != Result::OK ) {
		std::cout << "Cannot read from socket." << std::endl;
		return Result::FAILED;
	}
	if (m.Parse(received) != Result::OK){
		std::cout << "Cannot parse message." << std::endl;
		return Result::FAILED;
	}
	return Result::OK;
}
Result Server::Receive(Message &m){
	return this->Receive(m, Sockets[m.Sender.ToString()]);
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
		std::map<Socket*, SysThread *>::iterator it;
		for(it = UnverifiedReceiverThreads.begin(); it != UnverifiedReceiverThreads.end(); it++)
		{
			it->second->Terminate();
			delete it->second;
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
void Server::MapThreadToUser(User & u) {
	std::map<Socket *, SysThread *>::iterator it;
	for (it = UnverifiedReceiverThreads.begin(); it != UnverifiedReceiverThreads.end(); ++it) {
		if (it->first == Sockets[u.ToString()]) {
			ReceiverThreads[u.ToString()] = it->second;
			UnverifiedReceiverThreads.erase(it);
			break;
		}
	}
}
// use semaphores!
void Server::AddGroup(Group &g){
	Groups.push_back(g);
}
// use semaphores!
// remove user with thread
// it is called from receiver thread, so we just want to delete it from map
// it will finish a moment later
void Server::RemoveUser(User &u){
	Users.remove(u);
	std::map<std::string, Socket *>::iterator it;
	std::string key = u.ToString();
	for(it=Sockets.begin(); it !=Sockets.end(); )
	{
		if(it->first == key) {
			delete it->second;
			Sockets.erase(it++);
		}
		else
			++it;
	}	
	SysThread * toDelete;
	std::map<std::string, SysThread*>::iterator mit;
	for(mit=ReceiverThreads.begin(); mit !=ReceiverThreads.end(); )
	{
		if(mit->first == key) {
			toDelete = mit->second;
			ThreadsToDelete.push_back(toDelete);
			ReceiverThreads.erase(mit++);
		}
		else
			++mit;	
	}
}
// use semaphores!
void Server::RemoveGroup(Group &g){
	Groups.remove(g);
}

void Server::DeleteNotUsedThreads(){
	std::list<SysThread *>::iterator i;
	for(i = ThreadsToDelete.begin(); i != ThreadsToDelete.end(); i++)
	{
		(*i)->Terminate();
		delete *i;
	}
	ThreadsToDelete.clear();
}

//switch (Type){
//	case MessageType::LOGIN: {	
//		break;
//	}
//	case MessageType::LOGOUT: {		
//		break;
//	}
//	case MessageType::RESULT: {			
//		break;
//	}
//	case MessageType::USERLIST: {		
//		break;
//	}
//	case MessageType::MESSAGE: {	
//		break;
//	}
//	case MessageType::GROUPMESSAGE: {	
//		break;
//	}
//	default:
//		break;
//}