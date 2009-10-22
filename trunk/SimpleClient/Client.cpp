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
	return;
}

void Client::DoReceiving(){
	return;
}

void Client::DoSending(){
	return;
}


Client::Client(){
}

Client::~Client(){
}
Client::Client(std::string host, int port){
}

Result Client::LogIn(User u){
	return Result::OK;
}

Result Client::LogOut(){
	return Result::OK;
}

Result Client::Send(Message & m){
	return Result::OK;
}
Result Client::Receive(Message &m){
	return Result::OK;
}