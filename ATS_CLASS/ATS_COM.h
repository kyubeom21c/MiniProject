#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include "ATS_DEF.h"
#include "UDPSocket.h"

#define OPC_ADDR		"127.0.0.1" // localhost
#define ATS_PORT		1000
#define OPC_PORT		1001
#define OPC_LIFE_PORT	1002

#define OPC_MSG_CNT		6

using namespace std;

class ATS_COM : public UDPSocket
{
public:
	ATS_COM(unsigned short port);
	~ATS_COM();
	void sendInformation(SendData sData);
	MissionData* receiveInformation();

private:
	vector<string> splitMessage(string str, char delimiter);
	string makeMessage(SendData* sData);
	MissionData* processMessage(char* rData);

private:
	unsigned short udpPort;
	SendData* sendData;
	MissionData* receiveData;
	UDPSocket* udpSocket;
};

