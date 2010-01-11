#include "stdafx.h"
#include "Client.h"

unsigned long __stdcall Client:: SenderFunction(void*c){
	Client * C = reinterpret_cast<Client*>(c);
	C->DoSending();
	return 0;
}

unsigned long __stdcall Client:: ReceiverFunction(void*c){
	Client * C = reinterpret_cast<Client*>(c);
	C->DoReceiving();
	return 0;
}

unsigned long __stdcall Client:: HandlerFunction(void*c){
	Client * C = reinterpret_cast<Client*>(c);
	C->DoHandling();
	return 0;
}

void Client::DoHandling(){
	Message m;
	while(1) {
		NewInputMessage->Wait();
		InputMsgsAccess->Wait();
		m = InputMsgs.back();
		InputMsgs.pop_back();
		InputMsgsAccess->Release();
		switch (m.Type){
		case LOGIN: {
			// not ussed
			break;
		}
		case LOGOUT: {		
			// not used
			break;
		}
		case RESULT: {		
			// wynik logowania 
			std::cout << "Result: " << m.ToString() << std::endl;
			Me = m.Receiver;
			break;
		}
		case USERLIST: {
			std::cout << "User list: " << m.ToString() << std::endl;
			DataAccess->Wait();
			Users = m.InvolvedGroup.GroupMembers;
			DataAccess->Release();
			break;
		}
		case MESSAGE: {	
			std::cout << "Message: " << m.ToString() << std::endl;
			break;
		}
		case GROUPMESSAGE: {	
			std::cout << "Group Message: " << m.ToString() << std::endl;
			break;
		}
		default:
			break;
		}
	}
	return;
}

void Client::DoReceiving(){

	while (1){	
		// do receiving
		if (SelectSocket::CanRead(CommSocket, true)) {
			Message m = Message();
			Result result = Receive(m);		
			if (result != OK)
				break;
			std::cout << "Received messsage from user: " << m.Sender.ToString('@') << std::endl;
			InputMsgsAccess->Wait();
			InputMsgs.push_back(m);
			InputMsgsAccess->Release();
			NewInputMessage->Release();
			if (m.Type == LOGOUT) {
				std::cout << "Log Out" << std::endl;
				return;
			}
		}
	}
	return;
}

void Client::DoSending(){
	while(1) {
		NewOutputMessage->Wait();
		OutputMsgsAccess->Wait();
		Message m = OutputMsgs.back();
		OutputMsgs.pop_back();
		Send(m);
		OutputMsgsHistory.push_back(m);
		OutputMsgsAccess->Release();
		if (m.Type == LOGOUT) {
			std::cout << "Log Out" << std::endl;
			return;
		}

	}
	return;
}


Client::Client():IsConnected(false), IsLogged(false){
}

Client::~Client(){
}

Client::Client(std::string host, int port, bool connect): IsConnected(false), IsLogged(false){
	Host = host;
	Port = port;
	if (connect) {
		this->Connect(Host, Port);			
	}
}

Result Client::Connect(std::string host, int port) {
	if (IsConnected) {
		Disconnect();
	}
	CommSocket = new ClientSocket(host, port);
	NewInputMessage = new SysSemaphore(0, MESSAGE_QUEUE_LENGTH);
	NewOutputMessage = new SysSemaphore(0, MESSAGE_QUEUE_LENGTH);
	DataAccess = new SysSemaphore(1, 1);
	InputMsgsAccess = new SysSemaphore(1, 1);
	OutputMsgsAccess = new SysSemaphore(1, 1);

	ReceiverThread = new SysThread(Client::ReceiverFunction, (void *) this);
	SenderThread = new SysThread(Client::SenderFunction, (void *) this);
	HandlerThread = new SysThread(Client::HandlerFunction, (void *) this);

	HandlerThread->Start();
	SenderThread->Start();
	ReceiverThread->Start();

	IsConnected = true;
	IP = GetHostName("localhost");
	return OK;
}

Result Client::Disconnect() {
	if (IsConnected) {
		Port = 0;
		Host = "";
		IP = "";
		delete CommSocket;

		delete NewInputMessage;
		delete NewOutputMessage;
		delete DataAccess;
		delete InputMsgsAccess;
		delete OutputMsgsAccess;

		ReceiverThread->Terminate();
		SenderThread->Terminate();
		HandlerThread->Terminate();

		delete ReceiverThread;
		delete SenderThread;
		delete HandlerThread;
		CommSocket = 0;
		IsConnected = false;
	}
	return OK;
}

Result Client::LogIn(std::string login){
	if (!IsConnected) {
		if (OK != Connect(Host, Port))
			return FAILED;
		IsConnected = true;
	}
	if (IsLogged) {
		LogOut();
	}
	OutputMsgsAccess->Wait();
	OutputMsgs.push_back(Message(LOGIN, DateTimeNow(), User(login, IP), User(), Group(), ""));
	OutputMsgsAccess->Release();
	NewOutputMessage->Release();
	return OK;
}

Result Client::LogOut(){
	if (!IsConnected)
		IsLogged = false;
	if (IsLogged) {
		OutputMsgsAccess->Wait();
		OutputMsgs.push_back(Message(LOGIN, DateTimeNow(), Me, User(), Group(), ""));
		OutputMsgsAccess->Release();
		Me = User();
	}
	return OK;
}

Result Client::Send(Message & m){
	return CommSocket->SendBytes(m.ToString(), MESSAGE_DELIMITER);
}
void Client::SendAsynchronously(Message & m){
	OutputMsgsAccess->Wait();
	OutputMsgs.push_back(m);
	OutputMsgsHistory.push_back(m);
	OutputMsgsAccess->Release();
	NewOutputMessage->Release();
}
Result Client::Receive(Message &m){
	std::string received;
	Result result = CommSocket->ReceiveBytes(received, MESSAGE_DELIMITER);
	if (result != OK) {
		std::cout << "Cannot read from socket." << std::endl;
		return FAILED;
	}
	if (m.Parse(received) != OK){
		std::cout << "Cannot parse message." << std::endl;
		return FAILED;
	}
	return OK;
}

std::string Client::PrintUsers() {
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

void Client::SendToUser(User recipient, std::string text){
	SendAsynchronously(Message(MESSAGE, DateTimeNow(), Me, recipient, Group(), text));
}

void Client::SendToGroup(Group g, std::string text){
	Message m(GROUPMESSAGE, DateTimeNow(), Me, User(), g, text);
	SendAsynchronously(m);
}