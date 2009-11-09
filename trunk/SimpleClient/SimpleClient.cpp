// SimpleClient.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Client.h"

int _tmain(int argc, _TCHAR* argv[])
{
	/*ClientSocket c("localhost", 1986);
	c.SendBytes(Message (LOGIN, User("ala", "IP"), User(), Group(), "zawartoœæ wiadomoœci").ToString(), MESSAGE_DELIMITER);*/
	Client * client = new Client("AbacaNETFP", 6666, true);
	std::string toSend, received;
	Message m;
	
	while (1) {
		received = "";
		getline(std::cin, toSend);
		std::vector<std::string> temp = split(toSend,' ');
		if (toSend.size() > 1) {
			if (toSend[0] == 'l') {
				std::cout << "Login: " << temp[1] << std::endl;
				client->LogIn(temp[1]);
			}
			if (toSend[0] == 'o') {
				std::cout << "logout" << std::endl;
			}
			if (toSend[0] == 'm') {
				client->SendToUser(User(temp[1], "BrunoPC"), temp[2]);
				std::cout << "message" << std::endl;
			}
			if (toSend[0] == 'g') {
				std::cout << "group" << std::endl;
				Group g;
				for (int i = 1; i <= temp.size() - 2; i ++)
					g.GroupMembers.push_back(User(temp[i], "BrunoPC"));
				client->SendToGroup(g, temp[temp.size() - 1]);
			}
			if (toSend[0] == 'u') {
				std::cout << client->PrintUsers() << std::endl;
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
	Client.SendBytes(Message (LOGIN, User(temp, "IP"), User(), Group(), "zawartoœæ wiadomoœci").ToString(), MESSAGE_DELIMITER);
	while(1){
		received = "";
		Client.ReceiveBytes(received, MESSAGE_DELIMITER);
		std::cout << received << std::endl;
		//Client.SendBytes(Message (LOGOUT, User(temp, "IP"), User(), Group(), "zawartoœæ wiadomoœci").ToString(), MESSAGE_DELIMITER);
		std::cin >>received;
		Client.SendBytes(Message (GROUPMESSAGE, User(temp, "IP"), User(), g1, "zawartoœæ wiadomoœci").ToString(), MESSAGE_DELIMITER);
		
	}*/