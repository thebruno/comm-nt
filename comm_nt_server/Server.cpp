#include "stdafx.h"
#include "Server.h"

std::string ToString(CommandType command) {
	switch (command)
	{
		case CommandType::CONNECT:
			return "CONNECT";
		case CommandType::LOGIN:
			return "LOGIN";
		case CommandType::DISCONNECT:
			return "DISCONNECT";
		case CommandType::LOGOUT:
			return "LOGOUT";
		case CommandType::MESSAGE:
			return "MESSAGE";
		case CommandType::GETUSERS:
			return "GETUSERS";
		case CommandType::GETMESSAGE:
			return "GETMESSAGE";
	}
}

CommandType ToEnum(std::string s)
{
	if (strcmp(s.c_str(), "CONNECT"))
	{
		return CommandType::CONNECT;
	}
	else if (strcmp(s.c_str(), "LOGIN"))
	{
		return CommandType::LOGIN;
	} 
	else if (strcmp(s.c_str(), "DISCONNECT"))
	{
		return CommandType::DISCONNECT;
	} 
	else if (strcmp(s.c_str(), "LOGOUT"))
	{
		return CommandType::LOGOUT;
	} 
	else if (strcmp(s.c_str(), "MESSAGE"))
	{
		return CommandType::MESSAGE;
	} 
	else if (strcmp(s.c_str(), "GETUSERS"))
	{
		return CommandType::GETUSERS;
	} 
	else if (strcmp(s.c_str(), "GETMESSAGE"))
	{
		
	} 
}


Packet::Packet(std::string s)
{
	std::string temp;
	int begin = 0;
	//strsp
	for (int i = 0; i < s.length(); i++)
	{
	//	if (s[

	}
}


std::string Packet::Pack(){
	std::string toReturn;
	//toReturn.append(ToString(Command));
	//ToString(this->Command);
	toReturn.append("\0xFF");
	toReturn.append(Content);
	toReturn.append("\0xFF");
	toReturn.append(SourceIP);
	toReturn.append("\0xFF");
	toReturn.append(SourceUser);
	toReturn.append("\0xFF");
	toReturn.append(DestinationUser);
	toReturn.append("\0xFF");
	toReturn.append(DestintionGroup);
	toReturn.append("\0xFF");
	return toReturn;
}



//ok
unsigned long __stdcall Server::ListenerFunction(void*s){
	Server * S = reinterpret_cast<Server*>(s);
	S->DoListening();
	return 0;
}

unsigned long __stdcall Server::ReceiverFunction(void*s){
	Server * S = reinterpret_cast<Server*>(s);
	S->DoListening();
	return 0;
}

unsigned long __stdcall Server::SenderFunction(void*s){
	Server * S = reinterpret_cast<Server*>(s);
	S->DoListening();
	return 0;
}

unsigned long __stdcall Server::HandlerFunction(void*s){
	Server * S = reinterpret_cast<Server*>(s);
	S->DoListening();
	return 0;
}

void Server::DoListening(){
	while (1) {
		//Socket* s=in.Accept();
		unsigned long ret;
	}
}

void Server::DoReceiving(){}

void Server::DoSending(){

void Server::DoHandling(){