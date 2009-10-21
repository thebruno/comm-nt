// SimpleClient.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <Server.h>

int _tmain(int argc, _TCHAR* argv[])
{
	ClientSocket Client("localhost", 1986);
	User u1("ala","ip2");
	Group g1;
	g1.GroupMembers.push_back(User("ala", "IP"));
	g1.GroupMembers.push_back(User("ola", "IP"));
	//g1.GroupMembers.push_back(User("jola", "ip"));

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
		//Client.SendBytes(Message (MessageType::LOGOUT, User(temp, "IP"), User(), Group(), "zawartoœæ wiadomoœci").ToString(), MESSAGE_DELIMITER);
		std::cin >>received;
		Client.SendBytes(Message (MessageType::GROUPMESSAGE, User(temp, "IP"), User(), g1, "zawartoœæ wiadomoœci").ToString(), MESSAGE_DELIMITER);
		
	}
	return 0;
}

