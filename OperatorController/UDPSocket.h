#pragma once
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <system_error>
#include <string>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

#define TIMEOUT_MS 5000

class UDPSocket
{
public:
	UDPSocket();
	~UDPSocket();
	
	void SendTo(const std::string& address, unsigned short port, const char* buffer, int len, int flags = 0);
	void SendTo(sockaddr_in& address, const char* buffer, int len, int flags = 0);
	sockaddr_in RecvFrom(char* buffer, int len, int flags = 0);
	bool RecvFromForLife(char* buffer, int len, int flags = 0);
	void Bind(unsigned short port);
	
private:
	SOCKET sock;
};

class WSASession
{
public:
	WSASession();
	~WSASession();
	
private:
	WSAData data;
};