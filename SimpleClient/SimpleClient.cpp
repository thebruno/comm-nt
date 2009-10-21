// SimpleClient.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <Server.h>

int _tmain(int argc, _TCHAR* argv[])
{
	ClientSocket Client("localhost", 1986);
	User u2("login2","ip2");
	Group g1;
	g1.GroupMembers.push_back(u2);

	std::string temp = "";
	std::string received = "";
	std::cout << "login: ";
	std::cin >> temp;
	std::cout << std::endl;
	Client.SendBytes(Message (MessageType::LOGIN, User(temp, "IP"), User(), Group(), "zawartoœæ wiadomoœci").ToString(), MESSAGE_DELIMITER);
	while(1){
		received = "";
		Client.ReceiveBytes(received, MESSAGE_DELIMITER);
		std::cout << received << std::endl;
		
	}
	return 0;
}

