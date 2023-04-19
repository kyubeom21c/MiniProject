#include "ATS_COM.h"

ATS_COM::ATS_COM(unsigned short port)
{
	sendData = new SendData;
	receiveData = new MissionData;
	udpSocket = new UDPSocket;

	sendData->curPos.x = 0.0;
	sendData->curPos.y = 0.0;
	sendData->sysState = "";

	receiveData->startPos.x = 0.0;
	receiveData->startPos.y = 0.0;
	receiveData->endPos.x = 0.0;
	receiveData->endPos.y = 0.0;
	receiveData->velocity = 0.0;
	receiveData->sysCommand = "";
	receiveData->threat = 0;

	udpPort = port;
	if (udpPort == 1000)
	{
		udpSocket->Bind(udpPort);
	}
}

ATS_COM::~ATS_COM()
{
	delete udpSocket;
	udpSocket = nullptr;
	delete sendData;
	sendData = nullptr;
	delete receiveData;
	receiveData = nullptr;
}

void ATS_COM::sendInformation(SendData sData)
{
	string sendStr;

	if (udpPort == 1001)
	{
		sendData->curPos.x = sData.curPos.x;
		sendData->curPos.y = sData.curPos.y;

		sendStr = makeMessage(sendData);

		udpSocket->SendTo(OPC_ADDR, OPC_PORT, sendStr.c_str(), sendStr.size());
	}
	if (udpPort == 1002)
	{
		sendData->sysState = sData.sysState;

		sendStr = makeMessage(sendData);

		udpSocket->SendTo(OPC_ADDR, OPC_LIFE_PORT, sendStr.c_str(), sendStr.size());
	}
}

MissionData* ATS_COM::receiveInformation()
{
	char buff[100];
	MissionData* rData = new MissionData;

	udpSocket->RecvFrom(buff, sizeof(buff), 0);

	rData = processMessage(buff);

	return rData;
}

string ATS_COM::makeMessage(SendData *sData)
{
	string sendStr = "";
	string del = ",";

	if (udpPort == 1001)
	{
		string curX = to_string(sData->curPos.x);
		string curY = to_string(sData->curPos.y);

		sendStr = curX + del + curY; // 2 - msg
	}
	if (udpPort == 1002)
	{
		string state = sData->sysState;

		sendStr = state; // 1 - msg
	}

	return sendStr;
}

MissionData* ATS_COM::processMessage(char* rData)
{
	string rStr = "";
	string pStr[OPC_MSG_CNT];
	vector<string> vStr;

	rStr.append(rData);

	vStr = splitMessage(rStr, ',');

	if (vStr[0] == "start")
	{
		receiveData->sysCommand = vStr[0];
	}
	else if (vStr[0] == "down")
	{
		receiveData->sysCommand = vStr[0];
	}
	else
	{
		for (int i = 0; i < OPC_MSG_CNT; i++)
		{
			pStr[i] = vStr[i];
		}

		receiveData->startPos.x = atof(pStr[0].c_str());
		receiveData->startPos.y = atof(pStr[1].c_str());
		receiveData->endPos.x = atof(pStr[2].c_str());
		receiveData->endPos.y = atof(pStr[3].c_str());
		receiveData->threat = atoi(pStr[4].c_str());
		receiveData->velocity = atof(pStr[5].c_str());
	}

	return receiveData;
}

vector<string> ATS_COM::splitMessage(string str, char delimiter)
{
	vector<string> answer;
	stringstream sStr(str);
	string temp;

	while (getline(sStr, temp, delimiter)) 
	{
		answer.push_back(temp);
	}

	return answer;
}