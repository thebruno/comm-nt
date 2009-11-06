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
}

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
	// get first data from client, drop user if there is another user with the same login and ip
	// if ok - add user
	Result result = Receive(m, userSocket);
	if (result != OK)
		return;
	u = m.Sender;
	DataAccess->Wait();
	if (m.Type == LOGIN){
		if (IsUserLogged(u) == true) {
			DataAccess->Release();
			// login not available, send fail
			std::cout << "Login not available! " << std::endl;
			// sand back that user have to put different login
			std::string info;
			info.append("Login: ").append(m.Sender.Login).append(" is not available. Please choose different login.");
			Message temp (RESULT, DateTimeNow(), User(), LOGIN, FAILED, info) ;			
			this->Send(temp, userSocket);
			delete userSocket;
			return;
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
	// inform about new message
	NewMessage->Release();
	while (1){	
		// do receiving
		if (SelectSocket::CanRead(constUserSocket, true)) {
			m = Message();
			Result result = Receive(m, userSocket);		
			if (result != OK)
				break;
			std::cout << "Received: " << m.ToString() << " from user: " << u.ToString() << std::endl;
			InputMsgsAccess->Wait();
			InputMsgs.push_back(m);
			InputMsgsAccess->Release();
			NewMessage->Release();
			if (m.Type == LOGOUT)
				return;
		}
	}
	// clean up
	DataAccess->Wait();
	RemoveUser(u);
	std::cout << "User: " << u.ToString() << " disconnected." << std::endl;
	DataAccess->Release();
	// emulowanie roz³¹czenia tak jakby user przys³a³, ¿e siê roz³¹czy³
	InputMsgsAccess->Wait();	
	InputMsgs.push_back(Message(LOGOUT, DateTimeNow(), u, User(), Group(), ""));
	InputMsgsAccess->Release();
	NewMessage->Release();
	return;
}

void Server::DoHandling(){
	std::list<User>::iterator uIt;
	while(1){
		NewMessage->Wait();
		// get message
		InputMsgsAccess->Wait();
		Message m = InputMsgs.back();
		InputMsgs.pop_back();
		InputMsgsAccess->Release();
		DataAccess->Wait();
		// handle
		switch (m.Type) {
		case LOGIN: {
			// user has already been added, map his/her thread only
			MapThreadToUser(m.Sender);
			Message newMessage;
			// send all users list to all users
			for (uIt = Users.begin(); uIt != Users.end(); ++uIt) {
				newMessage = Message(USERLIST, DateTimeNow(), User(), *uIt, Group(Users), "");
				this->Send(newMessage);
			}
			newMessage = Message(RESULT, DateTimeNow(), m.Sender, LOGIN, OK, "You have logged in successfully");
			this->Send(newMessage);
			break;
		}
		case LOGOUT: {
			// user has already been removed
			// his/her thread will be removed soon
			// send all users list
			for (uIt = Users.begin(); uIt != Users.end(); ++uIt) {
				Message newMessage = Message(USERLIST, DateTimeNow(), User(), *uIt, Group(Users), "");
				this->Send(newMessage);
			}
			break;
		}
		case RESULT: {
			// send it to user
			this->Send(m);
			break;
		}
		case USERLIST: {
			// cannot occure at server side
			break;
		}
		case MESSAGE: {
			// send it to user
			this->Send(m);
			break;
		}
		case GROUPMESSAGE: {
			// create group if it has not yet been created.
			if (IsGroupCreated(m.InvolvedGroup) == false)
				Groups.push_back(m.InvolvedGroup);
			// send to all users (without sender)
			for (uIt = m.InvolvedGroup.GroupMembers.begin(); uIt != m.InvolvedGroup.GroupMembers.end(); ++uIt) {
				// don't send back to sender
				if (!(*uIt == m.Sender) && IsUserLogged(*uIt)) {
					m.Receiver = *uIt;
					this->Send(m);
				}
			}
			break;
		}
		default:
			std::cout << "Cannot handle this message: "<< m.ToString() << std::endl;
		}
		//TODO: timeouts
		DeleteNotUsedThreads();
		DataAccess->Release();
	}
}

// TODO: add try/catch at all methods operation on sockets??????
Result Server::Send(Message& m){
	return Sockets[m.Receiver.ToString()]->SendBytes(m.ToString(), MESSAGE_DELIMITER);
}

Result Server::Send(Message& m, Socket * userSocket){
	return userSocket->SendBytes(m.ToString(), MESSAGE_DELIMITER);
}

Result Server::Receive(Message &m, Socket * userSocket){
	std::string received;
	Result result = userSocket->ReceiveBytes(received, MESSAGE_DELIMITER);
	if (result != OK ) {
		std::cout << "Cannot read from socket." << std::endl;
		return FAILED;
	}
	if (m.Parse(received) != OK){
		std::cout << "Cannot parse message." << std::endl;
		return FAILED;
	}
	return OK;
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
		NewMessage = new SysSemaphore(0, MESSAGE_QUEUE_LENGTH);
		ListenerThread = new SysThread(Server::ListenerFunction, this);
		HandlerThread = new SysThread(Server::HandlerFunction,  this);

		HandlerThread->Start();
		ListenerThread->Start();
	}
}

void Server::Stop(){
	if (Started) {
		Started = false;
		ListenerThread->Terminate();
		delete ListenerThread;
		delete ListenSocket;

		std::map<Socket*, SysThread *>::iterator it;
		for(it = UnverifiedReceiverThreads.begin(); it != UnverifiedReceiverThreads.end(); it++)
		{
			it->second->Terminate();
			delete it->second;
		}
		UnverifiedReceiverThreads.clear();

		std::map<std::string, SysThread *>::iterator mit;
		for(mit=ReceiverThreads.begin(); mit !=ReceiverThreads.end(); )
		{
			mit->second->Terminate();
			delete mit->second;			
		}
		ReceiverThreads.clear();

		HandlerThread->Terminate();
		delete HandlerThread;

		DeleteNotUsedThreads();
		delete NewMessage;
		delete InputMsgsAccess;
		delete OutputMsgsAccess;
		delete DataAccess;
	}
}

bool Server::IsUserLogged(User &u) {
	std::list<User>::iterator it;
	for (it = Users.begin(); it != Users.end(); it++)
		if (*it == u) 
			return true;
	return false;
}

bool Server::IsGroupCreated(Group &g) {
	std::list<Group>::iterator it;
	for (it = Groups.begin(); it != Groups.end(); it++)
		if (*it == g) 
			return true;					
	return false;
}

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

void Server::AddGroup(Group &g){
	Groups.push_back(g);
}

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

std::string Server::PrintUsers() {
	std::string toReturn = "Users list:\n";
	std::list<User>::iterator it;
	DataAccess->Wait();
	for (it = Users.begin(); it != Users.end(); ++it ) {
		toReturn.append("*");
		toReturn.append(it->Login);
		toReturn.append(" from ");
		toReturn.append(it->IP);
		toReturn.append("\n");
	}
	DataAccess->Release();
	return toReturn;
}

//switch (Type){
//	case LOGIN: {	
//		break;
//	}
//	case LOGOUT: {		
//		break;
//	}
//	case RESULT: {			
//		break;
//	}
//	case USERLIST: {		
//		break;
//	}
//	case MESSAGE: {	
//		break;
//	}
//	case GROUPMESSAGE: {	
//		break;
//	}
//	default:
//		break;
//}