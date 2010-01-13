#include "stdafx.h"
#include "socket.h"
#define RECEIVE_BUFFER_SIZE 1024
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
int Socket::SocketsCount = 0;

void Socket::Start() {
	++SocketsCount;
}

void Socket::End() {
}

Socket::Socket() : SocketHandle(0) {
	Start();	
	RefCount = new int(1);
}

Socket::Socket(int s) : SocketHandle(s) {
	Start();
	RefCount = new int(1);
};

Socket::~Socket() {
	if (! --(*RefCount)) {
		Close();
		delete RefCount;
		RefCount = 0;
	}

	if (!(--SocketsCount)) 
	{
		End();
	}
}

Socket::Socket(const Socket& s) {
	RefCount = s.RefCount;
	(*RefCount)++;
	SocketHandle = s.SocketHandle;
	SocketsCount++;
}

Socket& Socket::operator=(Socket& s) {
	(*s.RefCount)++;

	RefCount = s.RefCount;
	SocketHandle = s.SocketHandle;
	SocketsCount++;
	return *this;
}


void Socket::Close() {
	close(SocketHandle);
}

Result Socket::ReceiveBytes(std::string &s) {
	char buf[RECEIVE_BUFFER_SIZE];

	/*while (1) {
		u_long arg = 0;
		if (ioctlsocket(SocketHandle, FIONREAD, &arg) != 0)
			return FAILED;

		if (arg == 0)
			return EMPTY;

		if (arg > RECEIVE_BUFFER_SIZE) 
			arg = RECEIVE_BUFFER_SIZE;

		int rv = recv (SocketHandle, buf, arg, 0);
		if (rv == 0) 
			return DISCONNECTED;
		if (rv < 0)
			return FAILED;

		std::string temp;

		temp.assign (buf, rv);
		s += temp;
	}*/
	return OK;
}

Result Socket::ReceiveBytes(std::string &s, char delimiter ) {
	while (1) {
		char c;
//		switch(recv(SocketHandle, &c, 1, 0)) {
		switch(read(SocketHandle, &c, 1)) {
			case 0: 
				return DISCONNECTED;
			case -1:
				return FAILED;
				//      if (errno == EAGAIN) {
				//        return ret;
				//      } else {
				//      // not connected anymore
				//      return "";
				//      }
			default: {
			}	
		}
		if (c != delimiter)
			s += c;
		else 
			return OK;
	}
	return OK;
}


Result Socket::ReceiveLine(std::string &s) {
	return ReceiveBytes(s, '\n');
}

Result Socket::SendLine(std::string s) {
	if (s[s.length() - 1] != '\n')
		s += '\n';
	if (write(SocketHandle, s.c_str(), s.length()) == SOCKET_ERROR)
		return FAILED;
	return OK;
}

Result Socket::SendBytes(const std::string& s) {
	if (write(SocketHandle, s.c_str(), s.length()) == SOCKET_ERROR)
		return FAILED;
	return OK;
}

Result Socket::SendBytes(const std::string& s, char delimiter) {
	std::string temp = s;
	temp += delimiter;
	if (write(SocketHandle, temp.c_str(), temp.length()) == SOCKET_ERROR)
		return FAILED;
	return OK;
}

// wywo³anie z klasy bazowej, niepotrzebne utworzenie bazowego socketa?
ServerSocket::ServerSocket(int port, int connections, bool blocking) {

	hostent* pHostInfo;   /* holds info about a machine */
	nAddressSize = sizeof(sockaddr_in);
	//memset(&Address, 0, sizeof(Address));
	Address.sin_addr.s_addr = INADDR_ANY;   
	Address.sin_port = htons(port);   
	Address.sin_family = AF_INET;       

	//SocketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SocketHandle = socket(AF_INET, SOCK_STREAM, 0);

	if (SocketHandle == INVALID_SOCKET) {
		std::cout << "INVALID_SOCKET\nCannot create server socket.\n";
		throw std::exception();		
	}

	/*if(!blocking) {
		u_long arg = 1;
		ioctlsocket(SocketHandle, FIONBIO, &arg);
	}*/

	/* bind the socket to the internet address */
	if (bind(SocketHandle, (sockaddr *)&Address, sizeof(Address)) == SOCKET_ERROR) {
		this->Close();
		std::cout << "INVALID_SOCKET\nCannot bind the socket to the internet address.\n";
		throw std::exception();
	}

	getsockname(SocketHandle, (sockaddr *) &Address,(socklen_t *)&nAddressSize);
	printf("opened socket as fd (%d) on port (%d) for stream i/o\n", SocketHandle, ntohs(Address.sin_port) );

        printf("Server\n\
              sin_family        = %d\n\
              sin_addr.s_addr   = %d\n\
              sin_port          = %d\n"
              , Address.sin_family
              , Address.sin_addr.s_addr
              , ntohs(Address.sin_port)
            );

	if(listen(SocketHandle, connections) == SOCKET_ERROR) {
	        printf("\nCould not listen\n");
        	throw std::exception();
	}                    
}

Socket* ServerSocket::Accept() {
	int newSocket = accept(SocketHandle, (sockaddr*)&Address, (socklen_t *)&nAddressSize);
	if (newSocket == INVALID_SOCKET) {
		std::cout << "Invalid Socket";
		return 0;		
	}

	Socket* temp = new Socket(newSocket);
	return temp;
}



ClientSocket::ClientSocket(const std::string& host, int port) : Socket() {
	// UDP: use SOCK_DGRAM instead of SOCK_STREAM
	SocketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (SocketHandle == INVALID_SOCKET) {
		std::cout << "INVALID_SOCKET\nCannot create socket.\n";
		throw std::exception();
	}
	hostent *pHostInfo;
	long nHostAddress;

	/* get IP address from name */
	pHostInfo = gethostbyname(host.c_str());
	/* copy address into long */
	memcpy(&nHostAddress, pHostInfo->h_addr, pHostInfo->h_length);

	/* fill address struct */
	Address.sin_addr.s_addr = nHostAddress;
	Address.sin_port = htons(port);
	Address.sin_family = AF_INET;

	printf("\nConnecting to %s on port %d", host.c_str(), port);
			
	/* connect to host */
	if(connect(SocketHandle, (sockaddr*)&Address, sizeof(Address)) == SOCKET_ERROR) {
	        printf("\nCould not connect to host\n");
		throw std::exception();
	}
}

bool SelectSocket::CanRead(Socket const* const s, bool blocking) {
	/*d_set SocketArray;
	FD_ZERO(&SocketArray);
	FD_SET(const_cast<Socket*>(s)->SocketHandle, &SocketArray);
	TIMEVAL tval;
	tval.tv_sec  = 2;
	tval.tv_usec = 0;

	TIMEVAL *ptval;
	if(!blocking) {
		ptval = &tval;
	}
	else { 
		ptval = 0;
	}

	if (select (0, &SocketArray, (fd_set*) 0, (fd_set*) 0, ptval) == SOCKET_ERROR) 
		std::cout << "Error in select";
	if (FD_ISSET(s->SocketHandle, &SocketArray)) 
		return true;*/
	return true;
}

std::string GetHostName(std::string host){
	hostent *hostInfo;
	if ((hostInfo = gethostbyname(host.c_str())) == 0) {
		std::cout << "Error with gethostbyname\n";
	}
	std::string toReturn(hostInfo->h_name);
	return toReturn;
}
