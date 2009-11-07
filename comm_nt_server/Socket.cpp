#include "stdafx.h"
#include "Socket.h"
#define RECEIVE_BUFFER_SIZE 1024
int Socket::SocketsCount = 0;

void Socket::Start() {
	if (!SocketsCount) {
		WSADATA info;
		if (WSAStartup(MAKEWORD(2,0), &info)) {
			throw SocketException("Could not start WSA");
		}
	}
	++SocketsCount;
}

void Socket::End() {
	WSACleanup();
}

Socket::Socket() : SocketHandle(0) {
	Start();
	// UDP: use SOCK_DGRAM instead of SOCK_STREAM
	SocketHandle = socket(AF_INET, SOCK_STREAM, 0);

	if (SocketHandle == INVALID_SOCKET) {
		throw SocketException("INVALID_SOCKET\nCannot create socket.");
	}

	RefCount = new int(1);
}

Socket::Socket(SOCKET s) : SocketHandle(s) {
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
	closesocket(SocketHandle);
}

Result Socket::ReceiveBytes(std::string &s) {
	char buf[RECEIVE_BUFFER_SIZE];

	while (1) {
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
	}
	return OK;
}

Result Socket::ReceiveBytes(std::string &s, char delimiter ) {
	while (1) {
		char c;
		switch(recv(SocketHandle, &c, 1, 0)) {
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
	if (send(SocketHandle, s.c_str(), s.length(), 0) == SOCKET_ERROR)
		return FAILED;
	return OK;
}

Result Socket::SendBytes(const std::string& s) {
	if (send(SocketHandle, s.c_str(), s.length(), 0) == SOCKET_ERROR)
		return FAILED;
	return OK;
}

Result Socket::SendBytes(const std::string& s, char delimiter) {
	std::string temp = s;
	temp += delimiter;
	if (send(SocketHandle, temp.c_str(), temp.length(), 0) == SOCKET_ERROR)
		return FAILED;
	return OK;
}

// wywo³anie z klasy bazowej, niepotrzebne utworzenie bazowego socketa?
ServerSocket::ServerSocket(int port, int connections, bool blocking) {
	sockaddr_in address;

	memset(&address, 0, sizeof(address));

	address.sin_family = PF_INET;             
	address.sin_port = htons(port);          
	SocketHandle = socket(AF_INET, SOCK_STREAM, 0);
	if (SocketHandle == INVALID_SOCKET) {
		throw SocketException("INVALID_SOCKET\nCannot create server socket.");
	}

	if(!blocking) {
		u_long arg = 1;
		ioctlsocket(SocketHandle, FIONBIO, &arg);
	}

	/* bind the socket to the internet address */
	if (bind(SocketHandle, (sockaddr *)&address, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		this->Close();
		throw SocketException("INVALID_SOCKET\nCannot bind the socket to the internet address.");
	}
	listen(SocketHandle, connections);                               
}

Socket* ServerSocket::Accept() {
	SOCKET newSocket = accept(SocketHandle, 0, 0);
	if (newSocket == INVALID_SOCKET) {
		int rc = WSAGetLastError();
		if(rc == WSAEWOULDBLOCK) {
			return 0; // non-blocking call, no request pending
		}
		else {
			throw SocketException("Invalid Socket");
		}
	}

	Socket* temp = new Socket(newSocket);
	return temp;
}



ClientSocket::ClientSocket(const std::string& host, int port) : Socket() {
	std::string error;

	hostent *hostInfo;
	if ((hostInfo = gethostbyname(host.c_str())) == 0 && (hostInfo = gethostbyaddr(host.c_str(), 4, AF_INET))) {
		throw SocketException(strerror(errno));
	}

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr = *((in_addr *)hostInfo->h_addr);
	memset(&(address.sin_zero), 0, 8); 

	if (::connect(SocketHandle, (sockaddr *) &address, sizeof(sockaddr))) {
		throw SocketException(strerror(WSAGetLastError()));
	}
}

bool SelectSocket::CanRead(Socket const* const s, bool blocking) {
	fd_set SocketArray;
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
		throw SocketException("Error in select");
	if (FD_ISSET(s->SocketHandle, &SocketArray)) 
		return true;
	return false;
}

std::string GetHostName(std::string host){
	hostent *hostInfo;
	if ((hostInfo = gethostbyname(host.c_str())) == 0) {
		throw SocketException(strerror(errno));
	}
	std::string toReturn(hostInfo->h_name);
	return toReturn;
}