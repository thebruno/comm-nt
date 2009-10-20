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
	while(1){
		int temp = 0;
		Client.SendBytes(Message (MessageType::LOGIN, User("login", "IP"), g1, "zawartoœæ wiadomoœci").ToString());
		std::cin >> temp;
	}
	return 0;
}

