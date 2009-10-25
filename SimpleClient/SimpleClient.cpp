// SimpleClient.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Client.h"

int _tmain(int argc, _TCHAR* argv[])
{
	/*ClientSocket c("localhost", 1986);
	c.SendBytes(Message (MessageType::LOGIN, User("ala", "IP"), User(), Group(), "zawartoœæ wiadomoœci").ToString(), MESSAGE_DELIMITER);*/
	Client * client = new Client("localhost", 1986, true);
	std::string toSend, received;
	Message m;
	
	while (1) {
		received = "";
		getline(std::cin, toSend);
		if (toSend.size() > 1) {
			if (toSend[0] == 'l') {
				std::cout << "Login: " << toSend.substr(2, toSend.size() - 2) << std::endl;
				client->LogIn(toSend.substr(2, toSend.size() - 2));
			}
			if (toSend[0] == 'o') {
				std::cout << "logout" << std::endl;
			}
			if (toSend[0] == 'm') {
				client->SendToUser(User(toSend.substr(2, toSend.size() - 2), "BrunoPC"), toSend.substr(5, toSend.size() - 5));
				std::cout << "message" << std::endl;
			}
			if (toSend[0] == 'g') {
				std::cout << "group" << std::endl;
			}
			if (toSend[0] == 'g') {
				//std::cout << client->PrintUsers() << std::endl;
			}
		}
	}

	return 0;
}

/*
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
		
	}*/