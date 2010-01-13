#ifndef SOCKET_H
#define SOCKET_H
#include "stdafx.h"
#include "utilities.h"


class Socket{
private:
	static void Start();
	static void End();
	static int  SocketsCount;
protected:
	friend class ServerSocket;
	friend class ClientSocket;
	friend class SelectSocket;
	sockaddr_in Address;
	int nAddressSize;

	Socket(int socket);
	Socket();

	int SocketHandle;

	int* RefCount;

public:
	virtual ~Socket();
	Socket(const Socket& s);
	Socket& operator=(Socket& s);

	Result ReceiveLine(std::string &s);
	Result ReceiveBytes(std::string &s);
	Result ReceiveBytes(std::string &s, char delimiter);

	// parameter is modified here
	Result SendLine (std::string line);
	//parameter is not modified here
	Result SendBytes(const std::string& bytes);
	Result SendBytes(const std::string& s, char delimiter);
	void   Close();
};


class ClientSocket : public Socket {
public:
	ClientSocket(const std::string& host, int port);
};

class ServerSocket : public Socket {
public:
	ServerSocket(int port, int connections, bool blocking = true);

	Socket* Accept();
};

class SelectSocket {
public:
	static bool CanRead(Socket const * const s, bool blocking);
}; 

std::string GetHostName(std::string host);
#endif
