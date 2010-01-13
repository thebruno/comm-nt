#include "stdafx.h"
#include "client.h"

void* Client:: SenderFunction(void*c){
	Client * C = reinterpret_cast<Client*>(c);
	C->DoSending();
	return 0;
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

        QTReceiverThread = new ReceiverThread(this);
	SenderThread = new SysThread(Client::SenderFunction, (void *) this);


        //QTReceiverThread->Start();

	IsConnected = true;
	IP = GetHostName("localhost");
        SenderThread->Start();
        QTReceiverThread->start();
	return OK;
}

Result Client::Disconnect() {
	if (IsConnected) {
                QTReceiverThread->terminate();
                SenderThread->Terminate();
		Port = 0;
		Host = "";
		IP = "";               
		delete CommSocket;
		delete NewInputMessage;
		delete NewOutputMessage;
		delete DataAccess;
		delete InputMsgsAccess;
		delete OutputMsgsAccess;

                delete QTReceiverThread;
		delete SenderThread;
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
        Message m = Message(MESSAGE, DateTimeNow(), Me, recipient, Group(), text);
        SendAsynchronously(m);
}

void Client::SendToGroup(Group g, std::string text){
        Message m(GROUPMESSAGE, DateTimeNow(), Me, User(), g, text);
	SendAsynchronously(m);
}


bool Client::IsUserLogged(User &u) {
        std::list<User>::iterator it;
        for (it = Users.begin(); it != Users.end(); it++)
                if (*it == u)
                        return true;
        return false;
}
