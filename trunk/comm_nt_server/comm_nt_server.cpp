// comm_nt_server.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "Utilities.h"
#include "Socket.h"
#include "Server.h"

typedef std::list<Socket*> socket_list;

socket_list g_connections;

unsigned long __stdcall HandleUserConnect(void *param) {
	return 0;
}

unsigned __stdcall Connection(void* a) {
  Socket* s = (Socket*) a;

  g_connections.push_back(s);

  //s->SendLine("Welcome to the Message Distributor");

  while (1) {
    std::string r;
	s->ReceiveLine(r);
    if (r.empty()) break;

    for (socket_list::iterator os =g_connections.begin();
                               os!=g_connections.end(); 
                               os++) {
      (*os)->SendLine(r);
    }
  }

  g_connections.remove(s);

  delete s;

  return 0;
}
int _tmain(int argc, _TCHAR* argv[]){
	//Server( 1986, 10);
	//ServerSocket in(2000,5);
	std::string temp;
	while (1) {
		//std::cin >> temp;
		//std::cout << "\n" << temp;
		User u("login", "ip", 0);
		std::cout << u.ToString() << std::endl;
		User u2("login2","ip2", 0);
		Group g1;
		g1.GroupMembers.push_back(u);
		g1.GroupMembers.push_back(u2);
		std::cout << g1.ToString()<< std::endl;
		Group g2;
		g2.Parse(g1.ToString());
		std::cout << g2.ToString()<< std::endl;
		User sender("send", "ipsend", 0);
		Message m1 (MessageType::LOGIN, sender, g1, "zawartoœæ wiadomoœci");
		std::cout << m1.ToString() << std::endl;
		Message m2;
		m2.Parse(m1.ToString());
		std::cout << m2.ToString() << std::endl;
		std::cout << (g1 == g2) << std::endl;
		g1.GroupMembers.push_back(sender);
		std::cout << (g1 == g2) << std::endl;

		//_beginthreadex(0, 0, Connection, (void*) s, 0, &ret);
	}
  
  return 0;
}