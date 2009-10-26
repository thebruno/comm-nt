// comm_nt_server.cpp : Defines the entry point for the console application.
#include "stdafx.h"

#include "Server.h"

typedef std::list<Socket*> socket_list;

socket_list g_connections;

void Tests(){
//std::cin >> temp;
		//std::cout << "\n" << temp;
		User u1("login1", "ip1");
		//std::cout << u1.ToString() << std::endl;
		User u2("login2","ip2");
		Group g1;
		g1.GroupMembers.push_back(u1);
		g1.GroupMembers.push_back(u2);
		//std::cout << g1.ToString()<< std::endl;
		//Group g2;
		//g2.Parse(g1.ToString());
		//std::cout << g2.ToString()<< std::endl;
		User sender("send", "ipsend");
		Message m1 (MESSAGE, sender, u1, g1, "zawartoœæ wiadomoœci");
		std::cout << m1.ToString() << std::endl;
		Message m2;
		m2.Parse(m1.ToString());
		std::cout << m2.ToString() << std::endl;

		Message m3 (LOGIN, sender, u1, g1, "zawartoœæ wiadomoœci");
		std::cout << m3.ToString() << std::endl;
		Message m4;
		m4.Parse(m3.ToString());
		std::cout << m4.ToString() << std::endl;

		Message m5 (RESULT, sender, u1, g1, "zawartoœæ wiadomoœci");
		m5.PreviousOperation = LOGIN;
		m5.PreviousResult = OK;
		m5.PreviusOperationInfo = "info";
		std::cout << m5.ToString() << std::endl;
		Message m6;
		m6.Parse(m5.ToString());
		std::cout << m6.ToString() << std::endl;


		Message m7 (USERLIST, sender, u1, g1, "zawartoœæ wiadomoœci");
		std::cout << m7.ToString() << std::endl;
		Message m8;
		m8.Parse(m7.ToString());
		std::cout << m8.ToString() << std::endl;

		//std::cout << (g1 == g2) << std::endl;
		//g1.GroupMembers.push_back(sender);
		//std::cout << (g1 == g2) << std::endl;
}



int _tmain(int argc, _TCHAR* argv[]){
	int port = 1986;
	int maxConnections = 255;
	std::cout << "Welcome to Simple Windows Communicator Server\nAuthor: Konrad Balys, OS2" << std::endl<< std::endl;
	std::cout << "Starting server..." << std::endl;
	try{
		Server * s = new Server(port, maxConnections);
		std::cout << "Server started at address:"  << GetHostName("localhost") << ", port: "<< port << ", max connections: " << maxConnections << std::endl;
		std::cout << "\nAvailable commands:\nq - stop and quit server\nstart - start server\nstop - stop server\nusers - print currently logged users\n" <<std::endl;
		
		std::string temp;
		
		std::map<std::string, Socket *> x;
		//Socket * SS = new ClientSocket("localhost", 1986);

		while (1) {
			temp = "";
			std::cin >> temp;
			if (temp == "q") {
				delete s;
				break;
			} else if (temp == "start"){
				s->Start();
			} else if (temp == "stop") {
				s->Stop();
			} else if (temp == "users") {
				std::cout << s->PrintUsers();
			}
			else {
				std::cout << "Unrecognized command. Please try again" << std::endl;
			}

			//_beginthreadex(0, 0, Connection, (void*) s, 0, &ret);
		}
		std::cout << "Closing server..." << std::endl;
	} catch (std::exception ex){
		std::cout << "Error occured: "<< ex.what()<< std::endl;
	}
  
  return 0;
}